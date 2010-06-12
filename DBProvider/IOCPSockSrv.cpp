/*
CIOCPSockSrv ��������
author: lincolnfz
lincolnfz@gmail.com
*/

#include "StdAfx.h"
#include "IOCPSockSrv.h"
#include "AdoProvider.h"
#include "../common/suUtil.h"
#include "log4cpp/Category.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/BasicLayout.hh"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

CIOCPSockSrv* pIOCPSockSrv;
volatile LONG nTotal = 0;
unsigned int nMaxTimeOut = 3*60*1000;//�ͻ��˳�ʱ����

volatile long index = 0;
long getIdx()
{
	return (InterlockedIncrement(&index));
}

CIOCPSockSrv::CIOCPSockSrv()
{
	lpfnAcceptEx = NULL;
	m_hIOHeap = m_hDataHeap = m_hPack = m_hunzipPack = NULL;
	m_bstop = true;
	pIOCPSockSrv = this;
	m_nListenPort = 10003;
	m_hCheckThread = NULL;
	m_phSendThread = NULL;
	m_phClose = NULL;
	m_hGuardAcceptExThread = NULL;
	m_nSockThread = 1;
	m_maxWsaSend = 1;
	m_lSend = 0;
	m_flagQueue = WT_EXECUTEDEFAULT;
	WT_SET_MAX_THREADPOOL_THREADS(m_flagQueue,2);
}

CIOCPSockSrv::~CIOCPSockSrv()
{
	/*if(m_bstop == false)
	{
	Stop();
	}*/
}

void CIOCPSockSrv::SetAdoProviderPtr(CAdoProvider* pAdoProvider)
{
	m_pAdoProvider = pAdoProvider;
}

void CIOCPSockSrv::SetListenPort(int nPort)
{
	if(nPort > 0)
		m_nListenPort = nPort;
}

void CIOCPSockSrv::setMaxSend(int nMaxSend)
{
	m_maxWsaSend = nMaxSend;
}

void CALLBACK CIOCPSockSrv::APCProc(ULONG_PTR dwParam)
{

}

void CIOCPSockSrv::Clean()
{
	int i = 0;
	for (i=0; i < m_nSockThread; i++)
	{
		PostQueuedCompletionStatus(m_hIOCP,0,NULL,NULL);
	}

	WaitForMultipleObjects(m_nSockThread+1,m_phClose,TRUE,INFINITE);

	for(i = 0; i < m_nSockThread+1; i++)
	{
		CloseHandle(m_phSendThread[i]);
	}
	delete[] m_phSendThread;
	m_phSendThread = NULL;

	for(i = 0; i < m_nSockThread+1; i++)
	{
		CloseHandle(m_phClose[i]);
	}
	delete[] m_phClose;
	m_phClose = NULL;

	if(m_hIOHeap)
	{
		HeapDestroy(m_hIOHeap);	
		m_hIOHeap = NULL;
	}
	if(m_hDataHeap)
	{
		HeapDestroy(m_hDataHeap);
		m_hDataHeap = NULL;
	}
	if(m_hPack)
	{
		HeapDestroy(m_hPack);
		m_hPack = NULL;
	}
	if(m_hunzipPack)
	{
		HeapDestroy(m_hunzipPack);
		m_hunzipPack = NULL;
	}
	closesocket(m_socklisten);
	m_sockmap.clear();

	WSACleanup();
}

void CIOCPSockSrv::Stop()
{
	Clean();//�˳���ɶ˿ڹ����߳�

	QueueUserAPC(APCProc,m_hGuardAcceptExThread,0);
	CloseHandle(m_hGuardAcceptExThread);

	QueueUserAPC(APCProc,m_hCheckThread,0);
	CloseHandle(m_hCheckThread);

	WaitForMultipleObjects(2,m_hEventClose,TRUE,INFINITE);
	CloseHandle(m_hEventClose[0]);
	CloseHandle(m_hEventClose[1]);
	m_bstop = true;
}

