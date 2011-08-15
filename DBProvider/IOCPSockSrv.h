/*
IOCPSockSrv����iocp�Ļ����Ϸ�װ��sock��������
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
	/* ����iocp����   
	nkernel �������������߳�*/
	/************************************************************************/
	void Run(DWORD nkernel=1);

	//ֹͣiocp����
	void Stop();

	//����AdoProvider ָ��
	void SetAdoProviderPtr(CAdoProvider* pAdoProvider);

	//�趨�����˿�
	void SetListenPort(int nPort);
	BOOL ReleaseDataPack(char* pack);//�ͷ�sock�����Ķѿռ�
	eIOSTATE SendAdo(SOCKET sock,DWORD dwId,char* pack,long len,BOOL bAdd);
	BOOL ExistSock(SOCKET sock);
	void setMaxSend(int nMaxSend);


protected:
	LPFN_ACCEPTEX lpfnAcceptEx;
	LPFN_GETACCEPTEXSOCKADDRS lpfnGetAcceptExSockaddrs;
	SOCKMAP m_sockmap;
	HANDLE m_hIOCP,m_hIOHeap,m_hDataHeap,m_hGuardAcceptExThread,m_hListenEvent;
	HANDLE m_hPack;//���ݰ���
	HANDLE m_hunzipPack;//ԭʼ���ݰ���
	HANDLE m_hCheckThread;
	HANDLE m_hEventClose[2];
	HANDLE* m_phSendThread; //�����߳�ָ������
	HANDLE* m_phClose;//�ر��¼�
	SOCKET m_socklisten;
	THREADCONTEXT m_ThreadContext;
	bool m_bstop;
	Mutex m_mutexCheck,m_mutexSockmap;
	CAdoProvider* m_pAdoProvider;
	int m_nListenPort;
	PENDINGPACKLIST m_pendingPackList;
	unsigned long m_nSockThread;
	unsigned long m_flagQueue;
	volatile LONG m_lSend; //�ж��ٸ��������߳����ڷ���
	volatile LONG m_maxWsaSend; //�������Ͷ�ݶ��ٸ�wsasend
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
	

	/*�������ݰ�*/
	bool SendPack(SOCKET sock,DWORD dwId,char* pack,long len);
	bool CheckPendingList();
	
};
