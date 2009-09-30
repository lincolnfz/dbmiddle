/*
线程池类,在此工程中的作用是做为数据库联接池使用
author: lincolnfz
lincolnfz@gmail.com
*/

#include "StdAfx.h"
#include "TaskPool.h"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

CTaskPool* pTaskPool = NULL;
CTaskPool::CTaskPool(void)
{
	pTaskPool = this;
	m_bStop = true;
	m_nThred = 0;
}

CTaskPool::~CTaskPool(void)
{
}

void CTaskPool::Init(int nThread, INITTASK InitFun, CLEARFUN ClearFun)
{
	if(nThread > 64) nThread = 64;
	if(nThread < 1) nThread = 1;

	m_nThred = nThread;
	unsigned int uiID;
	for (int i=0;i<m_nThred;i++)
	{
		m_EventArray[i] = CreateEvent(NULL,TRUE,FALSE,NULL);
		WORKTHREADITEM* pWorkItem = new WORKTHREADITEM;
		pWorkItem->idx = i;
		pWorkItem->hEvents[0] = CreateEvent(NULL,TRUE,FALSE,NULL);
		pWorkItem->hEvents[1] = CreateEvent(NULL,TRUE,FALSE,NULL);
		pWorkItem->pTaskInit = InitFun;  //初始线程
		pWorkItem->pClearFun = ClearFun; //清理线程
		pWorkItem->pWorkFun = NULL;
		pWorkItem->lpParam = NULL;
		pWorkItem->hThread = (HANDLE)_beginthreadex(NULL,0,WorkThread,pWorkItem,0,&uiID);
		CloseHandle(pWorkItem->hThread);
		m_IdleThreadList.push_back(pWorkItem);
	}
	m_bStop = false;
}

void CTaskPool::StopAll()
{
	m_bStop = true;

	THREADLIST::iterator it;

	m_mutexPendingList.lock();
	m_PendingList.clear();
	m_mutexPendingList.unlock();

	m_mutexSwap.lock();
	for (it = m_IdleThreadList.begin();it !=m_IdleThreadList.end();it++)
	{
		SetEvent((*it)->hEvents[1]);
	}
	for (it = m_BusyThreadList.begin();it !=m_BusyThreadList.end();it++)
	{
		SetEvent((*it)->hEvents[1]);
	}
	m_mutexSwap.unlock();

	//等待所有的线程正常退出
	DWORD dwRet = WaitForMultipleObjectsEx(m_nThred, m_EventArray, TRUE, INFINITE, FALSE);

	for (int i = 0; i < m_nThred; i++)
	{
		CloseHandle(m_EventArray[i]);
	}

	m_nThred = 0;
	m_IdleThreadList.clear();
	m_BusyThreadList.clear();
}

unsigned int CALLBACK CTaskPool::WorkThread(LPVOID lpParameter)
{
	WORKTHREADITEM* pWorkItem = (WORKTHREADITEM*)lpParameter;
	
	void* funContext = NULL;//函数上下文
	
	//先初始化线程函数
	if(pWorkItem->pTaskInit)
	{
		funContext = pWorkItem->pTaskInit();
	}

	//线程主体
	while (TRUE)
	{
		DWORD dwRet = WaitForMultipleObjectsEx(2,pWorkItem->hEvents,FALSE,INFINITE,TRUE);
		dwRet -= WAIT_OBJECT_0;
		if (dwRet == WAIT_IO_COMPLETION)
		{
			break;
		}
		else if(dwRet == 1)
		{
			ResetEvent(pWorkItem->hEvents[dwRet]);
			//退出
			break;
		}
		else if(dwRet == 0)
		{
			ResetEvent(pWorkItem->hEvents[dwRet]);
			//执行线程函数
			if(pWorkItem->pWorkFun)
			{
				//开始执行
				pWorkItem->pWorkFun(pWorkItem->lpParam, funContext);
				//执行结束

				pTaskPool->m_mutexPendingList.lock(); //保护m_PendingList
				if(pTaskPool->m_PendingList.empty() == false)
				{
					//继续从待处理队列中取出一个工作项
					PENDINGLIST::reference rf = pTaskPool->m_PendingList.front();
					PENDINGITEM pendingitem = rf;
					pTaskPool->m_PendingList.pop_front();
					pTaskPool->m_mutexPendingList.unlock();//取消保护m_PendingList

					pWorkItem->lpParam = pendingitem.lpParam;
					pWorkItem->pWorkFun = pendingitem.pWorkFun;
					SetEvent(pWorkItem->hEvents[0]);
				}
				else
				{
					pTaskPool->m_mutexPendingList.unlock(); //取消保护m_PendingList

					//将线程放回idle队列
					//busy交换到idle队列
					pTaskPool->m_mutexSwap.lock();
					THREADLIST::iterator itfind = std::find(pTaskPool->m_BusyThreadList.begin(),
						pTaskPool->m_BusyThreadList.end(), pWorkItem);

					if(itfind != pTaskPool->m_BusyThreadList.end())
					{						
						pTaskPool->m_BusyThreadList.erase(itfind);
						pTaskPool->m_IdleThreadList.push_back(pWorkItem);						
					}
					pTaskPool->m_mutexSwap.unlock();
					
				}
			}
		}
	}
	//线程主体处理结束

	//清理线程
	if (pWorkItem->pClearFun)
	{
		pWorkItem->pClearFun(funContext);
	}

	//线程结束
	SetEvent(pTaskPool->m_EventArray[pWorkItem->idx]);
	delete pWorkItem;
	
	return 0;
}

//procfun 函数指针 lpParam参数列表指针
int CTaskPool::SubmitTaskItem(PROCFUN procfun,void* lpParam)
{
	int iRet = -1;

	if(m_bStop)
	{
		return iRet;
	}
	
	//pTaskPool->m_mutexSwap.lock();
	//pTaskPool->m_mutexSwap.unlock();
	m_mutexSubmit.lock();
	if(m_IdleThreadList.empty() == false)
	{
		//直接提交到idle线程工作
		iRet = 1;
		THREADLIST::reference rf = m_IdleThreadList.front();
		WORKTHREADITEM* pWorkItem = rf;
		pWorkItem->pWorkFun = procfun;
		pWorkItem->lpParam = lpParam;	

		//idle交换到busy队列
		m_mutexSwap.lock();
		m_IdleThreadList.pop_front();			
		m_BusyThreadList.push_back(pWorkItem);
		m_mutexSwap.unlock();

		SetEvent(pWorkItem->hEvents[0]);//
	}
	else
	{
		//放在待处理队列尾
		iRet = 0;
		PENDINGITEM pendingitem;
		pendingitem.pWorkFun = procfun;
		pendingitem.lpParam = lpParam;
		m_mutexPendingList.lock();
		m_PendingList.push_back(pendingitem);
		m_mutexPendingList.unlock();
	}
	m_mutexSubmit.unlock();

	return iRet;
}