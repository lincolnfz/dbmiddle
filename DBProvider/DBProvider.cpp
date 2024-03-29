// DBProvider.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "AdoProvider.h"

//定义全局函数变量
void Init();
BOOL IsInstalled();
BOOL Install();
BOOL Uninstall();
void LogEvent(LPCTSTR pszFormat, ...);
void WINAPI ServiceMain();
void WINAPI ServiceStrl(DWORD dwOpcode);
BOOL ReadXml(LPCTSTR xmlfile);
void StartMainTask(bool block=false);

TCHAR szAppDir[MAX_PATH];
TCHAR szServiceName[] = {_T("DBProvider")};
TCHAR szSqlPort[10]={_T("3306")};
BOOL bInstall;
SERVICE_STATUS_HANDLE hServiceStatus;
SERVICE_STATUS status;
DWORD dwThreadID;
TCHAR szSqlsrv[128],szSqldb[128],szSqluser[128],szSqlpw[128],szDepend[128];
int nPort = 10003;
int nDbConn = 1;//联接池大小
int nMaxSend = 1;//最大并发投递量

CAdoProvider adoprovider;

void StartMainTask(bool block)
{
	adoprovider.Init();
	adoprovider.setConnectionPool(nDbConn);
	adoprovider.setNetAttrib(nPort,nMaxSend);
	adoprovider.setWorkMode(block);
	adoprovider.StartSrv(szSqlsrv,szSqlPort,szSqldb,szSqluser,szSqlpw);
}

int _tmain(int argc, _TCHAR* argv[])
{
	GetModuleFileName(NULL,szAppDir,MAX_PATH);	//获取当前目录
	char *p = strrchr(szAppDir, '\\');
	if (p)
		*(p + 1) = '\0';

	Init();

	CString xmlpath(szAppDir);
	xmlpath.Append("AppOption.xml");

	dwThreadID = ::GetCurrentThreadId();

#ifdef _DEBUG
	//ReadXml(xmlpath);
	//ServiceMain();
#endif

	SERVICE_TABLE_ENTRY st[] =
	{
		{ szServiceName, (LPSERVICE_MAIN_FUNCTION)ServiceMain },
		{ NULL, NULL }
	};

	if (strcmp(argv[argc-1], "/install") == 0)
	{
		if(!ReadXml(xmlpath))
		{
			LogEvent(_T("can not set Depend!"));
		}
		Install();
	}
	else if (strcmp(argv[argc-1], "/uninstall") == 0)
	{
		Uninstall();
	}
	else if (strcmp(argv[argc-1], "/run") == 0)
	{
		if(ReadXml(xmlpath))
		{
			StartMainTask(true);
		}
	}
	else
	{
		if(ReadXml(xmlpath))
		{
			if (!::StartServiceCtrlDispatcher(st))
			{
				LogEvent(_T("Register Service Main Function Error!"));
				
			}
		}
		else
		{
			LogEvent(_T("Miss AppOption.xml Error!"));
		}
	}

	return 0;
}

//*********************************************************
//Functiopn:			Init
//Description:			初始化
//Calls:				main
//Called By:				
//Table Accessed:				
//Table Updated:				
//Input:				
//Output:				
//Return:				
//Others:				
//History:				
//			<author>niying <time>2006-8-10		<version>		<desc>
//*********************************************************
void Init()
{
	hServiceStatus = NULL;
	status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	status.dwCurrentState = SERVICE_STOPPED;
	status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	status.dwWin32ExitCode = 0;
	status.dwServiceSpecificExitCode = 0;
	status.dwCheckPoint = 0;
	status.dwWaitHint = 0;
}

