#pragma once

#include <WinSock2.h>
#include <MSWSock.h>
#include <map>
#include "Ado.h"


#define MAX_BUFFER_SIZE 4096
#define TIME_OUT 300000

typedef struct
{
	long lpacklen;
	DWORD dwid;
}PACKHEAD;

typedef enum{
	NETIDLE,
	NETBUSY,
	NETOK,
	NETFAIL
}eNETSTATE;

typedef struct  
{
	SOCKET sock;
	DWORD dwID;
	long packlen;
	char* pack;
}PENDINGPACK;

typedef struct  
{
	DWORD dwID;	//�ͻ���adoset
	/////////���ղ���
	long dwRecvPacklen;//�����ܳ���
	long dwRecvTrans;//��ǰ�Ѵ���ĳ���,û���õ�
	long dwRecvRemain;//��ʣ�µĳ���
	char* Recvdatapack;//��
	char* Recvpos;
	/////////���Ͳ���
	long dwSendPacklen;//�����ܳ���
	long dwSendTrans;//��ǰ�Ѵ���ĳ���
	long dwSendRemain;//��ʣ�µĳ���
	int nSendTimes; //���ʹ���
	char* Senddatapack;//��
	//bool isPending; //�Ƿ�������ݰ�
	//PENDINGPACK* pending;//�Ƿ�������ݰ�
	//
	eNETSTATE netSendstate;
	eNETSTATE netRecvstate;
	HANDLE hAlive; //��¼�
	HANDLE hTimeOut; //�ȴ���ʱ�¼�
	bool bNeedClose;
	bool bCheckPending; //�Ƿ��ڼ��Ҫ���ͳ����߳���
	//bool bRecvPending;//sock�����Ƿ���pending״̬
	bool bSendPending;//sock�����Ƿ���pending״̬
	bool bClose;//�Ƿ�ִ����shutdown����
	bool bInSendWorkThread;//�Ƿ��ڹ����߷����߳���
}DATACLIENT;

//typedef std::map<DWORD,DATACLIENT> ADOCLIENTMAP;

typedef enum
{
	DIRECT,
	HTTP,
	SOCK,
}PROXYTYPE;

typedef enum
{
	ZERORECV,
	RECV,
	SEND,
	SENDCOMPLATED,
	ACCEPT,
}OP;

typedef enum
{
	SERVERSOCK,
	LISTENSOCK,
}SOCKTYPE;

//�߳�������
typedef struct
{
	HANDLE hICOP;	//��ɶ˿�
	HANDLE hIOHeap;	//ioר�ö�
	HANDLE hDataHeap;	//����ר�ö�
	//int indexEvent; //���Է����¼�
	void*  ptr;
}THREADCONTEXT,*LPTHREADCONTEXT;

//������ṹ
typedef struct
{
	SOCKET sock;
	sockaddr_in addr;
	WSAOVERLAPPED ovl;//Ϊ���acceptex,���ovl����
	char tinybuf[128];
	SOCKTYPE socktype;
}PER_HANDLE_DATA,*LPPER_HANDLE_DATA;

//��������ݽṹ
typedef struct
{
	WSAOVERLAPPED ovl;
	WSABUF wsabuf;
	char databuf[MAX_BUFFER_SIZE];
	OP operation;	//����ģʽ
}PER_IO_OPERATION_DATA,*LPPER_IO_OPERATION_DATA;

typedef struct
{
	DATACLIENT adodata;
	LPPER_HANDLE_DATA predata;
	LPPER_IO_OPERATION_DATA ioRecvData;
	LPPER_IO_OPERATION_DATA ioSendData;
}SOCK_INFO,*LPSOCK_INFO;

typedef struct
{
	SOCKET sock;
	DWORD id;
	char* databuf; //���PACK_ADO����
	unsigned long datalen; //���PACK_ADO����
	DWORD time;	//��Ϣʱ��
}PACK_INFO,*LPACK_INFO; //�����

typedef enum{
	REQUEST_ASK,
	RESULT_SUCESS,
	RESULT_FAIL
}EPACKATTRIB;

typedef struct 
{
	unsigned int uiUserFlag;
	enum{
		ADO_EXECUTE,
		ADO_UPDATE
	}adotype;
	EPACKATTRIB result;
	unsigned long datalen; //char* data�ĳ���
	char* data;
}PACK_ADO; //Ӧ�ò�

typedef enum{
	IO_OK,
	IO_FAIL,
	IO_PENDING
}eIOSTATE;

typedef struct{
	CAdoConnection* pAdoConnect;
	CAdoRecordSet* pAdoRecord;
}ADOCONTEXT;

typedef struct _PROCITEM 
{
	char *data;
	char *result;
	PACK_ADO* pack_ado;
	PACK_ADO* result_ado;
	void* pSrvUdpPeer;
	unsigned long sendlen;
}PROCITEM;