void CIOCPSockSrv::Run(DWORD nkernel)
{
	log4cpp::Category& info_log = log4cpp::Category::getInstance("info");
	unsigned int iID;
	HANDLE hThread;
	sockaddr_in srvAddr;
	SOCKET sockAccept;	//���յ���sock
	sockaddr_in acceptAddr; //���յ���sock ip��ַ
	LPPER_HANDLE_DATA lpPerHandleData;
	LPPER_IO_OPERATION_DATA lpPerIOData;
	DWORD dwflag=0,dwCode;

	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD( 2, 2 );
	err = WSAStartup( wVersionRequested, &wsaData );

#ifdef GET_WSAPROTOCOL
	//demo code get WSAPROTOCOL_INFO
	WSAPROTOCOL_INFO* lpwsaprotoinfo=NULL;
	DWORD len;
	if( WSAEnumProtocols(NULL,lpwsaprotoinfo,&len))
	{
		if(WSAGetLastError() != WSAENOBUFS)
		{
			WSACleanup();
			return;
		}
	}
	lpwsaprotoinfo = (WSAPROTOCOL_INFO*)(new char[len]);
	memset(lpwsaprotoinfo,0,len);
	int ii = WSAEnumProtocols(NULL,lpwsaprotoinfo,&len);
	DWORD ret = lpwsaprotoinfo->dwServiceFlags1 & XP1_QOS_SUPPORTED ;
	delete[] lpwsaprotoinfo;
#endif


	m_hIOHeap = HeapCreate(HEAP_GENERATE_EXCEPTIONS,0,0);
	m_hDataHeap = HeapCreate(HEAP_GENERATE_EXCEPTIONS,0,0);
	m_hPack = HeapCreate(HEAP_GENERATE_EXCEPTIONS,0,0);
	m_hunzipPack = HeapCreate(HEAP_GENERATE_EXCEPTIONS,0,0);

	//����������ɶ˿�
	m_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE,0,0,0);
	m_ThreadContext.hICOP = m_hIOCP;
	m_ThreadContext.hIOHeap = m_hIOHeap;
	m_ThreadContext.hDataHeap = m_hDataHeap;

	m_nSockThread = nkernel*2; //���ù������߳���Ŀ

	if(m_maxWsaSend < m_nSockThread) m_maxWsaSend = m_nSockThread;//�����ص�������Ŀ

	if(m_phSendThread == NULL)
		m_phSendThread = new HANDLE[m_nSockThread+1];
	m_phSendThread[0] = CreateEvent(NULL,FALSE,FALSE,NULL);

	if(m_phClose == NULL)
		m_phClose = new HANDLE[m_nSockThread+1];
	m_phClose[0] = CreateEvent(NULL,TRUE,TRUE,NULL);

	for (DWORD i=0; i < m_nSockThread; i++)
	{
		m_phSendThread[i+1] = CreateEvent(NULL,FALSE,FALSE,NULL);
		m_phClose[i+1] = CreateEvent(NULL,TRUE,FALSE,NULL);
		//m_ThreadContext.indexEvent = i+1;//ÿһ�������̷߳���һ�������¼�
		hThread = (HANDLE)_beginthreadex(NULL,0,CompletionThread,(void*)&m_ThreadContext,0,&iID);
		CloseHandle(hThread);
	}

	m_hEventClose[0] = CreateEvent(NULL,TRUE,FALSE,NULL);
	m_hEventClose[1] = CreateEvent(NULL,TRUE,FALSE,NULL);

	//���������Ͷ����߳�
	m_hCheckThread = (HANDLE)_beginthreadex(NULL,0,CheckPendingThread,this,0,&iID);

	//��������sock

	m_hListenEvent = WSACreateEvent();
	m_socklisten = WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED);
	if(INVALID_SOCKET == m_socklisten)
	{

	}

	srvAddr.sin_family = AF_INET;
	srvAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	srvAddr.sin_port = htons(m_nListenPort);

	BOOL bvalue = FALSE;
	int iret = sizeof(bvalue);
	//����SO_REUSEADDR����
	setsockopt(m_socklisten,SOL_SOCKET,SO_REUSEADDR,(char*)&bvalue,iret);
	if(bind(m_socklisten,(sockaddr*)&srvAddr,sizeof(sockaddr_in)) == SOCKET_ERROR)
	{
		closesocket(m_socklisten);
		return;
	}

	if(listen(m_socklisten,50) == SOCKET_ERROR)
	{
		info_log.error("�޷�����!");
		return;
	}

	GUID GuidAcceptEx = WSAID_ACCEPTEX;
	GUID GuidGetAcceptExSockaddrs = WSAID_GETACCEPTEXSOCKADDRS;
	DWORD dwBytes=0;

	//----------------------------------------
	// Load the AcceptEx function into memory using WSAIoctl.
	// The WSAIoctl function is an extension of the ioctlsocket()
	// function that can use overlapped I/O. The function's 3rd
	// through 6th parameters are input and output buffers where
	// we pass the pointer to our AcceptEx function. This is used
	// so that we can call the AcceptEx function directly, rather
	// than refer to the Mswsock.lib library.
	WSAIoctl(m_socklisten, 
		SIO_GET_EXTENSION_FUNCTION_POINTER, 
		&GuidAcceptEx, 
		sizeof(GuidAcceptEx),
		&lpfnAcceptEx, 
		sizeof(lpfnAcceptEx), 
		&dwBytes, 
		NULL, 
		NULL);

	WSAIoctl(m_socklisten, 
		SIO_GET_EXTENSION_FUNCTION_POINTER, 
		&GuidGetAcceptExSockaddrs, 
		sizeof(GuidGetAcceptExSockaddrs),
		&lpfnGetAcceptExSockaddrs, 
		sizeof(lpfnGetAcceptExSockaddrs), 
		&dwBytes, 
		NULL, 
		NULL);


	/*unsigned long ulCmd;
	ulCmd = 1;
	err = ioctlsocket(m_socklisten,FIONBIO,&ulCmd);*/ //����Ϊ������ģʽ
	WSAEventSelect(m_socklisten,m_hListenEvent,FD_ACCEPT);

	LPPER_HANDLE_DATA lpPerListenData = (LPPER_HANDLE_DATA)HeapAlloc(m_hIOHeap ,HEAP_ZERO_MEMORY,
		sizeof(PER_HANDLE_DATA ));
	lpPerListenData->sock = m_socklisten;
	lpPerListenData->socktype = SERVERSOCK;

	//----------------------------------------
	// Associate the listening socket with the completion port
	CreateIoCompletionPort((HANDLE)m_socklisten, m_hIOCP, (u_long)lpPerListenData, 0);
	for (DWORD i=0; i < m_nSockThread; i++)
	{
		CreatePerHander();
	}

	//����acceptex�����߳�
	m_hGuardAcceptExThread = (HANDLE)_beginthreadex(NULL,0,GuardAcceptEx,this,0,&iID);


	m_bstop = false;
}

//����sock
LPPER_HANDLE_DATA CIOCPSockSrv::CreatePerHander()
{
	log4cpp::Category& info_log = log4cpp::Category::getInstance("info");
	DWORD dwBytes,dwCode;
	DWORD dwAddressLength = sizeof(sockaddr_in) + 16;
	//�������������
	LPPER_HANDLE_DATA lpPerListenData = (LPPER_HANDLE_DATA)HeapAlloc(m_hIOHeap ,HEAP_ZERO_MEMORY,
		sizeof(PER_HANDLE_DATA ));

	lpPerListenData->socktype = LISTENSOCK;
	lpPerListenData->sock = WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED);

	//Ͷ��acceptex����
	dwBytes = 0;
	BOOL bret = lpfnAcceptEx(m_socklisten,lpPerListenData->sock,lpPerListenData->tinybuf,0,
		dwAddressLength, dwAddressLength, &dwBytes, &(lpPerListenData->ovl));

	if(bret == FALSE && (dwCode = WSAGetLastError()) != WSA_IO_PENDING)
	{			
		info_log.error("�޷�acceptex!");
		HeapFree(m_hIOHeap,0,lpPerListenData);
		lpPerListenData = NULL;
	}

	return lpPerListenData;
}

