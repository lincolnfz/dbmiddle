/*
�����ص�ģ��д���첽sock
author: lincoln
email: lincolnfz@gmail.com
*/
#include "StdAfx.h"
#include "AsynSock.h"
#include "suUtil.h"
#include "AdoClientManager.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CAsynSock* pAsynSock;

CAsynSock::CAsynSock(CAdoClientManager* pAdoClientManager)
{
	m_sock = INVALID_SOCKET;
	memset(m_srvip,0,16);
	memset(m_proxyip,0,16);
	memset(m_szBuffer,0,2048);
	memset(m_szAuth,0,1024);
	memset(m_szAuthT,0,1024);
	m_port = 0;
	m_proxyport = 0;
	m_iRunProxyTimes = 0;
	m_bRun = false;
	pAsynSock = this;
	m_bConnected = false;
	m_bAuth = false;
	m_pAdoClientManager = pAdoClientManager;
	m_hPackData = HeapCreate(HEAP_GENERATE_EXCEPTIONS,0,0);
	m_eProxyType = DIRECT;
	m_hConnected = CreateEvent(NULL,TRUE,FALSE,NULL);
}

CAsynSock::~CAsynSock(void)
{
	HeapDestroy(m_hPackData);
	CloseHandle(m_hConnected);
}

void CAsynSock::Clean()
{
	while (m_nTotalEvent)
	{
		WSACloseEvent(m_eventArray[--m_nTotalEvent]);
	}

	WSACleanup();
}

//��ʼ���ص�io
void CAsynSock::Init()
{
	m_iRunProxyTimes = 0;
	m_nTotalEvent = 0;
	m_eventArray[m_nTotalEvent++] = WSACreateEvent();
	m_eventArray[m_nTotalEvent++] = WSACreateEvent();

	memset(&m_recvOvl,0,sizeof(OVLDATA));
	m_recvOvl.ovl.hEvent = m_eventArray[0];
	m_recvOvl.wsabuf.buf = m_recvOvl.databuf;
	m_recvOvl.wsabuf.len = MAX_BUFFER_SIZE;

	memset(&m_sendOvl,0,sizeof(OVLDATA));
	m_sendOvl.ovl.hEvent = m_eventArray[1];
	m_sendOvl.wsabuf.buf = m_sendOvl.databuf;
	m_sendOvl.wsabuf.len = 0;

	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD( 2, 2 );
	int err = WSAStartup( wVersionRequested, &wsaData );

}

