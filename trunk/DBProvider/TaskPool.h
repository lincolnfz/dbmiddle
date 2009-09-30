/*
�̳߳���,�ڴ˹����е���������Ϊ���ݿ����ӳ�ʹ��
author: lincolnfz
lincolnfz@gmail.com
*/

#pragma once

#include <process.h>
#include <list>
#include "Mutex.h"

typedef void* (*INITTASK)(); //�����̳߳�ʼ������,���غ�����������

//��������,����1 ������������ ����2 �����Ĳ���ָ��,��ʹ���߽�����
typedef DWORD (*PROCFUN)(/*in*/void* lpParam , /*out*/void* funContext);

//������
typedef void (*CLEARFUN)(/*out*/void* funContext);

typedef struct{
	int idx;
	HANDLE hThread;
	HANDLE hEvents[2];//hEvents[0]ִ���ź� hEvents[1]�˳��ź�
	INITTASK pTaskInit;
	PROCFUN pWorkFun;
	CLEARFUN pClearFun;
	void* lpParam;
}WORKTHREADITEM;

typedef struct{
	PROCFUN pWorkFun;
	void* lpParam;
}PENDINGITEM;

typedef std::list<WORKTHREADITEM*> THREADLIST;
typedef std::list<PENDINGITEM> PENDINGLIST;

class CTaskPool
{
public:
	CTaskPool(void);
	~CTaskPool(void);

	void Init(int nThread, INITTASK InitFun=NULL, CLEARFUN ClearFun=NULL);
	void StopAll();
	int SubmitTaskItem(PROCFUN procfun,void* lpParam);

protected:
	static unsigned int CALLBACK WorkThread(LPVOID lpParameter);

	Mutex m_mutexPendingList,m_mutexSwap,m_mutexSubmit;
	int m_nThred;
	THREADLIST m_IdleThreadList,m_BusyThreadList;
	PENDINGLIST m_PendingList;
	bool m_bStop;
	HANDLE m_EventArray[64];
};