unsigned int CIOCPSockSrv::GuardAcceptEx(LPVOID lpParameter)
{
	log4cpp::Category& info_log = log4cpp::Category::getInstance("info");
	DWORD dwIndex,dwErr;
	WSANETWORKEVENTS netEvents;
	while(true)
	{
		dwIndex = WSAWaitForMultipleEvents(1,&(pIOCPSockSrv->m_hListenEvent),
			FALSE,WSA_INFINITE,TRUE); //�ȴ�m_hListenEvent�¼�

		dwIndex -= WSA_WAIT_EVENT_0;
		if (WAIT_IO_COMPLETION == dwIndex)
		{
			//������,�˳�ѭ��,��������
			break;
		}
		if(WSA_WAIT_FAILED == dwIndex)
		{
			dwErr = WSAGetLastError();
			info_log.error("GuardAcceptEx ������������(�˳�):%s",CsuUtil::GetLastErrorMessageString(dwErr));
			break;
		}
		memset(&netEvents,0,sizeof(netEvents));
		WSAEnumNetworkEvents(pIOCPSockSrv->m_socklisten,pIOCPSockSrv->m_hListenEvent,
			&netEvents); //��ô������¼�

		if( (netEvents.lNetworkEvents & FD_ACCEPT) &&
			(netEvents.iErrorCode[FD_ACCEPT_BIT]==0) )
		{
			for (DWORD i=0; i < pIOCPSockSrv->m_nSockThread; i++)
			{
				pIOCPSockSrv->CreatePerHander();
			}
		}
	}
	SetEvent(pIOCPSockSrv->m_hEventClose[0]);
	return 0;
}

//�ͷ�sock
void CIOCPSockSrv::ReleaseSock(SOCKET sock,int threadindex, bool binitiative)
{
	m_mutexSockmap.lock();
	bool bNeedNotifyNextPack=false;
	SOCKMAP::iterator itsockmap;
	itsockmap = m_sockmap.find(sock);
	if(itsockmap != m_sockmap.end())
	{	
		//Ϊ�����ͷŻ��ڹ����߳��е�sock,�ӳ��ͷ���Դ
		itsockmap->second.adodata.bNeedClose = true;
		if(itsockmap->second.adodata.bSendPending ||
			itsockmap->second.adodata.netSendstate == NETBUSY)
		{
			//��Ϊsock����pending״̬,���յ������쳣��Ϣ
			itsockmap->second.adodata.netSendstate = NETIDLE;
			bNeedNotifyNextPack = true;
		}
		if(itsockmap->second.adodata.bInSendWorkThread)
		{
			m_mutexSockmap.unlock();
			return;
		}
		if(itsockmap->second.adodata.netRecvstate == NETBUSY)
		{
			m_mutexSockmap.unlock();
			return;
		}
		if(itsockmap->second.adodata.bCheckPending == true)
		{
			m_mutexSockmap.unlock();
			return;
		}
		if(itsockmap->second.adodata.Senddatapack)
		{
			delete[] itsockmap->second.adodata.Senddatapack;
			itsockmap->second.adodata.Senddatapack = NULL;
		}
		if (itsockmap->second.adodata.Recvdatapack)
		{
			ReleaseDataPack(itsockmap->second.adodata.Recvdatapack);
			itsockmap->second.adodata.Recvdatapack = NULL;
		}
		if(itsockmap->second.adodata.hAlive)
			CloseHandle(itsockmap->second.adodata.hAlive);

		if(itsockmap->second.adodata.hTimeOut)
			UnregisterWaitEx(itsockmap->second.adodata.hTimeOut,NULL);

		HeapFree(m_hIOHeap,0,itsockmap->second.predata);
		HeapFree(m_hDataHeap,0,itsockmap->second.ioRecvData);
		HeapFree(m_hDataHeap,0,itsockmap->second.ioSendData);
		m_sockmap.erase(itsockmap);
	}
	m_mutexSockmap.unlock();

	if (bNeedNotifyNextPack)
	{
		InterlockedDecrement(&m_lSend);
		//֪ͨ�¸����ݰ����Է���
		if (pIOCPSockSrv->m_pendingPackList.empty() == FALSE)
		{
			SetEvent(pIOCPSockSrv->m_phSendThread[0]);
		}
	}

	log4cpp::Category& info_log = log4cpp::Category::getInstance("info");
	info_log.info("�ɹ��ͷ�socket:%d",sock);

	return;
}

