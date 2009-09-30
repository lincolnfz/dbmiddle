/*
基于重叠模型写的异步sock
author: lincoln
email: lincolnfz@gmail.com
*/
#pragma once

#include <process.h>
#include <string.h>
#include "CommonHead.h"
#include "Mutex.h"

class CAdoClientManager;

typedef struct 
{
	WSAOVERLAPPED ovl;
	WSABUF wsabuf;
	char databuf[MAX_BUFFER_SIZE];
	char* pdata;
	long lpacklen; //传输包长度
	long ltrans; //已传输长度
	long lremain; //还剩下的长度
	int ntimes;
	DWORD id;
}OVLDATA;

class CAsynSock
{
public:
	CAsynSock(CAdoClientManager* pAdoClientManager);
	~CAsynSock();

	bool Connect();
	bool DisConnect();
	bool SendData(DWORD id,char* pack,int len);//此方法供应用层调用
	void SetServer(std::string serverip,int port);
	void SetProxy(std::string proxyip,int proxyport,PROXYTYPE pt,
		std::string proxyUsernam="",std::string proxyPassword="");

	BOOL releaseBuf(char* buf);

protected:
	static unsigned int CALLBACK IOWorkThread(LPVOID lpParameter);

private:
	SOCKET m_sock;
	char m_srvip[16],m_proxyip[16];//服务器ip,代理ip
	int m_port,m_proxyport;//服务端口,代理端口
	bool m_bRun,m_bConnected,m_bAuth;//m_bConnected是否联接上服务器
	PROXYTYPE m_eProxyType;
	std::string m_proxyUsernam,m_proxyPassword;

	HANDLE m_hIOThread;
	int m_nTotalEvent;
	HANDLE m_eventArray[64];
	OVLDATA m_sendOvl,m_recvOvl;
	int m_iRunProxyTimes;
	HANDLE m_hPackData; //堆空间
	HANDLE m_hConnected;//

	char m_szBuffer[2048];
	char m_szAuth[1024];
	char m_szAuthT[1024];
	/*WSAOVERLAPPED m_recvOvl,m_sendOvl;
	char m_recvBuf[MAX_BUFFER_SIZE];
	char m_sendBuf[MAX_BUFFER_SIZE];
	WSABUF m_recvWsa,m_sendWsa;*/

	CAdoClientManager* m_pAdoClientManager;

	void Clean();
	void Init();
	void TranslateDestAddr();
};
