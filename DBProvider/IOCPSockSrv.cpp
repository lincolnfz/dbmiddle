/*
CIOCPSockSrv 服务器端
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
unsigned int nMaxTimeOut = 3*60*1000;//客户端超时设置

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
	Clean();//退出完成端口工作线程

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
	SOCKET sockAccept;	//接收到的sock
	sockaddr_in acceptAddr; //接收到的sock ip地址
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

	//创建监听完成端口
	m_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE,0,0,0);
	m_ThreadContext.hICOP = m_hIOCP;
	m_ThreadContext.hIOHeap = m_hIOHeap;
	m_ThreadContext.hDataHeap = m_hDataHeap;

	m_nSockThread = nkernel*2; //设置工作者线程数目

	if(m_maxWsaSend < m_nSockThread) m_maxWsaSend = m_nSockThread;//设置重叠发送数目

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
		//m_ThreadContext.indexEvent = i+1;//每一个工作线程分配一个发送事件
		hThread = (HANDLE)_beginthreadex(NULL,0,CompletionThread,(void*)&m_ThreadContext,0,&iID);
		CloseHandle(hThread);
	}

	m_hEventClose[0] = CreateEvent(NULL,TRUE,FALSE,NULL);
	m_hEventClose[1] = CreateEvent(NULL,TRUE,FALSE,NULL);

	//创建待发送队列线程
	m_hCheckThread = (HANDLE)_beginthreadex(NULL,0,CheckPendingThread,this,0,&iID);

	//创建监听sock

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
	//设置SO_REUSEADDR属性
	setsockopt(m_socklisten,SOL_SOCKET,SO_REUSEADDR,(char*)&bvalue,iret);
	if(bind(m_socklisten,(sockaddr*)&srvAddr,sizeof(sockaddr_in)) == SOCKET_ERROR)
	{
		closesocket(m_socklisten);
		return;
	}

	if(listen(m_socklisten,50) == SOCKET_ERROR)
	{
		info_log.error("无法监听!");
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
	err = ioctlsocket(m_socklisten,FIONBIO,&ulCmd);*/ //设置为非阻塞模式
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

	//创建acceptex监视线程
	m_hGuardAcceptExThread = (HANDLE)_beginthreadex(NULL,0,GuardAcceptEx,this,0,&iID);


	m_bstop = false;
}

//创建sock
LPPER_HANDLE_DATA CIOCPSockSrv::CreatePerHander()
{
	log4cpp::Category& info_log = log4cpp::Category::getInstance("info");
	DWORD dwBytes,dwCode;
	DWORD dwAddressLength = sizeof(sockaddr_in) + 16;
	//创建单句柄数据
	LPPER_HANDLE_DATA lpPerListenData = (LPPER_HANDLE_DATA)HeapAlloc(m_hIOHeap ,HEAP_ZERO_MEMORY,
		sizeof(PER_HANDLE_DATA ));

	lpPerListenData->socktype = LISTENSOCK;
	lpPerListenData->sock = WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED);

	//投递acceptex请求
	dwBytes = 0;
	BOOL bret = lpfnAcceptEx(m_socklisten,lpPerListenData->sock,lpPerListenData->tinybuf,0,
		dwAddressLength, dwAddressLength, &dwBytes, &(lpPerListenData->ovl));

	if(bret == FALSE && (dwCode = WSAGetLastError()) != WSA_IO_PENDING)
	{			
		info_log.error("无法acceptex!");
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
			FALSE,WSA_INFINITE,TRUE); //等待m_hListenEvent事件

		dwIndex -= WSA_WAIT_EVENT_0;
		if (WAIT_IO_COMPLETION == dwIndex)
		{
			//被唤醒,退出循环,结束程序
			break;
		}
		if(WSA_WAIT_FAILED == dwIndex)
		{
			dwErr = WSAGetLastError();
			info_log.error("GuardAcceptEx 产生错误如下(退出):%s",CsuUtil::GetLastErrorMessageString(dwErr));
			break;
		}
		memset(&netEvents,0,sizeof(netEvents));
		WSAEnumNetworkEvents(pIOCPSockSrv->m_socklisten,pIOCPSockSrv->m_hListenEvent,
			&netEvents); //获得触发的事件

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

