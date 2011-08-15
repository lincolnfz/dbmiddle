/*
IOCPSockSrv是在iocp的基础上封装的sock服务器类
author: lincolnfz
lincolnfz@gmail.com
*/

#pragma once

#include <list>
#include <map>
#include <process.h>
#include "../common/CommonHead.h"
#include "../common/Mutex.h"
#include "../common/UDTExpand.h"

class CAdoProvider;

typedef struct tag_TCPKEEPALIVE   
{   
	unsigned long onoff;
	unsigned long keepalivetime;
	unsigned long keepaliveinterval;     
}_TCPKEEPALIVE;   

#define SIO_KEEPALIVE_VALS  _WSAIOW(IOC_VENDOR,4)
#define KEEPTIME 10000
#define KEEPINTERVAL 1

typedef std::map<SOCKET,SOCK_INFO> SOCKMAP;

typedef std::list<PENDINGPACK*> PENDINGPACKLIST;

class CIOCPSockSrv
{
public:
	CIOCPSockSrv();
	~CIOCPSockSrv();

	/************************************************************************/
	/* 启动iocp服务   
	nkernel 开启几个并发线程*/
	/************************************************************************/
	void Run(DWORD nkernel=1);

	//停止iocp服务
	void Stop();

	//设置AdoProvider 指针
	void SetAdoProviderPtr(CAdoProvider* pAdoProvider);

	//设定监听端口
	void SetListenPort(int nPort);
	BOOL ReleaseDataPack(char* pack);//释放sock所开的堆空间
	eIOSTATE SendAdo(SOCKET sock,DWORD dwId,char* pack,long len,BOOL bAdd);
	BOOL ExistSock(SOCKET sock);
	void setMaxSend(int nMaxSend);


protected:
	LPFN_ACCEPTEX lpfnAcceptEx;
	LPFN_GETACCEPTEXSOCKADDRS lpfnGetAcceptExSockaddrs;
	SOCKMAP m_sockmap;
	HANDLE m_hIOCP,m_hIOHeap,m_hDataHeap,m_hGuardAcceptExThread,m_hListenEvent;
	HANDLE m_hPack;//数据包堆
	HANDLE m_hunzipPack;//原始数据包堆
	HANDLE m_hCheckThread;
	HANDLE m_hEventClose[2];
	HANDLE* m_phSendThread; //发送线程指针数组
	HANDLE* m_phClose;//关闭事件
	SOCKET m_socklisten;
	THREADCONTEXT m_ThreadContext;
	bool m_bstop;
	Mutex m_mutexCheck,m_mutexSockmap;
	CAdoProvider* m_pAdoProvider;
	int m_nListenPort;
	PENDINGPACKLIST m_pendingPackList;
	unsigned long m_nSockThread;
	unsigned long m_flagQueue;
	volatile LONG m_lSend; //有多少个共作者线程正在发送
	volatile LONG m_maxWsaSend; //允许最多投递多少个wsasend
	CUDTExpand m_udtSrv;

	static unsigned int CALLBACK CompletionThread(LPVOID lpParameter);
	static unsigned int CALLBACK GuardAcceptEx(LPVOID lpParameter);
	static unsigned int CALLBACK CheckPendingThread(LPVOID lpParameter);
	static void CALLBACK TimeOutCallback(PVOID lpParameter,BOOLEAN TimerOrWaitFired);
	static void CALLBACK APCProc(ULONG_PTR dwParam);
	void Clean();
	//void EraseSockMap(SOCKMAP::iterator eraseit);
	void ReleaseSock(SOCKET sock,int threadindex=0, bool binitiative=false);
	LPPER_HANDLE_DATA CreatePerHander();
	void SetKeepAlive(SOCKET sock, u_long keepalivetime, u_long keepaliveinterval, bool bOff = false);
	

	/*发送数据包*/
	bool SendPack(SOCKET sock,DWORD dwId,char* pack,long len);
	bool CheckPendingList();
	
};