bool CAsynSock::Connect()
{
	static int nTimes = 0;
	sockaddr_in addr;
	
	int err;
	DWORD dwFlag,dwlen,dwTrans;

	//��ʼ������
	Init();
	m_sock = WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED);
	if(m_sock == SOCKET_ERROR)
	{
		err = WSAGetLastError();		
		closesocket(m_sock);
		Clean();
		return false;
	}

	memset(&addr,0,sizeof(sockaddr_in));
	addr.sin_family = AF_INET;
	if(m_eProxyType == DIRECT)
	{
		//ֱ������
		addr.sin_addr.S_un.S_addr = inet_addr(m_srvip);
		addr.sin_port = ntohs(m_port);
	}
	else
	{
		//��������
		addr.sin_addr.S_un.S_addr = inet_addr(m_proxyip);
		addr.sin_port = ntohs(m_proxyport);
	}

	if(connect(m_sock,(struct sockaddr *)&addr,sizeof(sockaddr)) == SOCKET_ERROR)
	{
		closesocket(m_sock);
		Clean();		
		return false;
	}
	if(m_eProxyType == DIRECT)
	{
		//�����ֱ��,���е���˵���Գɹ����Ϸ�����������ʼ��������	
		dwFlag = dwTrans = 0;
		memset(&(m_recvOvl.ovl),0,sizeof(WSAOVERLAPPED));
		m_recvOvl.ovl.hEvent = pAsynSock->m_eventArray[0];
		err = WSARecv(m_sock,&(m_recvOvl.wsabuf),1,&dwTrans,&dwFlag,
			&(m_recvOvl.ovl),NULL);
		if((err == SOCKET_ERROR) && (WSAGetLastError() != WSA_IO_PENDING))
		{
			closesocket(m_sock);
			Clean();
			return false;
		}
		m_bConnected = true;
		SetEvent(m_hConnected);
	}

	//�������繤���߳�
	unsigned int iID;
	m_hIOThread = (HANDLE)_beginthreadex(NULL,0,IOWorkThread,this,0,&iID);
	

	dwFlag = dwlen = dwTrans = 0;

	//׼�����Ӵ������������
	if(m_eProxyType == HTTP)
	{
		if (m_bAuth)
		{
			//Proxy-Authorization: Basic 
			sprintf(m_szAuthT, "%s:%s", m_proxyUsernam.c_str(),m_proxyPassword.c_str());
			CsuUtil::to64frombits((UCHAR *)m_szAuth, (UCHAR *)m_szAuthT, strlen(m_szAuthT));
			sprintf(m_szBuffer, 
				"CONNECT %s:%d HTTP/1.1\r\nProxy-Authorization: Basic %s\r\n\r\n", 
				m_srvip, m_port, m_szAuth);
		} 
		else
		{
			sprintf(m_szBuffer,"CONNECT %s:%d HTTP/1.1\r\n\r\n",m_srvip,m_port);
		}
		dwlen = strlen(m_szBuffer);
		memcpy(m_sendOvl.databuf,m_szBuffer,dwlen);
	}
	else if (m_eProxyType == SOCK)
	{
		if (m_bAuth)
		{
			m_szBuffer[0] = 5;
			m_szBuffer[1] = 2;
			m_szBuffer[2] = 0;
			m_szBuffer[3] = 2;
			dwlen = 4;
			memcpy(m_sendOvl.databuf,m_szBuffer,dwlen);
		} 
		else
		{
			m_szBuffer[0] = 5;
			m_szBuffer[1] = 1;
			m_szBuffer[2] = 0;
			dwlen = 3;
			memcpy(m_sendOvl.databuf,m_szBuffer,dwlen);
		}
	}
	//׼�����Ӵ������������.���


	if(m_eProxyType != DIRECT)
	{
		//�������ֱ��,�ȴ��ʹ���
		m_sendOvl.wsabuf.len = dwlen;
		dwFlag = dwTrans = 0;

		memset(&(m_sendOvl.ovl),0,sizeof(WSAOVERLAPPED));
		m_sendOvl.ovl.hEvent = m_eventArray[1];
		err = WSASend(m_sock,&(m_sendOvl.wsabuf),1,&dwTrans,dwFlag,&(m_sendOvl.ovl),NULL);
		if((err == SOCKET_ERROR) && (WSAGetLastError() != WSA_IO_PENDING))
		{
			closesocket(m_sock);
			Clean();
			return false;
		}
	}

	m_bRun = true;
	return true;
}

bool CAsynSock::DisConnect()
{
	ResetEvent(m_hConnected);	
	closesocket(m_sock);
	Clean();
	m_bConnected = m_bRun = false;

	if (m_recvOvl.pdata)
	{
		releaseBuf(m_recvOvl.pdata);
		m_recvOvl.pdata = NULL;
	}
	if(m_sendOvl.pdata)
	{
		delete[] m_sendOvl.pdata;
		m_sendOvl.pdata = NULL;
	}

	return true;
}