//��ɶ˿ڹ����߳�
unsigned int CIOCPSockSrv::CompletionThread(LPVOID lpParameter)
{
	log4cpp::Category& info_log = log4cpp::Category::getInstance("info");
	DWORD dwThreadID = GetCurrentThreadId();

	LPTHREADCONTEXT pThreadContext = (LPTHREADCONTEXT)lpParameter;
	HANDLE hIOCP = pThreadContext->hICOP;
	HANDLE hIOHeap = pThreadContext->hIOHeap;
	HANDLE hDataHeap = pThreadContext->hDataHeap;
	int indexEvent = (int)getIdx();//pThreadContext->indexEvent;
	DWORD dwflag=0,dwByteTranslate=0,dwRet,dwTrans,dwErr;
	LPPER_HANDLE_DATA lpPerHandleData;
	LPPER_IO_OPERATION_DATA lpPerIOData;
	WSAOVERLAPPED* pWsaOvl;
	long dwRecv=0;
	SOCKMAP::iterator itsockmap;
	//ADOCLIENTMAP::iterator itadoclient;
	//pair<ADOCLIENTMAP::iterator,bool> insval;
	int err = 0;
	//long lPackLen=0;
	//DWORD dwID;
	PACKHEAD packhead;
	DWORD dwAddressLength = sizeof(sockaddr_in) + 16;

	while (TRUE)
	{
		pWsaOvl = NULL;
		lpPerHandleData = NULL;
		lpPerIOData = NULL;
		if(FALSE == GetQueuedCompletionStatus(hIOCP,&dwByteTranslate,(PULONG_PTR)&lpPerHandleData,
			(LPWSAOVERLAPPED*)&pWsaOvl,INFINITE))
		{
			dwRet = WSAGetLastError();
			if (WAIT_TIMEOUT == dwRet || ERROR_NETNAME_DELETED == dwRet)
			{
				info_log.warn("CompletionThread ��������:%d,%s",lpPerHandleData->sock, CsuUtil::GetLastErrorMessageString(dwRet));
				pIOCPSockSrv->ReleaseSock(lpPerHandleData->sock,indexEvent,true);
				continue;
			} 
			else if(WSA_OPERATION_ABORTED == dwRet)
			{
				info_log.warn("CompletionThread ��������:%d,%s",lpPerHandleData->sock, CsuUtil::GetLastErrorMessageString(dwRet));
				pIOCPSockSrv->ReleaseSock(lpPerHandleData->sock,indexEvent,true);
				continue;
			}
			else
			{
				info_log.warn("CompletionThread �����˳�:%d,%s",lpPerHandleData->sock, CsuUtil::GetLastErrorMessageString(dwRet));
				break;
			}
		}

		if(lpPerHandleData == NULL)
		{
			info_log.info("lpPerHandleData is null �����߳��˳�");
			break;
		}

		//������sock
		if(lpPerHandleData->socktype == SERVERSOCK)
		{
			LPPER_HANDLE_DATA lpPerListenData = CONTAINING_RECORD(pWsaOvl,PER_HANDLE_DATA,ovl);
			LPPER_IO_OPERATION_DATA lpRecvPerIOData,lpSendPerIOData;
			lpRecvPerIOData = lpSendPerIOData = NULL;
			//˵�����յ�һ���µ�accetpex����
			try
			{
				setsockopt(lpPerListenData->sock, 
					SOL_SOCKET, 
					SO_UPDATE_ACCEPT_CONTEXT, 
					(char *)&(lpPerHandleData->sock),
					sizeof(lpPerHandleData->sock) );

				/*int len,size;
				size = sizeof(len);
				getsockopt(lpPerListenData->sock,SOL_SOCKET,SO_SNDBUF,
				(char*)&len,&size);*/

				sockaddr *localAddr,*remoteAddr;
				int nlAddrlen,nrAddrlen;
				//���͵�ַ
				pIOCPSockSrv->lpfnGetAcceptExSockaddrs(lpPerListenData->tinybuf,0,
					dwAddressLength,dwAddressLength,&localAddr,&nlAddrlen,
					&remoteAddr,&nrAddrlen);

				memcpy(&(lpPerListenData->addr),remoteAddr,sizeof(lpPerListenData->addr));
				//�õ��Է���ַ(127.0.0.1)
				//char* ip = inet_ntoa (lpListen->addr.sin_addr);


				//������IO��������
				lpRecvPerIOData = (LPPER_IO_OPERATION_DATA)HeapAlloc(pIOCPSockSrv->m_hDataHeap,
					HEAP_ZERO_MEMORY,sizeof(PER_IO_OPERATION_DATA));
				lpRecvPerIOData->wsabuf.buf = lpRecvPerIOData->databuf;
				lpRecvPerIOData->wsabuf.len = 0;
				lpRecvPerIOData->operation = ZERORECV;

				//������IO��������
				lpSendPerIOData = (LPPER_IO_OPERATION_DATA)HeapAlloc(pIOCPSockSrv->m_hDataHeap,
					HEAP_ZERO_MEMORY,sizeof(PER_IO_OPERATION_DATA));
				lpSendPerIOData->wsabuf.buf = lpSendPerIOData->databuf;
				lpSendPerIOData->wsabuf.len = 0;
			}
			catch(...)
			{
				info_log.error("listenThread ����sock��Դ����");
				//�ѿռ�����쳣
				if(lpPerListenData)
					HeapFree(pIOCPSockSrv->m_hIOHeap,0,lpPerListenData);
				if(lpRecvPerIOData)
					HeapFree(pIOCPSockSrv->m_hDataHeap,0,lpRecvPerIOData);
				if(lpSendPerIOData)
					HeapFree(pIOCPSockSrv->m_hDataHeap,0,lpSendPerIOData);
				continue;
			}

			//����sock map
			SOCK_INFO sockinfo;
			memset(&sockinfo,0,sizeof(SOCK_INFO));
			sockinfo.predata = lpPerListenData;
			sockinfo.ioRecvData = lpRecvPerIOData;
			sockinfo.ioSendData = lpSendPerIOData;
			sockinfo.adodata.hAlive = CreateEvent(NULL,FALSE,FALSE,NULL);

			//Ϊÿ��sock���ö�ʱ��
			if(RegisterWaitForSingleObject(&sockinfo.adodata.hTimeOut ,
				sockinfo.adodata.hAlive, TimeOutCallback,(PVOID)lpPerListenData->sock,
				nMaxTimeOut,pIOCPSockSrv->m_flagQueue)==FALSE)
			{
				dwErr = GetLastError();
				HeapFree(pIOCPSockSrv->m_hIOHeap,0,sockinfo.predata);
				HeapFree(pIOCPSockSrv->m_hDataHeap,0,sockinfo.ioRecvData);
				HeapFree(pIOCPSockSrv->m_hDataHeap,0,sockinfo.ioSendData);
				CloseHandle(sockinfo.adodata.hAlive);
				info_log.error("listenThread ע�ᳬʱ�¼�ʧ��:%s",CsuUtil::GetLastErrorMessageString(dwErr));
				continue;
			}
			//���ö�ʱ������

			pIOCPSockSrv->m_mutexSockmap.lock();
			pIOCPSockSrv->m_sockmap.insert(SOCKMAP::value_type(lpPerListenData->sock,sockinfo));
			pIOCPSockSrv->m_mutexSockmap.unlock();

			//���½��ܵ�socket����ɶ˿���ϵ��һ��
			if(CreateIoCompletionPort((HANDLE)(lpPerListenData->sock),pIOCPSockSrv->m_hIOCP,
				(u_long)lpPerListenData,0) == NULL)
			{
				dwErr = GetLastError();
				info_log.error("listenThread ����ɶ˿ڴ���:%s",CsuUtil::GetLastErrorMessageString(dwErr));
				pIOCPSockSrv->ReleaseSock(lpPerListenData->sock);
				continue;
			}

			dwflag = dwTrans = 0;
			ZeroMemory(&(lpRecvPerIOData->ovl),sizeof(WSAOVERLAPPED));
			dwErr = WSARecv(lpPerListenData->sock,&(lpRecvPerIOData->wsabuf),1,&dwTrans,
				&dwflag,&(lpRecvPerIOData->ovl),NULL);

			if (dwErr == SOCKET_ERROR)
			{
				dwErr = WSAGetLastError();
				if(dwErr != WSA_IO_PENDING)
				{
					info_log.error("listenThread ����sock��������:%s",CsuUtil::GetLastErrorMessageString(dwErr));
					//�ر�һ��sock
					pIOCPSockSrv->ReleaseSock(lpPerListenData->sock);
					continue;
				}
			}
			InterlockedIncrement(&nTotal);
			info_log.info("listenThread �ɹ�����sock:%d,����:(%d)��",lpPerListenData->sock,nTotal);

			continue;
		}//������sock

		//�ͻ���sock
		lpPerIOData = CONTAINING_RECORD(pWsaOvl,PER_IO_OPERATION_DATA,ovl);
		//1.��sock��æµ���
		pIOCPSockSrv->m_mutexSockmap.lock();
		itsockmap = pIOCPSockSrv->m_sockmap.find(lpPerHandleData->sock);
		//�������ݿ�ʼ
		if(itsockmap == pIOCPSockSrv->m_sockmap.end())
		{
			pIOCPSockSrv->m_mutexSockmap.unlock();
			continue; //����sock map�оͺ���
		}

		if (0 == dwByteTranslate && (itsockmap->second.adodata.bClose || 
			itsockmap->second.adodata.bNeedClose))
		{
			//�ر�һ��sock
			itsockmap->second.adodata.bSendPending = false;
			pIOCPSockSrv->m_mutexSockmap.unlock();
			pIOCPSockSrv->ReleaseSock(lpPerHandleData->sock,indexEvent,true);
			continue;
		}

		if(lpPerIOData->operation == ZERORECV)
		{
			itsockmap->second.adodata.netRecvstate = NETBUSY;
		}
		else if(lpPerIOData->operation == SEND)
		{
			itsockmap->second.adodata.bSendPending = false;
			itsockmap->second.adodata.bInSendWorkThread = true;
		}
		pIOCPSockSrv->m_mutexSockmap.unlock();

		bool bNetError = false;

		//����mutexsock��,��ʼ��������뷢�Ͳ���
		if(lpPerIOData->operation == ZERORECV)
		{			
			//�����յ��������¼�(��ʱ��)
			SetEvent(itsockmap->second.adodata.hAlive);
			long dwProcRemain = 0;
			lpPerIOData->wsabuf.len = MAX_BUFFER_SIZE;
			do{
				//ʹ�÷�������������sock�е�����ֱ���� WSAEWOULDBLOCK ����˵���Ѷ�ȡ������sock����
				dwflag = dwTrans = 0;
				err = WSARecv(lpPerHandleData->sock,&(lpPerIOData->wsabuf),1,&dwTrans,&dwflag,
					NULL,NULL);
				if(0 == err)
				{
					dwProcRemain = dwTrans;
				}
				else if(SOCKET_ERROR == err && WSAEWOULDBLOCK == WSAGetLastError())
				{
					//˵��sock���ջ�����û��������
					break;
				}
				else
				{
					//�������
					bNetError = true;				
				}

				//��Ϊ������ִ����shutdown,����dwProcRemainΪ0
				if(dwProcRemain == 0) 
					bNetError = true;

				if(bNetError)
				{
					//pIOCPSockSrv->m_mutexSockmap.lock();
					itsockmap->second.adodata.netRecvstate = NETIDLE;//���ý���Ϊ����״̬
					//pIOCPSockSrv->m_mutexSockmap.unlock();

					if(itsockmap->second.adodata.bNeedClose || (dwProcRemain == 0))
					{
						//fix meò�ƽ����������bcloseһ��Ϊtrue
						//if(dwProcRemain == 0)
						//	itsockmap->second.adodata.bClose = true;

						pIOCPSockSrv->ReleaseSock(lpPerHandleData->sock);
					}
					else
					{
						itsockmap->second.adodata.bClose = true;
						shutdown(lpPerHandleData->sock,SD_SEND);
						//closesocket(lpPerHandleData->sock);
					}
					break;
				}

				char* pvist = lpPerIOData->databuf;				
				while (dwProcRemain > 0)
				{

					if(itsockmap->second.adodata.dwRecvRemain == 0)
					{
						if(dwProcRemain < sizeof(PACKHEAD))
						{
							//�յ�һ����Ч�ͻ��˵�����
							info_log.warn("�ܾ�������Ч�ͻ�������:%d",lpPerHandleData->sock);
							bNetError = true; //����Щ�ͻ���Ϊ�쳣
							break;
						}

						//�յ�һ��ȫ�µ�����
						memcpy(&packhead,pvist,sizeof(PACKHEAD));
						pvist += sizeof(PACKHEAD);
						if(packhead.lpacklen <= 0)
							break;

						//1.�����յ��µİ�����
						itsockmap->second.adodata.dwID = packhead.dwid;
						itsockmap->second.adodata.dwRecvPacklen = packhead.lpacklen;
						//2.�����һ�����°��ĳ���
						itsockmap->second.adodata.dwRecvRemain = dwProcRemain - sizeof(PACKHEAD) - itsockmap->second.adodata.dwRecvPacklen;
						if(itsockmap->second.adodata.dwRecvRemain < 0)
							itsockmap->second.adodata.dwRecvRemain = 0 - itsockmap->second.adodata.dwRecvRemain ;
						else
							itsockmap->second.adodata.dwRecvRemain = 0;

						//3.�����һ���յİ��ĳ���,ע�����°�����,��һ���յ�
						dwRecv = itsockmap->second.adodata.dwRecvPacklen - itsockmap->second.adodata.dwRecvRemain;

						//���������ݴ洢�ռ�
						itsockmap->second.adodata.Recvdatapack = (char*)HeapAlloc(pIOCPSockSrv->m_hPack, HEAP_ZERO_MEMORY,
							itsockmap->second.adodata.dwRecvPacklen+1);
						//����buffer�����������ݴ洢�ռ�
						itsockmap->second.adodata.Recvpos = itsockmap->second.adodata.Recvdatapack;
						memcpy(itsockmap->second.adodata.Recvpos,pvist,dwRecv);
						pvist += dwRecv;
						itsockmap->second.adodata.Recvpos += dwRecv;

						dwProcRemain = dwProcRemain - sizeof(PACKHEAD) - dwRecv;

					}
					else
					{
						//���ղ���İ�				
						//���㱾���յ������ֽ�,ע�����ǲ����
						dwRecv = itsockmap->second.adodata.dwRecvRemain - dwProcRemain;
						if(dwRecv <= 0)
							dwRecv = itsockmap->second.adodata.dwRecvRemain;
						else
							dwRecv = dwProcRemain;
						//���㻹�ж����ֽ�δ����
						itsockmap->second.adodata.dwRecvRemain -= dwRecv;
						if(itsockmap->second.adodata.dwRecvRemain < 0)
							itsockmap->second.adodata.dwRecvRemain = 0;

						memcpy(itsockmap->second.adodata.Recvpos,pvist,dwRecv);
						pvist += dwRecv;
						itsockmap->second.adodata.Recvpos += dwRecv;						
						dwProcRemain = dwProcRemain - dwRecv;
					}					

					//�ж��Ƿ���Խ����յ������ύӦ�ò㴦��
					if(itsockmap->second.adodata.dwRecvRemain == 0)
					{
						info_log.info("�ɹ��յ���������:%d",lpPerHandleData->sock);
						//����һ�������İ�
						char* pdata = itsockmap->second.adodata.Recvdatapack;
						itsockmap->second.adodata.Recvdatapack = NULL;
						pIOCPSockSrv->m_pAdoProvider->NotifyRecvPack(lpPerHandleData->sock,itsockmap->second.adodata.dwID,
							pdata, itsockmap->second.adodata.dwRecvPacklen);						
					}

				}//(dwProcRemain > 0) һ�����ݰ��������
				
				if(bNetError)
				{
					break;
				}

			}while(true);

			//�����Ա������ݴ������

			if(bNetError)
			{
				//��⵽���緢���쳣
				continue;
			}

			//���½���socket����
			dwflag = dwTrans = 0;
			ZeroMemory(&(lpPerIOData->ovl),sizeof(WSAOVERLAPPED));
			lpPerIOData->wsabuf.len = 0;

			err = WSARecv(lpPerHandleData->sock,&(lpPerIOData->wsabuf),1,&dwTrans,
				&dwflag,&(lpPerIOData->ovl),NULL);
			if(err == SOCKET_ERROR)
			{
				dwErr = WSAGetLastError();
			}
			if (err == SOCKET_ERROR && dwErr != WSA_IO_PENDING)
			{
				//pIOCPSockSrv->m_mutexSockmap.lock();
				itsockmap->second.adodata.netRecvstate = NETIDLE;//���ý���Ϊ����״̬
				//pIOCPSockSrv->m_mutexSockmap.unlock();
				//�ر�һ��sock
				if(itsockmap->second.adodata.bNeedClose)
					pIOCPSockSrv->ReleaseSock(lpPerHandleData->sock);
				else
				{
					itsockmap->second.adodata.bClose = true;
					shutdown(lpPerHandleData->sock,SD_SEND);					
					//closesocket(lpPerHandleData->sock);
				}
				continue;
			}
			//pIOCPSockSrv->m_mutexSockmap.lock();
			itsockmap->second.adodata.netRecvstate = NETIDLE;//���ý���Ϊ����״̬
			//pIOCPSockSrv->m_mutexSockmap.unlock();

		}//recv ����
		else if(lpPerIOData->operation == SEND)
		{	
			bool bSendComplate = false; //��ʶ�Ƿ������
			dwflag = 0;
			if(itsockmap->second.adodata.nSendTimes == 0)
			{
				//��һ�η������ݰ�
				itsockmap->second.adodata.nSendTimes++;
				itsockmap->second.adodata.dwSendTrans += (dwByteTranslate-sizeof(PACKHEAD));
			}
			else
			{
				//�ǵ�һ�η������ݰ�
				itsockmap->second.adodata.nSendTimes++;
				itsockmap->second.adodata.dwSendTrans += dwByteTranslate;
			}
			itsockmap->second.adodata.dwSendRemain = itsockmap->second.adodata.dwSendPacklen - itsockmap->second.adodata.dwSendTrans;
			if(itsockmap->second.adodata.dwSendRemain > 0)
			{
				//����δ������İ�
				if(itsockmap->second.adodata.dwSendRemain <= MAX_BUFFER_SIZE)
				{
					memcpy(lpPerIOData->databuf,
						itsockmap->second.adodata.Senddatapack + itsockmap->second.adodata.dwSendTrans,
						itsockmap->second.adodata.dwSendRemain);
					lpPerIOData->wsabuf.len = itsockmap->second.adodata.dwSendRemain;
				}
				else
				{
					memcpy(lpPerIOData->databuf,
						itsockmap->second.adodata.Senddatapack + itsockmap->second.adodata.dwSendTrans,
						MAX_BUFFER_SIZE);
					lpPerIOData->wsabuf.len = MAX_BUFFER_SIZE;
				}

				//��������ʣ�µĲ���
				dwTrans=0;
				ZeroMemory(&(lpPerIOData->ovl),sizeof(WSAOVERLAPPED));
				int nRet = WSASend(lpPerHandleData->sock, &(lpPerIOData->wsabuf), 1, &dwTrans,
					dwflag,&(lpPerIOData->ovl), NULL);
				if(nRet == SOCKET_ERROR)
				{
					dwErr = WSAGetLastError();
				}
				if((nRet == SOCKET_ERROR) && (dwErr != WSA_IO_PENDING))
				{

					//���ͳ��ִ���
					//�ر�һ��sock
					//fix it
					delete[] itsockmap->second.adodata.Senddatapack;
					itsockmap->second.adodata.Senddatapack = NULL;
					bNetError = true;
				}
				else if((nRet == SOCKET_ERROR) && (dwErr == WSA_IO_PENDING))
				{
					//���sock��pending״̬
					itsockmap->second.adodata.bSendPending = true;

					info_log.info("���ͽ���pending״̬:%d,��%d��",lpPerHandleData->sock,
						itsockmap->second.adodata.nSendTimes+1);
				}
			}
			else
			{

				//���������
				bSendComplate = true;
				delete[] itsockmap->second.adodata.Senddatapack;
				itsockmap->second.adodata.Senddatapack = NULL;
				itsockmap->second.adodata.netSendstate = NETIDLE;

				info_log.info("�������:%d,��%d��",lpPerHandleData->sock,
					itsockmap->second.adodata.nSendTimes);

			}
			//pIOCPSockSrv->m_mutexSockmap.lock();
			itsockmap->second.adodata.bInSendWorkThread = false;
			//pIOCPSockSrv->m_mutexSockmap.unlock();

			if(itsockmap->second.adodata.bNeedClose || bNetError)
			{
				pIOCPSockSrv->ReleaseSock(lpPerHandleData->sock,indexEvent);
				continue;
			}

			//������ɻ����緢������
			if(bSendComplate /*|| bNetError*/)
			{
				InterlockedDecrement(&(pIOCPSockSrv->m_lSend));
				//֪ͨ�¸����ݰ����Է���
				if (pIOCPSockSrv->m_pendingPackList.empty() == FALSE)
				{
					SetEvent(pIOCPSockSrv->m_phSendThread[indexEvent]);
				}
			}

		}//send����

	}
	info_log.info("CompletionThread�˳�");
	SetEvent(pIOCPSockSrv->m_phClose[indexEvent]);
	//�߳̽���
	return 0;
}

