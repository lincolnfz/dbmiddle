/*
�̳߳���,�ڴ˹����е���������Ϊ���ݿ����ӳ�ʹ��
�̳߳�ͨ������,���Դ���1024���̡߳������ݿ���Դﵽ�Ĳ���������Ҫ����ʵ��������� 2009-10-17
author: lincolnfz
lincolnfz@gmail.com
*/

#pragma once

#include <process.h>
#include <vector>
#include <list>
#include "../common/Mutex.h"

typedef void* (*INITTASK)(); //�����̳߳�ʼ������,���غ�����������

//��������,����1 ������������ ����2 �����Ĳ���ָ��,��ʹ���߽�����
typedef DWORD (*PROCFUN)(/*in*/void* lpParam , /*out*/void* funContext);

//������
typedef void (*CLEARFUN)(/*out*/void* funContext);

typedef struct{
	int igroup;//���ڵ��¼���
	int idx; //�¼����������ֵ
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

typedef struct{
	HANDLE hEventArray[64];
}EVENTITEMS;

typedef std::list<WORKTHREADITEM*> THREADLIST;
typedef std::list<PENDINGITEM> PENDINGLIST;
typedef std::vector<EVENTITEMS> EVENTVECTOR;

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
	//HANDLE m_EventArray[64];
	EVENTVECTOR m_vEvent;
};
