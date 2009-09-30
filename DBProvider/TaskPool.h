/*
线程池类,在此工程中的作用是做为数据库联接池使用
author: lincolnfz
lincolnfz@gmail.com
*/

#pragma once

#include <process.h>
#include <list>
#include "Mutex.h"

typedef void* (*INITTASK)(); //工作线程初始化函数,返回函数的上下文

//工作函数,参数1 函数的上下文 参数2 函数的参数指针,对使用者仅保留
typedef DWORD (*PROCFUN)(/*in*/void* lpParam , /*out*/void* funContext);

//清理函数
typedef void (*CLEARFUN)(/*out*/void* funContext);

typedef struct{
	int idx;
	HANDLE hThread;
	HANDLE hEvents[2];//hEvents[0]执行信号 hEvents[1]退出信号
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