unsigned int CAsynSock::IOWorkThread(LPVOID lpParameter)
{
	DWORD dwTrans,dwFlag,dwIdx,dwErr;
	dwTrans = dwFlag = dwIdx = 0;
	PACKHEAD packhead;
	long lrecv;

	while (true)
	{
		dwIdx = WSAWaitForMultipleEvents(pAsynSock->m_nTotalEvent,pAsynSock->m_eventArray,FALSE,WSA_INFINITE,TRUE);
		if (WAIT_IO_COMPLETION == dwIdx)
		{
			break;
		}
		dwIdx -= WSA_WAIT_EVENT_0;
		WSAResetEvent(pAsynSock->m_eventArray[dwIdx]);
		dwTrans = dwFlag = 0;
		if(dwIdx == 0)
		{
			//������Ϣ
			WSAGetOverlappedResult(pAsynSock->m_sock,&(pAsynSock->m_recvOvl.ovl),&dwTrans,FALSE,&dwFlag);
		}
		else if(dwIdx == 1)
		{
			//������Ϣ
			WSAGetOverlappedResult(pAsynSock->m_sock,&(pAsynSock->m_sendOvl.ovl),&dwTrans,FALSE,&dwFlag);
		}
		else
		{
			continue;
		}

		if(dwTrans == 0)
		{
			//�������
			break;
		}
		if(dwIdx == 0)//wsarecv ����
		{	
			//��ʼ�����յ��İ�
			if(pAsynSock->m_bConnected == false)
			{
				//ʹ���˴�������
				if(pAsynSock->m_eProxyType == HTTP)
				{
					if(strnicmp(pAsynSock->m_recvOvl.databuf, "HTTP/1.0 200", strlen("HTTP/1.0 200")) != 0 && 
						strnicmp(pAsynSock->m_recvOvl.databuf, "HTTP/1.1 200", strlen("HTTP/1.1 200")) != 0)
					{
						//�޷�ʹ��http����
						break;
						//return -1;
					}
					else
					{
						//http�ɹ����Ϸ�����
						pAsynSock->m_bConnected = true;
						SetEvent(pAsynSock->m_hConnected);
					}
				}
				else if(pAsynSock->m_eProxyType == SOCK)
				{
					//ʹ��sock���� 
					if (pAsynSock->m_bAuth)
					{
						//��Ҫ��֤
					} 
					else
					{
						//������֤
						if(pAsynSock->m_recvOvl.databuf[0] != 5 || pAsynSock->m_recvOvl.databuf[1] != 0)
						{
							//ʧ��
							break;
							//return -1;
						}
						if(pAsynSock->m_iRunProxyTimes == 0)
						{
							//translate DestAddr
							pAsynSock->m_iRunProxyTimes++;
							pAsynSock->TranslateDestAddr();
						}
						else
						{
							//sock�ɹ����Ϸ�����
							pAsynSock->m_bConnected = true;
							SetEvent(pAsynSock->m_hConnected);
						}
					}
				}
			}
			else
			{
				//���������ķ��������ݰ�
				long lRemainProc = dwTrans;
				char* pvist = pAsynSock->m_recvOvl.databuf;
				while (lRemainProc > 0)
				{					
					if (pAsynSock->m_recvOvl.lremain == 0)
					{
						//�µİ�
						memcpy(&packhead,pvist,sizeof(PACKHEAD));//�õ����ݰ�����
						pvist += sizeof(PACKHEAD);
						pAsynSock->m_recvOvl.lpacklen = packhead.lpacklen;
						pAsynSock->m_recvOvl.id = packhead.dwid;
						pAsynSock->m_recvOvl.ltrans = 0;
						pAsynSock->m_recvOvl.lremain = lRemainProc - sizeof(PACKHEAD) - pAsynSock->m_recvOvl.lpacklen;
						if(pAsynSock->m_recvOvl.lremain < 0)
							pAsynSock->m_recvOvl.lremain = 0 - pAsynSock->m_recvOvl.lremain;
						else
							pAsynSock->m_recvOvl.lremain = 0;

						//�����һ���յ����ĳ���,ע�������µİ�
						lrecv = pAsynSock->m_recvOvl.lpacklen - pAsynSock->m_recvOvl.lremain;
						
						pAsynSock->m_recvOvl.pdata = (char*)HeapAlloc(pAsynSock->m_hPackData,
							HEAP_ZERO_MEMORY,pAsynSock->m_recvOvl.lpacklen+1);
						memcpy(pAsynSock->m_recvOvl.pdata,pvist,lrecv);
						pvist += lrecv;

						//��һ���ܹ��յ��˶����ֽ�
						pAsynSock->m_recvOvl.ltrans = lrecv;

						lRemainProc = lRemainProc - sizeof(PACKHEAD) - lrecv; //wsabuf�л��ж����ֽ�û����
					} 
					else
					{
						//�����İ�
						//���㱾���յ��������ĳ���
						lrecv = pAsynSock->m_recvOvl.lremain - lRemainProc;
						if(lrecv<0)
							lrecv = pAsynSock->m_recvOvl.lremain;
						else
							lrecv = lRemainProc;
						
						//�����°��ĳ���
						pAsynSock->m_recvOvl.lremain -= lrecv;
						if(pAsynSock->m_recvOvl.lremain < 0)
							pAsynSock->m_recvOvl.lremain = 0;

						//��������
						memcpy(pAsynSock->m_recvOvl.pdata + pAsynSock->m_recvOvl.ltrans,
							pvist,lrecv);
						pvist += lrecv;

						//��ֹ��ǰ��������ϵ����ݳ���
						pAsynSock->m_recvOvl.ltrans += lrecv;

						lRemainProc = lRemainProc - lrecv; //wsabuf�л��ж����ֽ�û����
					}

					if(pAsynSock->m_recvOvl.lremain == 0)
					{
						//�յ���һ�����������ݰ�

						//pAsynSock->m_recvOvl.id			//���id
						//pAsynSock->m_recvOvl.pdata	//�������
						//pAsynSock->m_recvOvl.lpacklen	//��ų���
						pAsynSock->m_pAdoClientManager->recvAdoData(pAsynSock->m_recvOvl.id,
							pAsynSock->m_recvOvl.lpacklen, pAsynSock->m_recvOvl.pdata);
						pAsynSock->m_recvOvl.pdata = NULL;
					}

					

				}
				//�����������ݰ�����
			}

			//���������
			memset(&(pAsynSock->m_recvOvl.ovl),0,sizeof(WSAOVERLAPPED));
			pAsynSock->m_recvOvl.ovl.hEvent = pAsynSock->m_eventArray[0];
			dwTrans = dwFlag = 0;
			if(WSARecv(pAsynSock->m_sock,&(pAsynSock->m_recvOvl.wsabuf),1,&dwTrans,&dwFlag,
				&(pAsynSock->m_recvOvl.ovl),NULL) == SOCKET_ERROR)
			{
				dwErr = WSAGetLastError();
				if(dwErr != WSA_IO_PENDING)
					break;
			}
		}
		else if (dwIdx == 1)
		{
			if(pAsynSock->m_bConnected==false)
			{
				//��û������,˵����ͨ�Ŵ����������֪ͨ
				dwTrans = dwFlag = 0;
				memset(&(pAsynSock->m_recvOvl.ovl),0,sizeof(WSAOVERLAPPED));
				pAsynSock->m_recvOvl.ovl.hEvent = pAsynSock->m_eventArray[0];
				if(WSARecv(pAsynSock->m_sock,&(pAsynSock->m_recvOvl.wsabuf),1,&dwTrans,&dwFlag,
					&(pAsynSock->m_recvOvl.ovl),NULL) == SOCKET_ERROR)				
				{
					dwErr = WSAGetLastError();
					if(dwErr != WSA_IO_PENDING)
						break;
				}
				continue;
			}
			//wsasend����
			//�����ܼƷ����˶����ֽ�
			if(pAsynSock->m_sendOvl.ntimes == 0)
			{
				pAsynSock->m_sendOvl.ntimes++;
				pAsynSock->m_sendOvl.ltrans = pAsynSock->m_sendOvl.ltrans + dwTrans - sizeof(PACKHEAD);
			}
			else
			{
				pAsynSock->m_sendOvl.ltrans = pAsynSock->m_sendOvl.ltrans + dwTrans;
			}
			//���㻹�ж����ֽ�û����
			pAsynSock->m_sendOvl.lremain = pAsynSock->m_sendOvl.lpacklen - pAsynSock->m_sendOvl.ltrans;
			if(pAsynSock->m_sendOvl.lremain <= 0)
			{
				//��ǰ���ݰ��������
				delete[] pAsynSock->m_sendOvl.pdata;//���������ķ��Ϳռ�
				pAsynSock->m_sendOvl.pdata = NULL;
				pAsynSock->m_pAdoClientManager->CompleteSend(pAsynSock->m_sendOvl.id);
				continue; 
			}

			if(pAsynSock->m_sendOvl.lremain <= MAX_BUFFER_SIZE)
			{
				memcpy(pAsynSock->m_sendOvl.wsabuf.buf,
					pAsynSock->m_sendOvl.pdata+pAsynSock->m_sendOvl.ltrans, pAsynSock->m_sendOvl.lremain);

				pAsynSock->m_sendOvl.wsabuf.len = pAsynSock->m_sendOvl.lremain;
			}
			else
			{
				memcpy(pAsynSock->m_sendOvl.wsabuf.buf,
					pAsynSock->m_sendOvl.pdata + pAsynSock->m_sendOvl.ltrans, MAX_BUFFER_SIZE);

				pAsynSock->m_sendOvl.wsabuf.len = MAX_BUFFER_SIZE;
			}
			dwTrans = dwFlag = 0;

			memset(&(pAsynSock->m_sendOvl.ovl),0,sizeof(WSAOVERLAPPED));
			pAsynSock->m_sendOvl.ovl.hEvent = pAsynSock->m_eventArray[1];
			if(WSASend(pAsynSock->m_sock,&(pAsynSock->m_sendOvl.wsabuf),1,&dwTrans,dwFlag,
				&(pAsynSock->m_sendOvl.ovl),NULL) == SOCKET_ERROR)
			{
				dwErr = WSAGetLastError();
				if(dwErr != WSA_IO_PENDING)
					break;
			}

		}
	}

	pAsynSock->DisConnect();


	return 0;
}