//释放sock
void CIOCPSockSrv::ReleaseSock(SOCKET sock,int threadindex, bool binitiative)
{
	m_mutexSockmap.lock();
	bool bNeedNotifyNextPack=false;
	SOCKMAP::iterator itsockmap;
	itsockmap = m_sockmap.find(sock);
	if(itsockmap != m_sockmap.end())
	{	
		//为避免释放还在工作线程中的sock,延迟释放资源
		itsockmap->second.adodata.bNeedClose = true;
		if(itsockmap->second.adodata.bSendPending ||
			itsockmap->second.adodata.netSendstate == NETBUSY)
		{
			//因为sock还在pending状态,就收到网络异常信息
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
		//通知下个数据包可以发送
		if (pIOCPSockSrv->m_pendingPackList.empty() == FALSE)
		{
			SetEvent(pIOCPSockSrv->m_phSendThread[0]);
		}
	}

	log4cpp::Category& info_log = log4cpp::Category::getInstance("info");
	info_log.info("成功释放socket:%d",sock);

	return;
}

//完成端口工作线程
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
				info_log.warn("CompletionThread 产生错误:%d,%s",lpPerHandleData->sock, CsuUtil::GetLastErrorMessageString(dwRet));
				pIOCPSockSrv->ReleaseSock(lpPerHandleData->sock,indexEvent,true);
				continue;
			} 
			else if(WSA_OPERATION_ABORTED == dwRet)
			{
				info_log.warn("CompletionThread 产生错误:%d,%s",lpPerHandleData->sock, CsuUtil::GetLastErrorMessageString(dwRet));
				pIOCPSockSrv->ReleaseSock(lpPerHandleData->sock,indexEvent,true);
				continue;
			}
			else
			{
				info_log.warn("CompletionThread 错误退出:%d,%s",lpPerHandleData->sock, CsuUtil::GetLastErrorMessageString(dwRet));
				break;
			}
		}

		if(lpPerHandleData == NULL)
		{
			info_log.info("lpPerHandleData is null 工作线程退出");
			break;
		}

		//服务器sock
		if(lpPerHandleData->socktype == SERVERSOCK)
		{
			LPPER_HANDLE_DATA lpPerListenData = CONTAINING_RECORD(pWsaOvl,PER_HANDLE_DATA,ovl);
			LPPER_IO_OPERATION_DATA lpRecvPerIOData,lpSendPerIOData;
			lpRecvPerIOData = lpSendPerIOData = NULL;
			//说明接收到一个新的accetpex请求
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
				//解释地址
				pIOCPSockSrv->lpfnGetAcceptExSockaddrs(lpPerListenData->tinybuf,0,
					dwAddressLength,dwAddressLength,&localAddr,&nlAddrlen,
					&remoteAddr,&nrAddrlen);

				memcpy(&(lpPerListenData->addr),remoteAddr,sizeof(lpPerListenData->addr));
				//得到对方地址(127.0.0.1)
				//char* ip = inet_ntoa (lpListen->addr.sin_addr);


				//创建单IO接收数据
				lpRecvPerIOData = (LPPER_IO_OPERATION_DATA)HeapAlloc(pIOCPSockSrv->m_hDataHeap,
					HEAP_ZERO_MEMORY,sizeof(PER_IO_OPERATION_DATA));
				lpRecvPerIOData->wsabuf.buf = lpRecvPerIOData->databuf;
				lpRecvPerIOData->wsabuf.len = 0;
				lpRecvPerIOData->operation = ZERORECV;

				//创建单IO发送数据
				lpSendPerIOData = (LPPER_IO_OPERATION_DATA)HeapAlloc(pIOCPSockSrv->m_hDataHeap,
					HEAP_ZERO_MEMORY,sizeof(PER_IO_OPERATION_DATA));
				lpSendPerIOData->wsabuf.buf = lpSendPerIOData->databuf;
				lpSendPerIOData->wsabuf.len = 0;
			}
			catch(...)
			{
				info_log.error("listenThread 分配sock资源错误");
				//堆空间分配异常
				if(lpPerListenData)
					HeapFree(pIOCPSockSrv->m_hIOHeap,0,lpPerListenData);
				if(lpRecvPerIOData)
					HeapFree(pIOCPSockSrv->m_hDataHeap,0,lpRecvPerIOData);
				if(lpSendPerIOData)
					HeapFree(pIOCPSockSrv->m_hDataHeap,0,lpSendPerIOData);
				continue;
			}

			//创建sock map
			SOCK_INFO sockinfo;
			memset(&sockinfo,0,sizeof(SOCK_INFO));
			sockinfo.predata = lpPerListenData;
			sockinfo.ioRecvData = lpRecvPerIOData;
			sockinfo.ioSendData = lpSendPerIOData;
			sockinfo.adodata.hAlive = CreateEvent(NULL,FALSE,FALSE,NULL);

			//为每个sock设置定时器
			if(RegisterWaitForSingleObject(&sockinfo.adodata.hTimeOut ,
				sockinfo.adodata.hAlive, TimeOutCallback,(PVOID)lpPerListenData->sock,
				nMaxTimeOut,pIOCPSockSrv->m_flagQueue)==FALSE)
			{
				dwErr = GetLastError();
				HeapFree(pIOCPSockSrv->m_hIOHeap,0,sockinfo.predata);
				HeapFree(pIOCPSockSrv->m_hDataHeap,0,sockinfo.ioRecvData);
				HeapFree(pIOCPSockSrv->m_hDataHeap,0,sockinfo.ioSendData);
				CloseHandle(sockinfo.adodata.hAlive);
				info_log.error("listenThread 注册超时事件失败:%s",CsuUtil::GetLastErrorMessageString(dwErr));
				continue;
			}
			//设置定时器结束

			pIOCPSockSrv->m_mutexSockmap.lock();
			pIOCPSockSrv->m_sockmap.insert(SOCKMAP::value_type(lpPerListenData->sock,sockinfo));
			pIOCPSockSrv->m_mutexSockmap.unlock();

			//将新接受的socket与完成端口联系到一起
			if(CreateIoCompletionPort((HANDLE)(lpPerListenData->sock),pIOCPSockSrv->m_hIOCP,
				(u_long)lpPerListenData,0) == NULL)
			{
				dwErr = GetLastError();
				info_log.error("listenThread 绑定完成端口错误:%s",CsuUtil::GetLastErrorMessageString(dwErr));
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
					info_log.error("listenThread 接受sock错误如下:%s",CsuUtil::GetLastErrorMessageString(dwErr));
					//关闭一个sock
					pIOCPSockSrv->ReleaseSock(lpPerListenData->sock);
					continue;
				}
			}
			InterlockedIncrement(&nTotal);
			info_log.info("listenThread 成功接收sock:%d,共有:(%d)个",lpPerListenData->sock,nTotal);

			continue;
		}//服务器sock

		//客户端sock
		lpPerIOData = CONTAINING_RECORD(pWsaOvl,PER_IO_OPERATION_DATA,ovl);
		//1.对sock做忙碌标记
		pIOCPSockSrv->m_mutexSockmap.lock();
		itsockmap = pIOCPSockSrv->m_sockmap.find(lpPerHandleData->sock);
		//处理数据开始
		if(itsockmap == pIOCPSockSrv->m_sockmap.end())
		{
			pIOCPSockSrv->m_mutexSockmap.unlock();
			continue; //不在sock map中就忽略
		}

		if (0 == dwByteTranslate && (itsockmap->second.adodata.bClose || 
			itsockmap->second.adodata.bNeedClose))
		{
			//关闭一个sock
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

		//解天mutexsock锁,开始具体接收与发送操作
		if(lpPerIOData->operation == ZERORECV)
		{			
			//设置收到心跳包事件(定时器)
			SetEvent(itsockmap->second.adodata.hAlive);
			long dwProcRemain = 0;
			lpPerIOData->wsabuf.len = MAX_BUFFER_SIZE;
			do{
				//使用非阻塞继续接收sock中的数据直遇到 WSAEWOULDBLOCK 错误，说明已读取完所有sock数据
				dwflag = dwTrans = 0;
				err = WSARecv(lpPerHandleData->sock,&(lpPerIOData->wsabuf),1,&dwTrans,&dwflag,
					NULL,NULL);
				if(0 == err)
				{
					dwProcRemain = dwTrans;
				}
				else if(SOCKET_ERROR == err && WSAEWOULDBLOCK == WSAGetLastError())
				{
					//说明sock接收缓存中没有数据了
					break;
				}
				else
				{
					//网络错误
					bNetError = true;				
				}

				//因为服务器执行了shutdown,导致dwProcRemain为0
				if(dwProcRemain == 0) 
					bNetError = true;

				if(bNetError)
				{
					//pIOCPSockSrv->m_mutexSockmap.lock();
					itsockmap->second.adodata.netRecvstate = NETIDLE;//设置接收为空闲状态
					//pIOCPSockSrv->m_mutexSockmap.unlock();

					if(itsockmap->second.adodata.bNeedClose || (dwProcRemain == 0))
					{
						//fix me貌似进入这个条件bclose一定为true
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
							//收到一条无效客户端的资料
							info_log.warn("拒绝处理无效客户端数据:%d",lpPerHandleData->sock);
							bNetError = true; //设置些客户端为异常
							break;
						}

						//收到一个全新的数据
						memcpy(&packhead,pvist,sizeof(PACKHEAD));
						pvist += sizeof(PACKHEAD);
						if(packhead.lpacklen <= 0)
							break;

						//1.设置收到新的包长度
						itsockmap->second.adodata.dwID = packhead.dwid;
						itsockmap->second.adodata.dwRecvPacklen = packhead.lpacklen;
						//2.计算第一次余下包的长度
						itsockmap->second.adodata.dwRecvRemain = dwProcRemain - sizeof(PACKHEAD) - itsockmap->second.adodata.dwRecvPacklen;
						if(itsockmap->second.adodata.dwRecvRemain < 0)
							itsockmap->second.adodata.dwRecvRemain = 0 - itsockmap->second.adodata.dwRecvRemain ;
						else
							itsockmap->second.adodata.dwRecvRemain = 0;

						//3.计算第一次收的包的长度,注意是新包数据,第一次收到
						dwRecv = itsockmap->second.adodata.dwRecvPacklen - itsockmap->second.adodata.dwRecvRemain;

						//创建包数据存储空间
						itsockmap->second.adodata.Recvdatapack = (char*)HeapAlloc(pIOCPSockSrv->m_hPack, HEAP_ZERO_MEMORY,
							itsockmap->second.adodata.dwRecvPacklen+1);
						//拷贝buffer数据至包数据存储空间
						itsockmap->second.adodata.Recvpos = itsockmap->second.adodata.Recvdatapack;
						memcpy(itsockmap->second.adodata.Recvpos,pvist,dwRecv);
						pvist += dwRecv;
						itsockmap->second.adodata.Recvpos += dwRecv;

						dwProcRemain = dwProcRemain - sizeof(PACKHEAD) - dwRecv;

					}
					else
					{
						//接收残余的包				
						//计算本次收到多少字节,注意这是残余包
						dwRecv = itsockmap->second.adodata.dwRecvRemain - dwProcRemain;
						if(dwRecv <= 0)
							dwRecv = itsockmap->second.adodata.dwRecvRemain;
						else
							dwRecv = dwProcRemain;
						//计算还有多少字节未接收
						itsockmap->second.adodata.dwRecvRemain -= dwRecv;
						if(itsockmap->second.adodata.dwRecvRemain < 0)
							itsockmap->second.adodata.dwRecvRemain = 0;

						memcpy(itsockmap->second.adodata.Recvpos,pvist,dwRecv);
						pvist += dwRecv;
						itsockmap->second.adodata.Recvpos += dwRecv;						
						dwProcRemain = dwProcRemain - dwRecv;
					}					

					//判断是否可以将接收到数据提交应用层处理
					if(itsockmap->second.adodata.dwRecvRemain == 0)
					{
						info_log.info("成功收到请求数据:%d",lpPerHandleData->sock);
						//处理一个完整的包
						char* pdata = itsockmap->second.adodata.Recvdatapack;
						itsockmap->second.adodata.Recvdatapack = NULL;
						pIOCPSockSrv->m_pAdoProvider->NotifyRecvPack(lpPerHandleData->sock,itsockmap->second.adodata.dwID,
							pdata, itsockmap->second.adodata.dwRecvPacklen);						
					}

				}//(dwProcRemain > 0) 一次数据包处理结束
				
				if(bNetError)
				{
					break;
				}

			}while(true);

			//网络层对本次数据处理结束

			if(bNetError)
			{
				//检测到网络发生异常
				continue;
			}

			//重新接收socket数据
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
				itsockmap->second.adodata.netRecvstate = NETIDLE;//设置接收为空闲状态
				//pIOCPSockSrv->m_mutexSockmap.unlock();
				//关闭一个sock
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
			itsockmap->second.adodata.netRecvstate = NETIDLE;//设置接收为空闲状态
			//pIOCPSockSrv->m_mutexSockmap.unlock();

		}//recv 结束
		else if(lpPerIOData->operation == SEND)
		{	
			bool bSendComplate = false; //标识是否发送完成
			dwflag = 0;
			if(itsockmap->second.adodata.nSendTimes == 0)
			{
				//第一次发送数据包
				itsockmap->second.adodata.nSendTimes++;
				itsockmap->second.adodata.dwSendTrans += (dwByteTranslate-sizeof(PACKHEAD));
			}
			else
			{
				//非第一次发送数据包
				itsockmap->second.adodata.nSendTimes++;
				itsockmap->second.adodata.dwSendTrans += dwByteTranslate;
			}
			itsockmap->second.adodata.dwSendRemain = itsockmap->second.adodata.dwSendPacklen - itsockmap->second.adodata.dwSendTrans;
			if(itsockmap->second.adodata.dwSendRemain > 0)
			{
				//还有未发送完的包
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

				//继续发送剩下的部分
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

					//发送出现错误
					//关闭一个sock
					//fix it
					delete[] itsockmap->second.adodata.Senddatapack;
					itsockmap->second.adodata.Senddatapack = NULL;
					bNetError = true;
				}
				else if((nRet == SOCKET_ERROR) && (dwErr == WSA_IO_PENDING))
				{
					//标记sock是pending状态
					itsockmap->second.adodata.bSendPending = true;

					info_log.info("发送进入pending状态:%d,第%d包",lpPerHandleData->sock,
						itsockmap->second.adodata.nSendTimes+1);
				}
			}
			else
			{

				//包发送完毕
				bSendComplate = true;
				delete[] itsockmap->second.adodata.Senddatapack;
				itsockmap->second.adodata.Senddatapack = NULL;
				itsockmap->second.adodata.netSendstate = NETIDLE;

				info_log.info("发送完毕:%d,共%d包",lpPerHandleData->sock,
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

			//发送完成或网络发生错误
			if(bSendComplate /*|| bNetError*/)
			{
				InterlockedDecrement(&(pIOCPSockSrv->m_lSend));
				//通知下个数据包可以发送
				if (pIOCPSockSrv->m_pendingPackList.empty() == FALSE)
				{
					SetEvent(pIOCPSockSrv->m_phSendThread[indexEvent]);
				}
			}

		}//send结束

	}
	info_log.info("CompletionThread退出");
	SetEvent(pIOCPSockSrv->m_phClose[indexEvent]);
	//线程结束
	return 0;
}