//*********************************************************
//Functiopn:			ServiceMain
//Description:			服务主函数，这在里进行控制对服务控制的注册
//Calls:
//Called By:
//Table Accessed:
//Table Updated:
//Input:
//Output:
//Return:
//Others:
//History:
//			<author>niying <time>2006-8-10		<version>		<desc>
//*********************************************************
void WINAPI ServiceMain()
{
	// Register the control request handler
	status.dwCurrentState = SERVICE_START_PENDING;
	status.dwControlsAccepted = SERVICE_ACCEPT_STOP;

	//注册服务控制
	hServiceStatus = RegisterServiceCtrlHandler(szServiceName, ServiceStrl);
	if (hServiceStatus == NULL)
	{
		LogEvent(_T("Handler not installed"));
		return;
	}
	SetServiceStatus(hServiceStatus, &status);

	status.dwWin32ExitCode = S_OK;
	status.dwCheckPoint = 0;
	status.dwWaitHint = 0;
	status.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(hServiceStatus, &status);

	//note 应用时将主要任务放于此即可
	StartMainTask(true);
	//

	status.dwCurrentState = SERVICE_STOPPED;
	SetServiceStatus(hServiceStatus, &status);
	LogEvent(_T("Service stopped"));
}

//*********************************************************
//Functiopn:			ServiceStrl
//Description:			服务控制主函数，这里实现对服务的控制，
//						当在服务管理器上停止或其它操作时，将会运行此处代码
//Calls:
//Called By:
//Table Accessed:
//Table Updated:
//Input:				dwOpcode：控制服务的状态
//Output:
//Return:
//Others:
//History:
//			<author>niying <time>2006-8-10		<version>		<desc>
//*********************************************************
void WINAPI ServiceStrl(DWORD dwOpcode)
{
	switch (dwOpcode)
	{
	case SERVICE_CONTROL_STOP:
		adoprovider.Stop();
		status.dwCurrentState = SERVICE_STOP_PENDING;
		SetServiceStatus(hServiceStatus, &status);
		PostThreadMessage(dwThreadID, WM_CLOSE, 0, 0);
		break;
	case SERVICE_CONTROL_PAUSE:
		break;
	case SERVICE_CONTROL_CONTINUE:
		break;
	case SERVICE_CONTROL_INTERROGATE:
		break;
	case SERVICE_CONTROL_SHUTDOWN:
		break;
	default:
		LogEvent(_T("Bad service request"));
	}
}
//*********************************************************
//Functiopn:			IsInstalled
//Description:			判断服务是否已经被安装
//Calls:
//Called By:
//Table Accessed:
//Table Updated:
//Input:
//Output:
//Return:
//Others:
//History:
//			<author>niying <time>2006-8-10		<version>		<desc>
//*********************************************************
BOOL IsInstalled()
{
	BOOL bResult = FALSE;

	//打开服务控制管理器
	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (hSCM != NULL)
	{
		//打开服务
		SC_HANDLE hService = ::OpenService(hSCM, szServiceName, SERVICE_QUERY_CONFIG);
		if (hService != NULL)
		{
			bResult = TRUE;
			::CloseServiceHandle(hService);
		}
		::CloseServiceHandle(hSCM);
	}
	return bResult;
}

//*********************************************************
//Functiopn:			Install
//Description:			安装服务函数
//Calls:
//Called By:
//Table Accessed:
//Table Updated:
//Input:
//Output:
//Return:
//Others:
//History:
//			<author>niying <time>2006-8-10		<version>		<desc>
//*********************************************************
BOOL Install()
{
	if (IsInstalled())
		return TRUE;

	//打开服务控制管理器
	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCM == NULL)
	{
		MessageBox(NULL, _T("Couldn't open service manager"), szServiceName, MB_OK);
		return FALSE;
	}

	// Get the executable file path
	TCHAR szFilePath[MAX_PATH];
	::GetModuleFileName(NULL, szFilePath, MAX_PATH);

	//创建服务,并设置依赖关系
	SC_HANDLE hService = ::CreateService(
		hSCM, szServiceName, szServiceName,
		SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,
		SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL,
		szFilePath, NULL, NULL, szDepend, NULL, NULL);

	if (hService == NULL)
	{
		::CloseServiceHandle(hSCM);
		MessageBox(NULL, _T("Couldn't create service"), szServiceName, MB_OK);
		return FALSE;
	}

	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);
	return TRUE;
}