//sock�����£���������������Զ�̷�������ַ
void CAsynSock::TranslateDestAddr()
{
	//translate DestAddr
	m_sendOvl.databuf[0] = 5;
	m_sendOvl.databuf[1] = 1;
	m_sendOvl.databuf[2] = 0;
	m_sendOvl.databuf[3] = 3;//DOMAIN
	m_sendOvl.databuf[4] = strlen(m_srvip);//domain len
	strcpy(m_sendOvl.databuf + 5, m_srvip);
	unsigned short uPort = htons(m_port);
	memcpy(m_sendOvl.databuf + 5 + strlen(m_srvip), &uPort, 2);
	m_sendOvl.wsabuf.len = 5 + strlen(m_srvip) + 2;
	DWORD dwFlag,dwTrans;
	dwFlag = dwTrans = 0;

	memset(&(m_sendOvl.ovl),0,sizeof(WSAOVERLAPPED));
	//m_sendOvl.ovl.hEvent = m_eventArray[1];
	WSASend(m_sock,&(m_sendOvl.wsabuf),1,&dwTrans,dwFlag,
		&(m_sendOvl.ovl),NULL);
}

void CAsynSock::SetServer(string serverip,int port)
{
	strcpy(m_srvip,serverip.c_str());
	m_port = port;
}

