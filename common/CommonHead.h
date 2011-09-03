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
	DWORD dwID;	//客户端adoset
	/////////接收部分
	long dwRecvPacklen;//包的总长度
	long dwRecvTrans;//当前已传输的长度,没有用到
	long dwRecvRemain;//还剩下的长度
	char* Recvdatapack;//包
	char* Recvpos;
	/////////发送部分
	long dwSendPacklen;//包的总长度
	long dwSendTrans;//当前已传输的长度
	long dwSendRemain;//还剩下的长度
	int nSendTimes; //发送次数
	char* Senddatapack;//包
	//bool isPending; //是否待发数据包
	//PENDINGPACK* pending;//是否待发数据包
	//
	eNETSTATE netSendstate;
	eNETSTATE netRecvstate;
	HANDLE hAlive; //活动事件
	HANDLE hTimeOut; //等待超时事件
	bool bNeedClose;
	bool bCheckPending; //是否在检查要发送除列线程中
	//bool bRecvPending;//sock接收是否在pending状态
	bool bSendPending;//sock发送是否在pending状态
	bool bClose;//是否执行了shutdown操作
	bool bInSendWorkThread;//是否在工作者发送线程中
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

//线程上下文
typedef struct
{
	HANDLE hICOP;	//完成端口
	HANDLE hIOHeap;	//io专用堆
	HANDLE hDataHeap;	//数据专用堆
	//int indexEvent; //可以发送事件
	void*  ptr;
}THREADCONTEXT,*LPTHREADCONTEXT;

//单句柄结构
typedef struct
{
	SOCKET sock;
	sockaddr_in addr;
	WSAOVERLAPPED ovl;//为配合acceptex,添加ovl属性
	char tinybuf[128];
	SOCKTYPE socktype;
}PER_HANDLE_DATA,*LPPER_HANDLE_DATA;

//单句柄数据结构
typedef struct
{
	WSAOVERLAPPED ovl;
	WSABUF wsabuf;
	char databuf[MAX_BUFFER_SIZE];
	OP operation;	//操作模式
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
	char* databuf; //存放PACK_ADO内容
	unsigned long datalen; //存放PACK_ADO长度
	DWORD time;	//消息时间
}PACK_INFO,*LPACK_INFO; //网络层

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
	unsigned long datalen; //char* data的长度
	char* data;
}PACK_ADO; //应用层

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

