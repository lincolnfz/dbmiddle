/*
CAdoProvider 数据集提供类
author: lincolnfz
lincolnfz@gmail.com
*/
#pragma once

#include "CommonHead.h"
#include "Ado.h"
#include "IOCPSockSrv.h"
#include "TaskPool.h"

class CAdoProvider
{
public:
	CAdoProvider();
	~CAdoProvider();

	void Init();
	BOOL Stop();
	BOOL StartSrv(CString dbsrc, CString dbname, CString user, CString pass, long lOptions = adConnectUnspecified);
	BOOL NotifyRecvPack(SOCKET sock,DWORD id,char* data,long len);
	static void* CreateConn();
	static DWORD RequestProc(void* lpParam , void* funContext);
	static void Clean(void* funContext);
	void setConnectionPool(int nNum);
	void setNetAttrib(int port,int maxSend);

	//设定工作模式,true阻塞, false非阻塞
	void setWorkMode(bool block);

protected:
	CTaskPool m_TaskPool;
	CIOCPSockSrv m_iocpsrv;
	CString m_dbsrc,m_dbname,m_dbuser,m_dbpass;
	long m_ldbOptions;
	DWORD m_nAdoThread; //联接池的数量
	HANDLE m_hActive;
	bool m_bRun;
	bool m_block;

	void waitForComplete();

};