//发送pack,此时已锁定
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

	//发送操作
	it->second.ioSendData->operation = SEND;
	ZeroMemory(&(it->second.ioSendData->ovl),sizeof(WSAOVERLAPPED));

	it->second.adodata.Senddatapack = pack;
	it->second.adodata.dwSendPacklen = len; //数据长度
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

	//增加发送一个wsasend投递
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

	//fix me 出现错误在完成端口中释放资源
	if((nRet == SOCKET_ERROR) && (dwErr != WSA_IO_PENDING))
	{
		//减掉一个wsasend投递
		InterlockedDecrement(&m_lSend);
		it->second.adodata.Senddatapack = NULL;
		it->second.adodata.dwSendPacklen = 0;
		//发送出现错误
		//关闭一个sock
		it->second.adodata.bClose = true;
		shutdown(sock,SD_SEND);		
		//closesocket(sock);
		return false;
	}
	else if((nRet == SOCKET_ERROR) && (dwErr == WSA_IO_PENDING))
	{
		//标记客户端sock是pending状态
		//m_mutexSockmap.lock();
		it->second.adodata.bSendPending = true;
		SetKeepAlive(sock,KEEPTIME,KEEPINTERVAL);
		info_log.warn("发送进入pending状态:%d",sock);
		//m_mutexSockmap.unlock();
	}

	return true;
}

