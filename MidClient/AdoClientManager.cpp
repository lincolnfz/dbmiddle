/*
客户端数据集管理类
author: lincoln
email: lincolnfz@gmail.com
*/
#include "StdAfx.h"
#include "AdoClientManager.h"
#include "AdoClient.h"
#include "Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

CAdoClientManager* pAdoClientManager;
CAdoClientManager::CAdoClientManager(void):m_sock(this)
{
	pAdoClientManager = this;
	m_hSendThread = NULL;
	m_hEvent = NULL;
	//unsigned int iID;
	//m_hSendThread = (HANDLE)_beginthreadex(NULL,0,SendThread,this,0,&iID);
	//m_hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
}

CAdoClientManager::~CAdoClientManager(void)
{
}

void CAdoClientManager::init()
{
	unsigned int iID;

	if(m_hEvent == NULL)
		m_hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);

	if(m_hSendThread == NULL)
		m_hSendThread = (HANDLE)_beginthreadex(NULL,0,SendThread,this,0,&iID);
	
}

void CAdoClientManager::CompleteSend(DWORD id)
{
	m_mutex.lock();
	ADOMAP::iterator it;
	it = pAdoClientManager->m_adomap.find(id);
	it->second.adonetstate = NETIDLE;
	m_mutex.unlock();
	QueueUserAPC(NotefyCompSendProc,m_hSendThread,NULL);
}

void CAdoClientManager::NotefyCompSendProc(ULONG_PTR dwParam)
{

}

//发送线程，负责把数据包提交到网络层
unsigned int CAdoClientManager::SendThread(LPVOID lpParameter)
{
	ADOMAP::iterator it;
	BOOL bComplete = TRUE;
	while (true)
	{
		//SleepEx(INFINITE,true);
		DWORD dwRet = WaitForSingleObjectEx(pAdoClientManager->m_hEvent,INFINITE,TRUE);
		if(dwRet == WAIT_OBJECT_0 && bComplete)
		{
			bComplete = FALSE;
			DATAQUEUE::reference rf = pAdoClientManager->m_sendQueue.front();
			BUFINFO bufinfo = rf;
			pAdoClientManager->m_sendQueue.pop_front();//释放已投递的消息
			if(pAdoClientManager->m_sock.SendData(bufinfo.id, bufinfo.buf, bufinfo.buflen) == false)
			{
				//m_mutex.lock();
				it = pAdoClientManager->m_adomap.find(bufinfo.id);
				if(it != pAdoClientManager->m_adomap.end())
				{
					it->second.adonetstate = NETIDLE;
				}
				//m_mutex.unlock();
			}
			ResetEvent(pAdoClientManager->m_hEvent);
		}
		else if(dwRet = WAIT_IO_COMPLETION)
		{
			bComplete = true;
			if (pAdoClientManager->m_sendQueue.empty() == false)
			{
				SetEvent(pAdoClientManager->m_hEvent);
			}

		}
		else
		{
			ResetEvent(pAdoClientManager->m_hEvent);
		}
	}
	return 0;
}

eNETSTATE CAdoClientManager::sendAdoData(CAdoClient* padoclient,PACK_ADO& pack)
{
	//m_adomap.insert(pair<DWORD,CAdoClient*>(padoclient->getID(),padoclient));
	ADOMAP::iterator it;
	m_mutex.lock();
	DWORD id = padoclient->getID();
	it = m_adomap.find(id);
	if (it == m_adomap.end())
	{
		ADOCLIENT adoclient;
		adoclient.padoclient = padoclient;
		adoclient.adonetstate = NETIDLE;
		pair<ADOMAP::iterator,bool> ins;
		ins = m_adomap.insert(ADOMAP::value_type(id,adoclient));
		it = ins.first;
	}
	

	if(it->second.adonetstate == NETBUSY)
	{
		m_mutex.unlock();
		return NETBUSY;
	}
	it->second.adonetstate = NETBUSY;
	m_mutex.unlock();

	BUFINFO buf;
	buf.id = id;
	char* pbuf = NULL;
	buf.buflen = CUtil::serialAdopack(&pbuf,pack);
	buf.buf = pbuf;

	//数据放入发送队列
	m_mutex.lock();
	m_sendQueue.push_back(buf);
	m_mutex.unlock();
	
	SetEvent(m_hEvent); //有新的数据包，请求网络发送线程处理

	return NETOK;
}

void CAdoClientManager::recvAdoData(DWORD id, long buflen, char* buf)
{
	m_mutex.lock();
	ADOMAP::iterator it;
	it = m_adomap.find(id);
	m_mutex.unlock();

	if(it == m_adomap.end())
	{
		releaseBuf(buf);
		return;
	}

	RECVADOBUF* pRecvAdo = new RECVADOBUF;
	pRecvAdo->pAdoClient = it->second.padoclient;
	pRecvAdo->len = buflen;
	pRecvAdo->pbuf = buf;
	QueueUserWorkItem(CAdoClient::ThreadRecvProc,pRecvAdo,WT_EXECUTEDEFAULT);
}

BOOL CAdoClientManager::releaseBuf(char* buf)
{
	return m_sock.releaseBuf(buf);
}

void CAdoClientManager::SetProxy(std::string proxyip,int proxyport,PROXYTYPE pt,
				std::string proxyUsernam,std::string proxyPassword)
{
	m_sock.SetProxy(proxyip,proxyport,pt,proxyUsernam,proxyPassword);
}

bool CAdoClientManager::ConnectDBProvider(std::string serverip,int port)
{
	init();
	m_sock.SetServer(serverip,port);
	return (m_sock.Connect());
}

void CAdoClientManager::SetDBProvider(std::string serverip,int port)
{
	init();
	m_sock.SetServer(serverip,port);
}