//����pack,��ʱ������
bool CIOCPSockSrv::SendPack(SOCKET sock,DWORD dwId,char* pack,long len)
{
	log4cpp::Category& info_log = log4cpp::Category::getInstance("info");
	SOCKMAP::iterator it;
	//ADOCLIENTMAP::iterator itadoclient;
	//pair<ADOCLIENTMAP::iterator,bool> insval;
	DWORD dwflag = 0;
	PACKHEAD packhead;
	//m_mutexSockmap.lock();
	it = m_sockmap.find(sock);
	//m_mutexSockmap.unlock();

	if(it == m_sockmap.end())
		return false;

	//���Ͳ���
	it->second.ioSendData->operation = SEND;
	ZeroMemory(&(it->second.ioSendData->ovl),sizeof(WSAOVERLAPPED));

	it->second.adodata.Senddatapack = pack;
	it->second.adodata.dwSendPacklen = len; //���ݳ���
	it->second.adodata.dwSendTrans = 0;
	it->second.adodata.nSendTimes = 0;
	it->second.adodata.dwSendRemain = it->second.adodata.dwSendPacklen;

	packhead.dwid = dwId;
	packhead.lpacklen = it->second.adodata.dwSendPacklen;
	memcpy(it->second.ioSendData->databuf,&packhead,sizeof(PACKHEAD));
	if(len <= MAX_BUFFER_SIZE-sizeof(PACKHEAD))
	{
		memcpy(it->second.ioSendData->databuf+sizeof(PACKHEAD),pack,len);
		it->second.ioSendData->wsabuf.len = it->second.adodata.dwSendPacklen+sizeof(PACKHEAD);
	}
	else
	{
		memcpy(it->second.ioSendData->databuf+sizeof(PACKHEAD),pack,MAX_BUFFER_SIZE-sizeof(PACKHEAD));
		it->second.ioSendData->wsabuf.len = MAX_BUFFER_SIZE;
	}

	//���ӷ���һ��wsasendͶ��
	InterlockedIncrement(&(m_lSend));

	it->second.adodata.bSendPending = false;
	DWORD dwTrans = 0;
	int nRet = WSASend(sock, &(it->second.ioSendData->wsabuf), 1, &dwTrans,
		dwflag,&(it->second.ioSendData->ovl), NULL);

	DWORD dwErr=0;
	if(nRet == SOCKET_ERROR)
	{
		dwErr = WSAGetLastError();
	}

	//fix me ���ִ�������ɶ˿����ͷ���Դ
	if((nRet == SOCKET_ERROR) && (dwErr != WSA_IO_PENDING))
	{
		//����һ��wsasendͶ��
		InterlockedDecrement(&m_lSend);
		it->second.adodata.Senddatapack = NULL;
		it->second.adodata.dwSendPacklen = 0;
		//���ͳ��ִ���
		//�ر�һ��sock
		it->second.adodata.bClose = true;
		shutdown(sock,SD_SEND);		
		//closesocket(sock);
		return false;
	}
	else if((nRet == SOCKET_ERROR) && (dwErr == WSA_IO_PENDING))
	{
		//��ǿͻ���sock��pending״̬
		//m_mutexSockmap.lock();
		it->second.adodata.bSendPending = true;
		SetKeepAlive(sock,KEEPTIME,KEEPINTERVAL);
		info_log.warn("���ͽ���pending״̬:%d",sock);
		//m_mutexSockmap.unlock();
	}

	return true;
}

