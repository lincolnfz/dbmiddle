#include "StdAfx.h"
#include "UDTExpand.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <wspiapi.h>
#include "UDTUnit.h"

#ifdef  _SERVER
	#include "../third_party/tcmalloc.h"
	#ifdef _DEBUG
		#pragma comment(lib, "../third_lib/libtcmalloc_minimal-debug.lib")
	#else
		#pragma comment(lib, "../third_lib/libtcmalloc_minimal.lib") 
	#endif
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


using namespace std;

const int g_IP_Version = AF_INET;
const int g_Socket_Type = SOCK_DGRAM;


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

int connect(UDTSOCKET& usock, int port, int version, int type)
{
	addrinfo hints, *peer;

	memset(&hints, 0, sizeof(struct addrinfo));

	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = version;
	hints.ai_socktype = type;

	char buffer[16];
	sprintf(buffer, "%d", port);

	if (0 != getaddrinfo("127.0.0.1", buffer, &hints, &peer))
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

		int activeNum = UDT::epoll_wait( eid , &readfds , NULL , INFINITE );
		if ( activeNum > 0 )
		{
			int recvLen = 0;
			for ( it = readfds.begin() ; it != readfds.end(); ++it )
			{
				while( 0 != (recvLen = UDT::recv( *it , recvBuf , 1024 , 0 )) )
				{
					if ( UDT::ERROR == recvLen )
					{
						//UDT::getlasterror().getErrorMessage();
						break;
					} 
					else
					{
						//正常收到数据
					}
				}


			}
		}

	}
	UDT::epoll_release(eid);
	return 0;
}

CUDTExpand::CUDTExpand(void)
{
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

int CUDTExpand::procRecvData( char* pData , unsigned int ulen )
{
	return 0;
}

//hool
//{
//	UDTSOCKET UDTSocket =UDT::socket(AF_INET, SOCK_DGRAM, 0); 
//	UINT nOptValue = 1;    
//	if ( UDT::setsockopt (UDTSocket, SOL_SOCKET,UDT_RENDEZVOUS,(char*)&nOptValue , sizeof(UINT) ) !=0)
//	{
//		TRACE("setsockopt SO_REUSEADDR 失败%s \n",UDT::getlasterror().getErrorMessage());
//		return;
//	}
//
//	if (UDT::ERROR == UDT::bind(UDTSocket, Socket))
//	{
//		TRACE("UDT绑定失败:%s " , UDT::getlasterror().getErrorMessage());
//		return;
//
//	}    
//
//	if (UDT::ERROR == UDT::connect(UDTSocket, (const struct sockaddr *)&addr,sizeof(addr)))
//	{
//		TRACE("UDT连接失败:%s\n ",UDT::getlasterror().getErrorMessage());        
//		return;            
//	}
//}



