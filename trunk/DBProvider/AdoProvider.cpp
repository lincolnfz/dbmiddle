/*
CAdoProvider ���ݼ��ṩ��
author: lincolnfz
lincolnfz@gmail.com
*/

#include "StdAfx.h"
#include "adoprovider.h"
//#include "jsonlib/json.h"
#include "base64.h"
#include "suUtil.h"
#include "log4cpp/Category.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/BasicLayout.hh"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

log4cpp::Layout* infolayout;
log4cpp::Layout* warnlayout;
log4cpp::Layout* errlayout;

log4cpp::Appender* infoappender;
log4cpp::Appender* warnappender;
log4cpp::Appender* errappender;

CAdoProvider* pCAdoProvider;
CAdoProvider::CAdoProvider(void)
{
	m_iocpsrv.SetAdoProviderPtr(this);
	pCAdoProvider = this;
	m_nAdoThread = 1;
	m_hActive = NULL;
	m_bRun = false;
	m_block = false;
}

CAdoProvider::~CAdoProvider(void)
{
	Stop();
}

void CAdoProvider::Init()
{
	CString logpath = CsuUtil::getAppDir();
	logpath.Append("info.log");

	m_ldbOptions = 0;

	// 1ʵ����һ��layout ����
	infolayout = new log4cpp::BasicLayout();
	//warnlayout = new log4cpp::BasicLayout();
	//errlayout = new log4cpp::BasicLayout();

	infoappender = new log4cpp::FileAppender("FileAppender",logpath.GetBuffer());
	infoappender->setLayout(infolayout);

	/*warnappender = new log4cpp::FileAppender("FileAppender","warn.log");
	warnappender->setLayout(warnlayout);
	
	// 2. ��ʼ��һ��appender ����
	errappender = new log4cpp::FileAppender("FileAppender","error.log");
	// 3. ��layout��������appender������
	errappender->setLayout(errlayout);*/

	log4cpp::Category& info_log = log4cpp::Category::getInstance("info");
	info_log.setAdditivity(false);
	info_log.setAppender(infoappender);
	info_log.setPriority(log4cpp::Priority::INFO);

	// 4. ʵ����һ��category����
	/*log4cpp::Category& warn_log = log4cpp::Category::getInstance("warn");
	// 5. ����additivityΪfalse���滻���е�appender
	warn_log.setAdditivity(false);
	// 5. ��appender���󸽵�category��
	warn_log.setAppender(errappender);
	// 6. ����category�����ȼ������ڴ����ȼ�����־������¼
	warn_log.setPriority(log4cpp::Priority::WARN);

	log4cpp::Category& err_log = log4cpp::Category::getInstance("err");
	err_log.setAdditivity(false);
	err_log.setAppender(errappender);
	err_log.setPriority(log4cpp::Priority::ERROR);*/

}

BOOL CAdoProvider::StartSrv(CString dbsrc, CString sqlport, CString dbname, CString user, CString pass, long lOptions)
{
	//�������ݿ����Ӳ���
	m_dbsrc = dbsrc;
	m_sqlport = sqlport;
	m_dbname = dbname;
	m_dbuser = user;
	m_dbpass = pass;
	m_ldbOptions = lOptions;

	SYSTEM_INFO systeminfo;
	GetSystemInfo(&systeminfo);
	DWORD nkernel = systeminfo.dwNumberOfProcessors;

	m_TaskPool.Init(m_nAdoThread,CAdoProvider::CreateConn,CAdoProvider::Clean);

	//sock��������ʼ����
	m_iocpsrv.Run(nkernel);

	m_hActive = CreateEvent(NULL,TRUE,FALSE,NULL);
	m_bRun = true;

	if(m_block)
	{
		waitForComplete();
	}

	return TRUE;
}

BOOL CAdoProvider::Stop()
{
	if(m_bRun == false)
		return TRUE;

	m_iocpsrv.Stop();
	m_TaskPool.StopAll();
	SetEvent(m_hActive);

	if(m_block == false)
	{
		waitForComplete();
	}

	m_bRun = false;

	return TRUE;
}