BOOL CIOCPSockSrv::ReleaseDataPack(char* pack)
{
	return HeapFree(m_hPack,0,pack);
}

//IO_FAIL 数据包永远不被发送
//IO_PENDING 数据包进入待发送队列
eIOSTATE CIOCPSockSrv::SendAdo(SOCKET sock,DWORD dwId,char* pack,long len,BOOL bAdd)
{
	eIOSTATE eRet = IO_FAIL;
	SOCKMAP::iterator it;
	m_mutexSockmap.lock();
	it = m_sockmap.find(sock);

	//判断要发送的目的sock是否已经失效。
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
	pendingpack->pack = pack; //赋值要发送的数据
	m_mutexCheck.lock();
	m_pendingPackList.push_back(pendingpack);
	m_mutexCheck.unlock();
	eRet = IO_PENDING;
	SetEvent(m_phSendThread[0]);
	return eRet;
}

//仅发送头队列元素
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

		//1.判断要发送数据的客户端是否有效
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

		//2.判断是否可以向客户端发送数据
		if(enetstate == NETIDLE)
		{
			//发送当前数据包
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
				//向客户端成功投递数据
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
			//因当前要发送的客户端正在发送,不可以同时发送,取下个数据包
			it++;
			continue;
		}
	}

	return bsend;
}

//检查还未发送队列线程
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
		//超时
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