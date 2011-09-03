#include "StdAfx.h"
#include "UDTExpand.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <wspiapi.h>
#include "UDTUnit.h"
#include "suUtil.h"

using namespace std;

const int g_IP_Version = AF_INET;
const int g_Socket_Type = SOCK_DGRAM;
CUDTExpand* pUDTExpand = NULL;


int createUDTSocket(UDTSOCKET& usock, int version = AF_INET, int type = SOCK_STREAM, int port = 0, bool rendezvous = false)
{
	addrinfo hints;
	addrinfo* res;

	memset(&hints, 0, sizeof(struct addrinfo));

	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = version;
	hints.ai_socktype = type;

	char service[16];
	sprintf(service, "%d", port);

	if (0 != getaddrinfo(NULL, service, &hints, &res))
	{
		//cout << "illegal port number or port is busy.\n" << endl;
		return -1;
	}

	usock = UDT::socket(res->ai_family, res->ai_socktype, res->ai_protocol);

	// since we will start a lot of connections, we set the buffer size to smaller value.
	int snd_buf = 64000;
	int rcv_buf = 64000;
	UDT::setsockopt(usock, 0, UDT_SNDBUF, &snd_buf, sizeof(int));
	UDT::setsockopt(usock, 0, UDT_RCVBUF, &rcv_buf, sizeof(int));
	snd_buf = 64000;
	rcv_buf = 64000;
	UDT::setsockopt(usock, 0, UDP_SNDBUF, &snd_buf, sizeof(int));
	UDT::setsockopt(usock, 0, UDP_RCVBUF, &rcv_buf, sizeof(int));
	int fc = 64;
	UDT::setsockopt(usock, 0, UDT_FC, &fc, sizeof(int));
	bool reuse = true;
	UDT::setsockopt(usock, 0, UDT_REUSEADDR, &reuse, sizeof(bool));
	UDT::setsockopt(usock, 0, UDT_RENDEZVOUS, &rendezvous, sizeof(bool));

	if (UDT::ERROR == UDT::bind(usock, res->ai_addr, res->ai_addrlen))
	{
		//cout << "bind: " << UDT::getlasterror().getErrorMessage() << endl;
		return -1;
	}

	freeaddrinfo(res);
	return 0;
}

int connect(UDTSOCKET& usock, char* nodename , int port, int version, int type)
{
	addrinfo hints, *peer;

	memset(&hints, 0, sizeof(struct addrinfo));

	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = version;
	hints.ai_socktype = type;

	char buffer[16];
	sprintf(buffer, "%d", port);

	if (0 != getaddrinfo(nodename, buffer, &hints, &peer))
	{
		return 0;
	}

	UDT::connect(usock, peer->ai_addr, peer->ai_addrlen);

	freeaddrinfo(peer);

	return 0;
}

unsigned int __stdcall ListenUDTData(LPVOID lpParameter)
{
	UDTSOCKET udtSock = *((UDTSOCKET*)lpParameter);
	sockaddr_storage remoteAddr;
	int nlen = sizeof(remoteAddr);
	char recvBuf[1024];

	if( UDT::listen( udtSock , 50 ) != 0 )
	{
		return 0;
	}
	int eid = UDT::epoll_create();
	set<UDTSOCKET> readfds;
	set<UDTSOCKET>::iterator it;

	while(1) 
	{
		UDTSOCKET remoteSock = UDT::accept(udtSock ,(sockaddr*)&remoteAddr , &nlen );
		if ( UDT::INVALID_SOCK == remoteSock )
		{
			continue;
		}
		UDT::epoll_add_usock(eid , remoteSock );
		pUDTExpand->AddNewContent( remoteSock , remoteAddr );

		int activeNum = UDT::epoll_wait( eid , &readfds , NULL , INFINITE );
		if ( activeNum > 0 )
		{
			int recvLen = 0;
			for ( it = readfds.begin() ; it != readfds.end(); ++it )
			{
				UDTSOCKET sock_peer = *it;
				while( 0 != (recvLen = UDT::recv( sock_peer , recvBuf , 1024 , 0 )) )
				{
					if ( UDT::ERROR == recvLen )
					{
						UDT::getlasterror().getErrorMessage();
						pUDTExpand->RemoveContent( eid , sock_peer );
						break;
					} 
					else
					{						
						//正常收到数据,提交到远程处理的客户端处理
						pUDTExpand->SubmitTask( sock_peer , (const char*&)recvBuf , recvLen );
						
					}
				}


			}
		}

	}
	UDT::epoll_release(eid);
	return 0;
}

unsigned long __stdcall procDataThread( /*in*/void* lpParam , /*out*/void* funContext )
{
	CUDTExpand::PEERPARM* parm = (CUDTExpand::PEERPARM*)lpParam;
	pUDTExpand->procRecvData( parm->sock , (const char*&)parm->buf , parm->buf_len );

	delete parm->buf;
	delete parm;
	return 0;
}