//֪ͨ�̴߳����µ����ݰ�
BOOL CAdoProvider::NotifyRecvPack(SOCKET sock,DWORD id,char* data,long len)
{
	PACK_INFO* packinfo = new PACK_INFO;
	packinfo->sock = sock;
	packinfo->id = id;
	packinfo->databuf = data;
	packinfo->datalen = len;
	packinfo->time = GetTickCount();

	m_TaskPool.SubmitTaskItem(CAdoProvider::RequestProc,packinfo);

	return TRUE;
}

//���̳߳ص��ó�ʼ����
void* CAdoProvider::CreateConn()
{
	::CoInitialize(NULL);
	log4cpp::Category& info_log = log4cpp::Category::getInstance("info");
	DWORD dwThreadID = GetCurrentThreadId();
	ADOCONTEXT* pFunContext = new ADOCONTEXT;

	CAdoConnection* pAdoConn = new CAdoConnection();
	CAdoRecordSet* pAdoRecord = new CAdoRecordSet();

	pFunContext->pAdoConnect = pAdoConn;
	pFunContext->pAdoRecord = pAdoRecord;

	/*if(pAdoConn->ConnectMysql(pCAdoProvider->m_dbsrc, pCAdoProvider->m_sqlport, pCAdoProvider->m_dbname,
		pCAdoProvider->m_dbuser, pCAdoProvider->m_dbpass, pCAdoProvider->m_ldbOptions)==FALSE)
	{
		info_log.error("%d�߳�:���ݿ�����ʧ��",dwThreadID);
		return pFunContext;
	}*/
	info_log.info("%d�߳�:�����ɹ�",dwThreadID);

	pAdoRecord->SetAdoConnection(pAdoConn);
	return pFunContext;
}

//���̳߳ص���������
void CAdoProvider::Clean(void* funContext)
{
	ADOCONTEXT* pAdoContext = (ADOCONTEXT*)funContext;
	pAdoContext->pAdoConnect->Close();
	delete pAdoContext->pAdoRecord;
	delete pAdoContext->pAdoConnect;
	delete pAdoContext;
	::CoUninitialize();
}