//*********************************************************
//Functiopn:			Uninstall
//Description:			删除服务函数
//Calls:
//Called By:
//Table Accessed:
//Table Updated:
//Input:
//Output:
//Return:
//Others:
//History:
//			<author>niying <time>2006-8-10		<version>		<desc>
//*********************************************************
BOOL Uninstall()
{
	if (!IsInstalled())
		return TRUE;

	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (hSCM == NULL)
	{
		MessageBox(NULL, _T("Couldn't open service manager"), szServiceName, MB_OK);
		return FALSE;
	}

	SC_HANDLE hService = ::OpenService(hSCM, szServiceName, SERVICE_STOP | DELETE);

	if (hService == NULL)
	{
		::CloseServiceHandle(hSCM);
		MessageBox(NULL, _T("Couldn't open service"), szServiceName, MB_OK);
		return FALSE;
	}
	SERVICE_STATUS status;
	::ControlService(hService, SERVICE_CONTROL_STOP, &status);

	//删除服务
	BOOL bDelete = ::DeleteService(hService);
	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);

	if (bDelete)
		return TRUE;

	LogEvent(_T("Service could not be deleted"));
	return FALSE;
}

//*********************************************************
//Functiopn:			LogEvent
//Description:			记录服务事件
//Calls:
//Called By:
//Table Accessed:
//Table Updated:
//Input:
//Output:
//Return:
//Others:
//History:
//			<author>niying <time>2006-8-10		<version>		<desc>
//*********************************************************
void LogEvent(LPCTSTR pFormat, ...)
{
	TCHAR    chMsg[256];
	HANDLE  hEventSource;
	LPTSTR  lpszStrings[1];
	va_list pArg;

	va_start(pArg, pFormat);
	_vstprintf(chMsg, pFormat, pArg);
	va_end(pArg);

	lpszStrings[0] = chMsg;

	hEventSource = RegisterEventSource(NULL, szServiceName);
	if (hEventSource != NULL)
	{
		ReportEvent(hEventSource, EVENTLOG_INFORMATION_TYPE, 0, 0, NULL, 1, 0, (LPCTSTR*) &lpszStrings[0], NULL);
		DeregisterEventSource(hEventSource);
	}
}

BOOL ReadXml(LPCTSTR xmlfile)
{
	BOOL bRet = FALSE;
	const TCHAR* pbuf = NULL;
	if(-1 == _access(xmlfile,00))
	{
		return bRet;
	}
	XMLNode xOptionNode = XMLNode::openFileHelper(xmlfile,"option");
	
	XMLNode xSqlsrv = xOptionNode.getChildNode("sqlsrv");
	strcpy(szSqlsrv,xSqlsrv.getText());

	XMLNode xSqlPort = xOptionNode.getChildNode("sqlport");
	strcpy(szSqlPort,xSqlPort.getText());

	XMLNode xSqldb = xOptionNode.getChildNode("sqldb");
	strcpy(szSqldb,xSqldb.getText());

	XMLNode xSqluser = xOptionNode.getChildNode("sqluser");
	strcpy(szSqluser,xSqluser.getText());

	XMLNode xSqlpw = xOptionNode.getChildNode("sqlpw");
	if( xSqlpw.getText() )
		strcpy(szSqlpw,xSqlpw.getText());

	char szdbconn[10]={0};
	XMLNode xdbconn = xOptionNode.getChildNode("dbconn");
	strcpy(szdbconn,xdbconn.getText());
	nDbConn = atoi(szdbconn);

	char szport[10]={0};
	XMLNode xport = xOptionNode.getChildNode("port");
	strcpy(szport,xport.getText());
	nPort = atoi(szport);

	char szmaxsend[10]={0};
	XMLNode xmaxsend = xOptionNode.getChildNode("maxsend");
	strcpy(szmaxsend,xmaxsend.getText());
	nMaxSend = atoi(szmaxsend);

	XMLNode xDepend = xOptionNode.getChildNode("depend");
	pbuf = xDepend.getText();
	if( pbuf )
	{
		strcpy(szDepend, pbuf);
	}

	bRet = TRUE;
	return bRet;
}