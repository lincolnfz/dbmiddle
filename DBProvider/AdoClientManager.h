#pragma once

#include <list>
#include <map>
#include "AsynSock.h"
#include "Mutex.h"

class CAdoClient;

typedef struct{
	CAdoClient* pAdoClient;
	long len;
	char* pbuf;
}RECVADOBUF;

typedef struct{
	CAdoClient* padoclient;
	eNETSTATE adonetstate;
}ADOCLIENT;

typedef struct{
	DWORD id;
	char* buf;
	int buflen;
}BUFINFO;

typedef std::map<DWORD,ADOCLIENT> ADOMAP;
typedef std::list<BUFINFO> DATAQUEUE;

class CAdoClientManager
{
public:
	CAdoClientManager(void);
	~CAdoClientManager(void);

	eNETSTATE sendAdoData(CAdoClient* padoclient,PACK_ADO& pack);
	static void CALLBACK NotefyCompSendProc(ULONG_PTR dwParam);
	static unsigned int CALLBACK SendThread(LPVOID lpParameter);
	void recvAdoData(DWORD id, long buflen, char* buf);
	void CompleteSend(DWORD id);

	//通知网络层释放所分配的空间
	BOOL releaseBuf(char* buf);

	void SetProxy(std::string proxyip,int proxyport,PROXYTYPE pt,
			std::string proxyUsernam="",std::string proxyPassword="");
	void SetDBProvider(std::string serverip,int port);
	bool ConnectDBProvider(std::string serverip,int port);

	
protected:
	//初始化
	void init();

private:
	HANDLE m_hSendThread;
	CAsynSock m_sock;
	ADOMAP m_adomap;
	DATAQUEUE m_sendQueue;
	Mutex m_mutex;
	HANDLE m_hEvent;
};