//���̳߳ص��ô�����
DWORD CAdoProvider::RequestProc(/*in*/void* lpParam , /*out*/void* funContext)
{
	log4cpp::Category& info_log = log4cpp::Category::getInstance("info");
	ADOCONTEXT* pFunContext = (ADOCONTEXT*)funContext;
	PACK_INFO* packinfo = (PACK_INFO*)lpParam;

	CAdoConnection* pAdoConn = pFunContext->pAdoConnect;
	CAdoRecordSet* pAdoRecord = pFunContext->pAdoRecord;

	DWORD nowTime = GetTickCount();
	BOOL bExist = TRUE;
	if ((nowTime - packinfo->time) >= TIME_OUT)
	{
		bExist = pCAdoProvider->m_iocpsrv.ExistSock(packinfo->sock);
	}
	if(bExist == FALSE)
	{
		pCAdoProvider->m_iocpsrv.ReleaseDataPack(packinfo->databuf);
		delete packinfo;
		return 0;
	}

	/*if(pAdoConn->IsOpen() == FALSE)
	{
		if(pAdoConn->ReConnect() == FALSE)
		{
			info_log.error("�������ݿ�ʧ��:%d",packinfo->sock);
			pCAdoProvider->m_iocpsrv.ReleaseDataPack(packinfo->databuf);
			delete packinfo;
			return 0;
		}
	}*/

	if(pAdoConn->ConnectMysql(pCAdoProvider->m_dbsrc, pCAdoProvider->m_sqlport, pCAdoProvider->m_dbname,
		pCAdoProvider->m_dbuser, pCAdoProvider->m_dbpass, pCAdoProvider->m_ldbOptions)==FALSE)
	{
		info_log.error("%d�߳�:���ݿ�����ʧ��", GetCurrentThreadId() );
		return 0;
	}

	PACK_ADO packado;
	//�յ������ݷ����л�ΪPACK_ADO����
	char* pUncomprData = CsuUtil::paserAdopack(packinfo->databuf, packinfo->datalen , packado);

	//�ͷ�������������Ľ���buf
	pCAdoProvider->m_iocpsrv.ReleaseDataPack(packinfo->databuf);

	if (packado.adotype == PACK_ADO.ADO_EXECUTE)
	{		
		if(pAdoRecord->SetRecordset(pAdoConn->Execute(packado.data)))
		{
			//info_log.info("ִ��ADO_EXECUTE�ɹ�:%d",packinfo->sock);
			//�ɹ�ִ��sql���
			pAdoRecord->SaveToBuffer(packado.data,packado.datalen);
			packado.result = RESULT_SUCESS;
		}
		else
		{
			info_log.error("ִ��ADO_EXECUTEʧ��:%d",packinfo->sock);
			//sql���ִ��ʧ��
			packado.result = RESULT_FAIL;
			packado.datalen = 0;
			packado.data = NULL;						
		}
	}
	else if(packado.adotype == PACK_ADO.ADO_UPDATE)
	{
		if(pAdoRecord->LoadBuffer(packado.data, packado.datalen))
		{
			if(pAdoRecord->UpdateBatch())
			{
				//info_log.info("ִ��UpdateBatch�ɹ�:%d",packinfo->sock);
				//���³ɹ�
				packado.result = RESULT_SUCESS;
				packado.datalen = 0;
				packado.data = NULL;
			}
			else
			{
				info_log.error("ִ��ʧ��ʧ��:%d",packinfo->sock);
				//����ʧ��
				packado.result = RESULT_FAIL;
				packado.datalen = 0;
				packado.data = NULL;
			}
		}
		else
		{
			info_log.error("����UpdateBatchʧ��:%d",packinfo->sock);
			//��Ч�Ŀͻ�������
			packado.result = RESULT_FAIL;
			packado.datalen = 0;
			packado.data = NULL;
		}
	}
	else
	{
		info_log.error("��Ч�Ŀͻ�����:%d",packinfo->sock);
		//�ͻ�����������Ч�����ݰ�

		//�ͷ�������������Ľ���buf
		//pCAdoProvider->m_iocpsrv.ReleaseDataPack(packinfo->databuf);
		delete packinfo;
		return 0;
	}

	//���¿�ʼ������Ч���ݰ��ķ��Ͳ���

	//�ͷ�������������Ľ���buf
	//pCAdoProvider->m_iocpsrv.ReleaseDataPack(packinfo->databuf);
	delete []pUncomprData; //�ͷŽ�ѹ��

	char* sendbuf;
	int ilen = CsuUtil::serialAdopack(&sendbuf,packado);
	delete[] packado.data; //�ͷ�ado����������

	//������㷢������
	packinfo->databuf = sendbuf;
	packinfo->datalen = ilen;
	pCAdoProvider->m_iocpsrv.SendAdo(packinfo->sock, packinfo->id, packinfo->databuf, packinfo->datalen, TRUE);

	delete packinfo;

	pAdoConn->Close();

	return 0;
}

void CAdoProvider::setConnectionPool(int nNum)
{
	if(nNum <= 0)
		nNum = 1;

	if(nNum > 64)
		nNum = 64;

	m_nAdoThread = nNum;
}

void CAdoProvider::setNetAttrib(int port,int maxSend)
{
	m_iocpsrv.SetListenPort(port);
	m_iocpsrv.setMaxSend(maxSend);
}

void CAdoProvider::waitForComplete()
{
	//�ȴ������������
	WaitForSingleObject(m_hActive,INFINITE);
	CloseHandle(m_hActive);
	m_hActive = NULL;
}

void CAdoProvider::setWorkMode(bool block)
{
	m_block = block;
}