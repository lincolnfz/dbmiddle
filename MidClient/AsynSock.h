/*
�����ص�ģ��д���첽sock
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
	long lpacklen; //���������
	long ltrans; //�Ѵ��䳤��
	long lremain; //��ʣ�µĳ���
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
	bool SendData(DWORD id,char* pack,int len);//�˷�����Ӧ�ò����
	void SetServer(std::string serverip,int port);
	void SetProxy(std::string proxyip,int proxyport,PROXYTYPE pt,
		std::string proxyUsernam="",std::string proxyPassword="");

	BOOL releaseBuf(char* buf);

protected:
	static unsigned int CALLBACK IOWorkThread(LPVOID lpParameter);

private:
	SOCKET m_sock;
	char m_srvip[16],m_proxyip[16];//������ip,����ip
	int m_port,m_proxyport;//����˿�,����˿�
	bool m_bRun,m_bConnected,m_bAuth;//m_bConnected�Ƿ������Ϸ�����
	PROXYTYPE m_eProxyType;
	std::string m_proxyUsernam,m_proxyPassword;

	HANDLE m_hIOThread;
	int m_nTotalEvent;
	HANDLE m_eventArray[64];
	OVLDATA m_sendOvl,m_recvOvl;
	int m_iRunProxyTimes;
	HANDLE m_hPackData; //�ѿռ�
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