BOOL CIOCPSockSrv::ReleaseDataPack(char* pack)
{
	return HeapFree(m_hPack,0,pack);
}

//IO_FAIL ���ݰ���Զ��������
//IO_PENDING ���ݰ���������Ͷ���
eIOSTATE CIOCPSockSrv::SendAdo(SOCKET sock,DWORD dwId,char* pack,long len,BOOL bAdd)
{
	eIOSTATE eRet = IO_FAIL;
	SOCKMAP::iterator it;
	m_mutexSockmap.lock();
	it = m_sockmap.find(sock);

	//�ж�Ҫ���͵�Ŀ��sock�Ƿ��Ѿ�ʧЧ��
	if(it == m_sockmap.end())
	{
		delete[] pack;
		m_mutexSockmap.unlock();
		return eRet;
	}
	m_mutexSockmap.unlock();

	PENDINGPACK* pendingpack = new PENDINGPACK;
	pendingpack->sock = sock;
	pendingpack->dwID = dwId;
	pendingpack->packlen = len;
	pendingpack->pack = pack; //��ֵҪ���͵�����
	m_mutexCheck.lock();
	m_pendingPackList.push_back(pendingpack);
	m_mutexCheck.unlock();
	eRet = IO_PENDING;
	SetEvent(m_phSendThread[0]);
	return eRet;
}

