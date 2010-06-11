/*
CAdoProvider 数据集提供类
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

	// 1实例化一个layout 对象
	infolayout = new log4cpp::BasicLayout();
	//warnlayout = new log4cpp::BasicLayout();
	//errlayout = new log4cpp::BasicLayout();

	infoappender = new log4cpp::FileAppender("FileAppender",logpath.GetBuffer());
	infoappender->setLayout(infolayout);

	/*warnappender = new log4cpp::FileAppender("FileAppender","warn.log");
	warnappender->setLayout(warnlayout);
	
	// 2. 初始化一个appender 对象
	errappender = new log4cpp::FileAppender("FileAppender","error.log");
	// 3. 把layout对象附着在appender对象上
	errappender->setLayout(errlayout);*/

	log4cpp::Category& info_log = log4cpp::Category::getInstance("info");
	info_log.setAdditivity(false);
	info_log.setAppender(infoappender);
	info_log.setPriority(log4cpp::Priority::INFO);

	// 4. 实例化一个category对象
	/*log4cpp::Category& warn_log = log4cpp::Category::getInstance("warn");
	// 5. 设置additivity为false，替换已有的appender
	warn_log.setAdditivity(false);
	// 5. 把appender对象附到category上
	warn_log.setAppender(errappender);
	// 6. 设置category的优先级，低于此优先级的日志不被记录
	warn_log.setPriority(log4cpp::Priority::WARN);

	log4cpp::Category& err_log = log4cpp::Category::getInstance("err");
	err_log.setAdditivity(false);
	err_log.setAppender(errappender);
	err_log.setPriority(log4cpp::Priority::ERROR);*/

}

BOOL CAdoProvider::StartSrv(CString dbsrc, CString sqlport, CString dbname, CString user, CString pass, long lOptions)
{
	//设置数据库联接参数
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

	//sock服务器开始运行
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

//通知线程处理新的数据包
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

//供线程池调用初始函数
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
		info_log.error("%d线程:数据库联接失败",dwThreadID);
		return pFunContext;
	}*/
	info_log.info("%d线程:创建成功",dwThreadID);

	pAdoRecord->SetAdoConnection(pAdoConn);
	return pFunContext;
}

//供线程池调用清理函数
void CAdoProvider::Clean(void* funContext)
{
	ADOCONTEXT* pAdoContext = (ADOCONTEXT*)funContext;
	pAdoContext->pAdoConnect->Close();
	delete pAdoContext->pAdoRecord;
	delete pAdoContext->pAdoConnect;
	delete pAdoContext;
	::CoUninitialize();
}

//供线程池调用处理函数
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
			info_log.error("重联数据库失败:%d",packinfo->sock);
			pCAdoProvider->m_iocpsrv.ReleaseDataPack(packinfo->databuf);
			delete packinfo;
			return 0;
		}
	}*/

	if(pAdoConn->ConnectMysql(pCAdoProvider->m_dbsrc, pCAdoProvider->m_sqlport, pCAdoProvider->m_dbname,
		pCAdoProvider->m_dbuser, pCAdoProvider->m_dbpass, pCAdoProvider->m_ldbOptions)==FALSE)
	{
		info_log.error("%d线程:数据库联接失败", GetCurrentThreadId() );
		return 0;
	}

	PACK_ADO packado;
	//收到的内容反序列化为PACK_ADO对像
	char* pUncomprData = CsuUtil::paserAdopack(packinfo->databuf, packinfo->datalen , packado);

	//释放网络层所创建的接收buf
	pCAdoProvider->m_iocpsrv.ReleaseDataPack(packinfo->databuf);

	if (packado.adotype == PACK_ADO.ADO_EXECUTE)
	{		
		if(pAdoRecord->SetRecordset(pAdoConn->Execute(packado.data)))
		{
			//info_log.info("执行ADO_EXECUTE成功:%d",packinfo->sock);
			//成功执行sql语句
			pAdoRecord->SaveToBuffer(packado.data,packado.datalen);
			packado.result = RESULT_SUCESS;
		}
		else
		{
			info_log.error("执行ADO_EXECUTE失败:%d",packinfo->sock);
			//sql语句执行失败
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
				//info_log.info("执行UpdateBatch成功:%d",packinfo->sock);
				//更新成功
				packado.result = RESULT_SUCESS;
				packado.datalen = 0;
				packado.data = NULL;
			}
			else
			{
				info_log.error("执行失败失败:%d",packinfo->sock);
				//更新失败
				packado.result = RESULT_FAIL;
				packado.datalen = 0;
				packado.data = NULL;
			}
		}
		else
		{
			info_log.error("载入UpdateBatch失败:%d",packinfo->sock);
			//无效的客户端数据
			packado.result = RESULT_FAIL;
			packado.datalen = 0;
			packado.data = NULL;
		}
	}
	else
	{
		info_log.error("无效的客户请求:%d",packinfo->sock);
		//客户端收来的无效的数据包

		//释放网络层所创建的接收buf
		//pCAdoProvider->m_iocpsrv.ReleaseDataPack(packinfo->databuf);
		delete packinfo;
		return 0;
	}

	//以下开始处理有效数据包的发送操作

	//释放网络层所创建的接收buf
	//pCAdoProvider->m_iocpsrv.ReleaseDataPack(packinfo->databuf);
	delete []pUncomprData; //释放解压包

	char* sendbuf;
	int ilen = CsuUtil::serialAdopack(&sendbuf,packado);
	delete[] packado.data; //释放ado产生的数据

	//向网络层发送数据
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
	//等待服务运行完成
	WaitForSingleObject(m_hActive,INFINITE);
	CloseHandle(m_hActive);
	m_hActive = NULL;
}

void CAdoProvider::setWorkMode(bool block)
{
	m_block = block;
}