CUDTExpand::CUDTExpand(void)
{
	pUDTExpand = this;
	SYSTEM_INFO systeminfo;
	GetSystemInfo(&systeminfo);
	DWORD nkernel = systeminfo.dwNumberOfProcessors;
	m_TaskPool.Init( nkernel<<1 );
}


CUDTExpand::~CUDTExpand(void)
{
}

void CUDTExpand::UDT_srv()
{
	UDT::startup();
	unsigned int nid;
	int port = 8902;
	while( createUDTSocket(m_UDTSock_srv, AF_INET, SOCK_STREAM , port) < 0 )
	{
		++port;
	}
	HANDLE hThread = (HANDLE)_beginthreadex( NULL , 0 , ListenUDTData , (void*)&m_UDTSock_srv, 0 , &nid );
	CloseHandle(hThread);
}

void CUDTExpand::UDT_clean()
{
	UDT::close(m_UDTSock_srv);
	UDT::cleanup();
}

int CUDTExpand::SubmitTask( const UDTSOCKET& sock , const char*& buf , const int& len )
{
	PEERPARM* p = new PEERPARM;
	p->sock = sock;
	p->buf = new char[len];
	p->buf_len = len;
	memcpy( p->buf , buf , p->buf_len );
	m_TaskPool.SubmitTaskItem( procDataThread , p );
	return 0;
}

CUDTUnit* CUDTExpand::GetNewUDTUnit()
{
	return new CUDTUnit( );
}

void CUDTExpand::AddNewContent( UDTSOCKET& remoteSock , sockaddr_storage& remoteAddr )
{
	CUDTUnit* p = GetNewUDTUnit();
	p->setkey( remoteSock );
	p->setudtsock( remoteSock );
	p->setaddr( remoteAddr );
	std::pair<REMOTE_CLIENT_MAP::iterator , bool > ret;
	m_mutex.lock();
	ret = m_remote_peers.insert( REMOTE_CLIENT_MAP::value_type( remoteSock , p ) );
	m_mutex.unlock();
	if ( ret.second == false )
	{
		//发生了错误
		delete p;
	}
	
}

void CUDTExpand::RemoveContent( int& eid , UDTSOCKET& remoteSock )
{
	CUDTUnit* pUnit = NULL;

	m_mutex.lock();
	REMOTE_CLIENT_MAP::iterator it = m_remote_peers.find( remoteSock );
	if ( it != m_remote_peers.end() )
	{
		pUnit = it->second;		
		m_remote_peers.erase( it );				
	}
	m_mutex.unlock();

	if ( pUnit )
	{
		delete pUnit;
	}
	UDT::epoll_remove_usock( eid , remoteSock );
}

int CUDTExpand::procRecvData( const UDTSOCKET& sock , const char*& buf , const int& len )
{
	m_mutex.lock();
	REMOTE_CLIENT_MAP::iterator it = m_remote_peers.find( sock );
	m_mutex.unlock();
	if( it != m_remote_peers.end() )
	{
		CUDTUnit* pUnit = it->second;
		pUnit->AnalyzePack( buf , len );
	}
	return 0;
}

void CUDTExpand::udpHole( const char*& remoteIP , const int& port )
{
	sockaddr_in remoteAddr;
	remoteAddr.sin_family = AF_INET;
	remoteAddr.sin_addr.s_addr = inet_addr(remoteIP);
	remoteAddr.sin_port = htons(port);
	SOCKET udpsock = socket( AF_INET , SOCK_DGRAM , 0 );
	if ( udpsock == INVALID_SOCKET )
	{
		return;
	}
	char buf[1] = {0};
	sendto( udpsock , buf , 1 , 0 , (sockaddr*)&remoteAddr , sizeof(sockaddr_in) );
	
	
}

int CUDTExpand::bindUDPSock( const SOCKET& udpsock , const sockaddr_in& addr )
{
	UDTSOCKET UDTSocket =UDT::socket(AF_INET, SOCK_DGRAM, 0); 
	UINT nOptValue = 1;    
	if ( UDT::setsockopt (UDTSocket, SOL_SOCKET,UDT_RENDEZVOUS,(char*)&nOptValue , sizeof(UINT) ) !=0)
	{
		//TRACE("setsockopt SO_REUSEADDR 失败%s \n",UDT::getlasterror().getErrorMessage());
		return 0;
	}
	
	if (UDT::ERROR == UDT::bind(UDTSocket, udpsock))
	{
		//TRACE("UDT绑定失败:%s " , UDT::getlasterror().getErrorMessage());
		return 0;
	
	}    
	
	if (UDT::ERROR == UDT::connect(UDTSocket, (const struct sockaddr *)&addr,sizeof(addr)))
	{
		//TRACE("UDT连接失败:%s\n ",UDT::getlasterror().getErrorMessage());        
		return 0;            
	}
	return UDTSocket;
}