//������ͷ����Ԫ��
bool CIOCPSockSrv::CheckPendingList()
{
	PENDINGPACKLIST::iterator it,itdel;
	PENDINGPACK* pendinginfo;
	eNETSTATE enetstate;
	bool bsend=false;
	for (it = m_pendingPackList.begin(); it != m_pendingPackList.end(); )
	{
		pendinginfo = *it;
		SOCKMAP::iterator itsockmap;

		//1.�ж�Ҫ�������ݵĿͻ����Ƿ���Ч
		m_mutexSockmap.lock();
		itsockmap = m_sockmap.find(pendinginfo->sock);
		if (itsockmap == m_sockmap.end())
		{
			itdel = it;
			it++;
			m_mutexCheck.lock();
			m_pendingPackList.erase(itdel);
			m_mutexCheck.unlock();
			delete[] pendinginfo->pack;
			delete pendinginfo;
			m_mutexSockmap.unlock();
			continue;
		}
		enetstate = itsockmap->second.adodata.netSendstate;
		itsockmap->second.adodata.bCheckPending = true;
		m_mutexSockmap.unlock();

		//2.�ж��Ƿ������ͻ��˷�������
		if(enetstate == NETIDLE)
		{
			//���͵�ǰ���ݰ�
			itsockmap->second.adodata.netSendstate = NETBUSY;
			m_mutexSockmap.lock();
			bsend = SendPack(pendinginfo->sock,pendinginfo->dwID,
				pendinginfo->pack,pendinginfo->packlen);
			m_mutexSockmap.unlock();
			itdel = it;
			it++;
			m_mutexCheck.lock();
			m_pendingPackList.erase(itdel);
			m_mutexCheck.unlock();
			if(bsend == false)
			{
				itsockmap->second.adodata.netSendstate = NETIDLE;
				itsockmap->second.adodata.bCheckPending = false;
				if (itsockmap->second.adodata.bNeedClose)
				{
					ReleaseSock(pendinginfo->sock);
				}
				delete[] pendinginfo->pack;
				delete pendinginfo;
				continue;				
			}
			else
			{
				//��ͻ��˳ɹ�Ͷ������
				itsockmap->second.adodata.bCheckPending = false;
				if (itsockmap->second.adodata.bNeedClose)
				{
					ReleaseSock(pendinginfo->sock);
				}
				delete pendinginfo;
				break;
			}
		}
		else
		{
			itsockmap->second.adodata.bCheckPending = false;
			if (itsockmap->second.adodata.bNeedClose)
			{
				ReleaseSock(pendinginfo->sock);
			}
			//��ǰҪ���͵Ŀͻ������ڷ���,������ͬʱ����,ȡ�¸����ݰ�
			it++;
			continue;
		}
	}

	return bsend;
}

