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
	if(nThread > 1024) nThread = 1024;
	if(nThread < 1) nThread = 1;

	m_nThred = nThread;
	unsigned int uiID;
	int igroup = (m_nThred / 64);
	int idiff = (m_nThred % 64) == 0 ? 0 : 1;
	igroup += idiff;
	
	do{
		EVENTITEMS events;
		memset(events.hEventArray,0,sizeof(events.hEventArray));
		m_vEvent.push_back(events);
		igroup--;
	}while (igroup);

	for (int i=0;i<m_nThred;i++)
	{
		igroup = i / 64;//������ֵ
		int idx = i % 64;//���������±�
		//m_EventArray[i] = CreateEvent(NULL,TRUE,FALSE,NULL);
		m_vEvent.at(igroup).hEventArray[idx] = CreateEvent(NULL,TRUE,FALSE,NULL);
		WORKTHREADITEM* pWorkItem = new WORKTHREADITEM;
		pWorkItem->igroup = igroup;
		pWorkItem->idx = idx;
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
	//DWORD dwRet = WaitForMultipleObjectsEx(m_nThred, m_EventArray, TRUE, INFINITE, FALSE);
	int igroup = m_vEvent.size(); //�ܹ�������event
	int isum = m_nThred;
	
	do{
		int idx = igroup - 1;//m_vEvent����ֵ
		int icount = isum - idx*64;
		isum -= icount;
		WaitForMultipleObjectsEx(icount ,m_vEvent.at(idx).hEventArray, TRUE, INFINITE, FALSE);
		igroup--;
	}while(igroup); //�ȴ����е��߳��˳�
	//WaitForMultipleObjectsEx();

	for (int i = 0; i < m_nThred; i++)
	{
		//CloseHandle(m_EventArray[i]);
		int igroup = i / 64;
		int idx = i % 64;//�����±�
		CloseHandle(m_vEvent.at(igroup).hEventArray[idx]);
	}

	m_nThred = 0;
	m_IdleThreadList.clear();
	m_BusyThreadList.clear();
	m_vEvent.clear();
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
				if ( pWorkItem->hUserEvent )
				{
					SetEvent( pWorkItem->hUserEvent ); //����û��������¼�,֪ͨ�û��Ѵ������
				}
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
	//SetEvent(pTaskPool->m_EventArray[pWorkItem->idx]);
	SetEvent(pTaskPool->m_vEvent.at(pWorkItem->igroup).hEventArray[pWorkItem->idx]);
	delete pWorkItem;
	
	return 0;
}

//procfun ����ָ�� lpParam�����б�ָ��
int CTaskPool::SubmitTaskItem( PROCFUN procfun , void* lpParam , HANDLE hUserEvent )
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
		pWorkItem->hUserEvent = NULL;
		if ( hUserEvent )
		{
			DuplicateHandle( GetCurrentProcess() , hUserEvent , GetCurrentProcess() , 
					&(pWorkItem->hUserEvent) , 0 , TRUE , DUPLICATE_SAME_ACCESS );//��ͬ���̸��ƾ��
			ResetEvent( pWorkItem->hUserEvent );
		}

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