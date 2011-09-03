/*
CAdoProvider ���ݼ��ṩ��
author: lincolnfz
lincolnfz@gmail.com
*/
#pragma once

#include "../common/CommonHead.h"
#include "../common/Ado.h"
#include "IOCPSockSrv.h"
#include "../common/TaskPool.h"

class CAdoProvider
{
public:
	CAdoProvider();
	~CAdoProvider();

	void Init();
	BOOL Stop();
	BOOL StartSrv(CString dbsrc, CString sqlport, CString dbname, CString user, CString pass, long lOptions = adConnectUnspecified);
	BOOL NotifyRecvPack(SOCKET sock,DWORD id,char* data,long len);
	static void* CALLBACK CreateConn();
	static DWORD CALLBACK RequestProc(void* lpParam , void* funContext);
	static void CALLBACK Clean(void* funContext);
	static DWORD CALLBACK DoRequest(void* lpParam , void* funContext);
	void setConnectionPool(int nNum);
	void setNetAttrib(int port,int maxSend);

	//�趨����ģʽ,true����, false������
	void setWorkMode(bool block);

	BOOL NotifyRequest( PROCITEM* );

protected:
	CTaskPool m_TaskPool;
	CIOCPSockSrv m_iocpsrv;
	CString m_dbsrc,m_dbname,m_dbuser,m_dbpass, m_sqlport;
	long m_ldbOptions;
	DWORD m_nAdoThread; //���ӳص�����
	HANDLE m_hActive;
	bool m_bRun;
	bool m_block;

	void waitForComplete();

};