//��黹δ���Ͷ����߳�
unsigned int CIOCPSockSrv::CheckPendingThread(LPVOID lpParameter)
{
	DWORD dwNum = pIOCPSockSrv->m_nSockThread + 1;
	while(TRUE)
	{
		DWORD dwRet = WaitForMultipleObjectsEx(dwNum,pIOCPSockSrv->m_phSendThread,FALSE,INFINITE,TRUE);
		DWORD dwIndex = dwRet - WAIT_OBJECT_0;
		if(dwRet == WAIT_IO_COMPLETION)
		{
			break;
		}
		else if (dwRet == 0)
		{
			ResetEvent(pIOCPSockSrv->m_phSendThread[dwRet]);
			if(pIOCPSockSrv->m_lSend < pIOCPSockSrv->m_maxWsaSend)
			{
				pIOCPSockSrv->CheckPendingList();
			}
			//ResetEvent(pIOCPSockSrv->m_phSendThread[dwRet]);
		}
		else if(dwRet < dwNum)
		{
			ResetEvent(pIOCPSockSrv->m_phSendThread[dwRet]);
			pIOCPSockSrv->CheckPendingList();
			//ResetEvent(pIOCPSockSrv->m_phSendThread[dwRet]);
		}
		else
		{
			break;
		}
	}
	SetEvent(pIOCPSockSrv->m_hEventClose[1]);
	return 0;
}

void CIOCPSockSrv::TimeOutCallback(PVOID lpParameter,BOOLEAN TimerOrWaitFired)
{
	SOCKET sock = (SOCKET)lpParameter;
	SOCKMAP::iterator it;
	if(TimerOrWaitFired)
	{
		//��ʱ
		pIOCPSockSrv->m_mutexSockmap.lock();
		it = pIOCPSockSrv->m_sockmap.find(sock);
		if(it != pIOCPSockSrv->m_sockmap.end())
		{
			UnregisterWaitEx(it->second.adodata.hTimeOut,NULL);
			it->second.adodata.hTimeOut = NULL;
			CloseHandle(it->second.adodata.hAlive);
			it->second.adodata.hAlive = NULL;
			if(it->second.adodata.bNeedClose)
			{
				pIOCPSockSrv->ReleaseSock(sock);
			}
			else
			{
				it->second.adodata.bClose = true;
				shutdown(sock,SD_SEND);
			}
			//closesocket(sock);
		}
		pIOCPSockSrv->m_mutexSockmap.unlock();
	}
}

void CIOCPSockSrv::SetKeepAlive(SOCKET sock, u_long keepalivetime, 
								u_long keepaliveinterval, bool bOff)
{
	_TCPKEEPALIVE inKeepAlive,outKeepAlive = {0};
	unsigned long ulInLen = sizeof(_TCPKEEPALIVE);
	DWORD dwReturn;

	inKeepAlive.onoff = (bOff ? 0 : 1);
	inKeepAlive.keepalivetime = keepalivetime;
	inKeepAlive.keepaliveinterval = keepaliveinterval;

	DWORD dwErr;
	if(WSAIoctl(sock, SIO_KEEPALIVE_VALS, (LPVOID)&inKeepAlive, ulInLen, 
		(LPVOID)&outKeepAlive, ulInLen, &dwReturn, NULL, NULL) == SOCKET_ERROR)
	{
		dwErr = WSAGetLastError();
	}
}

BOOL CIOCPSockSrv::ExistSock(SOCKET sock)
{
	SOCKMAP::iterator it;
	it = m_sockmap.find(sock);

	return((it != m_sockmap.end()) ? TRUE : FALSE );
}