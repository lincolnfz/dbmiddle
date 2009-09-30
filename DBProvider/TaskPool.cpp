/*
�̳߳���,�ڴ˹����е���������Ϊ���ݿ����ӳ�ʹ��
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
		pWorkItem->pTaskInit = InitFun;  //��ʼ�߳�
		pWorkItem->pClearFun = ClearFun; //�����߳�
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

	//�ȴ����е��߳������˳�
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
	
	void* funContext = NULL;//����������
	
	//�ȳ�ʼ���̺߳���
	if(pWorkItem->pTaskInit)
	{
		funContext = pWorkItem->pTaskInit();
	}

	//�߳�����
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
			//�˳�
			break;
		}
		else if(dwRet == 0)
		{
			ResetEvent(pWorkItem->hEvents[dwRet]);
			//ִ���̺߳���
			if(pWorkItem->pWorkFun)
			{
				//��ʼִ��
				pWorkItem->pWorkFun(pWorkItem->lpParam, funContext);
				//ִ�н���

				pTaskPool->m_mutexPendingList.lock(); //����m_PendingList
				if(pTaskPool->m_PendingList.empty() == false)
				{
					//�����Ӵ����������ȡ��һ��������
					PENDINGLIST::reference rf = pTaskPool->m_PendingList.front();
					PENDINGITEM pendingitem = rf;
					pTaskPool->m_PendingList.pop_front();
					pTaskPool->m_mutexPendingList.unlock();//ȡ������m_PendingList

					pWorkItem->lpParam = pendingitem.lpParam;
					pWorkItem->pWorkFun = pendingitem.pWorkFun;
					SetEvent(pWorkItem->hEvents[0]);
				}
				else
				{
					pTaskPool->m_mutexPendingList.unlock(); //ȡ������m_PendingList

					//���̷߳Ż�idle����
					//busy������idle����
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
	//�߳����崦�����

	//�����߳�
	if (pWorkItem->pClearFun)
	{
		pWorkItem->pClearFun(funContext);
	}

	//�߳̽���
	SetEvent(pTaskPool->m_EventArray[pWorkItem->idx]);
	delete pWorkItem;
	
	return 0;
}

//procfun ����ָ�� lpParam�����б�ָ��
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
		//ֱ���ύ��idle�̹߳���
		iRet = 1;
		THREADLIST::reference rf = m_IdleThreadList.front();
		WORKTHREADITEM* pWorkItem = rf;
		pWorkItem->pWorkFun = procfun;
		pWorkItem->lpParam = lpParam;	

		//idle������busy����
		m_mutexSwap.lock();
		m_IdleThreadList.pop_front();			
		m_BusyThreadList.push_back(pWorkItem);
		m_mutexSwap.unlock();

		SetEvent(pWorkItem->hEvents[0]);//
	}
	else
	{
		//���ڴ��������β
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