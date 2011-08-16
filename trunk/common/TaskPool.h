/*
线程池类,在此工程中的作用是做为数据库联接池使用
线程池通过改正,可以创建1024个线程。但数据库可以达到的并发连接数要依据实际情况配置 2009-10-17
author: lincolnfz
lincolnfz@gmail.com
*/

#pragma once

#include <process.h>
#include <vector>
#include <list>
#include "../common/Mutex.h"

typedef void* (*INITTASK)(); //工作线程初始化函数,返回函数的上下文

//工作函数,参数1 函数的上下文 参数2 函数的参数指针,对使用者仅保留
typedef DWORD (*PROCFUN)(/*in*/void* lpParam , /*out*/void* funContext);

//清理函数
typedef void (*CLEARFUN)(/*out*/void* funContext);

typedef struct{
	int igroup;//所在的事件组
	int idx; //事件组里的索引值
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