void CAsynSock::SetProxy(string proxyip,int proxyport,PROXYTYPE pt,
						 string proxyUsernam,string proxyPassword)
{
	//ASSERT(pt == DIRECT);
	m_eProxyType = pt;
	strcpy(m_proxyip,proxyip.c_str());
	m_proxyport = proxyport;
	if(!proxyUsernam.empty() || !proxyPassword.empty())
	{
		m_bAuth = true;
	}
}

bool CAsynSock::SendData(DWORD id,char* pack,int len)
{
	//û�����Ϸ������Զ�����
	if(m_bConnected == false)
	{
		if(Connect())
		{
			if(WaitForSingleObject(m_hConnected,60000) == WAIT_TIMEOUT)
			{
				delete[] pack;
				return false;
			}
		}
		else
		{
			delete[] pack;
			return false;
		}
	}
	//SendPack
	PACKHEAD packhead;
	m_sendOvl.lpacklen = len;
	m_sendOvl.lremain = len;
	m_sendOvl.ltrans = 0;
	m_sendOvl.pdata = pack;
	m_sendOvl.ntimes = 0;
	m_sendOvl.id = id;

	packhead.lpacklen = len;
	packhead.dwid = id;
	memcpy(m_sendOvl.wsabuf.buf, &packhead, sizeof(PACKHEAD));
	if (len <= MAX_BUFFER_SIZE-sizeof(PACKHEAD))
	{
		memcpy(m_sendOvl.wsabuf.buf+sizeof(PACKHEAD), m_sendOvl.pdata, m_sendOvl.lpacklen);
		m_sendOvl.wsabuf.len = sizeof(PACKHEAD)+m_sendOvl.lpacklen;
	}
	else
	{
		memcpy(m_sendOvl.wsabuf.buf+sizeof(PACKHEAD), m_sendOvl.pdata, MAX_BUFFER_SIZE-sizeof(PACKHEAD));
		m_sendOvl.wsabuf.len = MAX_BUFFER_SIZE;
	}
	DWORD dwTrans,dwFlag,dwErr;
	dwTrans = dwFlag = 0;

	memset(&(m_sendOvl.ovl),0,sizeof(WSAOVERLAPPED));
	m_sendOvl.ovl.hEvent = m_eventArray[1];
	if(WSASend(m_sock,&(m_sendOvl.wsabuf),1,&dwTrans,dwFlag,&(m_sendOvl.ovl),NULL) == SOCKET_ERROR)
	{
		dwErr = WSAGetLastError();
		if(dwErr != WSA_IO_PENDING)
			return false;
	}
	return true;
}

BOOL CAsynSock::releaseBuf(char* buf)
{
	return HeapFree(m_hPackData,0,buf);
}