
#include "stdafx.h"
#include <afxdisp.h>
#include "ado.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*########################################################################
------------------------------------------------
数值类型转换
------------------------------------------------
########################################################################*/
COleDateTime vartodate(const _variant_t& var)
{
	COleDateTime value;
	switch (var.vt) 
	{
	case VT_DATE:
		{
			value = var.date;
		}
		break;
	case VT_EMPTY:
	case VT_NULL:
		value.SetStatus(COleDateTime::null);
		break;
	default:
		value.SetStatus(COleDateTime::null);
		TRACE(_T("Warning: 未处理的 _variant_t 类型值; 文件: %s; 行: %d\n"), __FILE__, __LINE__);
	}
	return value;
}

COleCurrency vartocy(const _variant_t& var)
{
	COleCurrency value;
	switch (var.vt) 
	{
	case VT_CY:
		value = (CURRENCY)var.cyVal;
		break;
	case VT_EMPTY:
	case VT_NULL:
		value.m_status = COleCurrency::null;
		break;
	default:
		value.m_status = COleCurrency::null;
		TRACE(_T("Warning: 未处理的 _variant_t 类型值; 文件: %s; 行: %d\n"), __FILE__, __LINE__);
	}
	return value;
}

bool vartobool(const _variant_t& var)
{
	bool value = false;
	switch (var.vt)
	{
	case VT_BOOL:
		value = var.boolVal ? true : false;
	case VT_EMPTY:
	case VT_NULL:
		break;
	default:
		TRACE(_T("Warning: 未处理的 _variant_t 类型值; 文件: %s; 行: %d\n"), __FILE__, __LINE__);
	}
	return value;
}

BYTE vartoby(const _variant_t& var)
{
	BYTE value = 0;
	switch (var.vt)
	{
	case VT_I1:
	case VT_UI1:
		value = var.bVal;
		break;
	case VT_NULL:
	case VT_EMPTY:
		value = 0;
		break;
	default:
		TRACE(_T("Warning: 未处理的 _variant_t 类型值; 文件: %s; 行: %d\n"), __FILE__, __LINE__);
	}	
	return value;
}

short vartoi(const _variant_t& var)
{
	short value = 0;
	switch (var.vt)
	{
	case VT_BOOL:
		value = var.boolVal;
		break;
	case VT_UI1:
	case VT_I1:
		value = var.bVal;
		break;
	case VT_I2:
	case VT_UI2:
		value = var.iVal;
		break;
	case VT_NULL:
	case VT_EMPTY:
		value = 0;
		break;
	default:
		TRACE(_T("Warning: 未处理的 _variant_t 类型值; 文件: %s; 行: %d\n"), __FILE__, __LINE__);
	}	
	return value;
}

long vartol(const _variant_t& var)
{
	long value = 0;
	switch (var.vt)
	{
	case VT_BOOL:
		value = var.boolVal;
		break;
	case VT_UI1:
	case VT_I1:
		value = var.bVal;
		break;
	case VT_UI2:
	case VT_I2:
		value = var.iVal;
		break;
	case VT_I4:
	case VT_UI4:
		value = var.lVal;
		break;
	case VT_INT:
		value = var.intVal;
		break;
	case VT_R4:
		value = (long)(var.fltVal + 0.5);
		break;
	case VT_R8:
		value = (long)(var.dblVal + 0.5);
		break;
	case VT_DECIMAL:
		value = (long)var;
		break;
	case VT_CY:
		value = (long)var;
		break;
	case VT_BSTR://字符串
	case VT_LPSTR://字符串
	case VT_LPWSTR://字符串
		value = atol((LPCTSTR)(_bstr_t)var);
		break;
	case VT_NULL:
	case VT_EMPTY:
		value = 0;
		break;
	default:
		TRACE(_T("Warning: 未处理的 _variant_t 类型值; 文件: %s; 行: %d\n"), __FILE__, __LINE__);
	}
	return value;
}

double vartof(const _variant_t& var)
{
	double value = 0;
	switch (var.vt)
	{
	case VT_R4:
		value = var.fltVal;
		break;
	case VT_R8:
		value = var.dblVal;
		break;
	case VT_DECIMAL:
		value = (double)var;
		break;
	case VT_CY:
		value = (double)var;
		break;
	case VT_BOOL:
		value = var.boolVal;
		break;
	case VT_UI1:
	case VT_I1:
		value = var.bVal;
		break;
	case VT_UI2:
	case VT_I2:
		value = var.iVal;
		break;
	case VT_UI4:
	case VT_I4:
		value = var.lVal;
		break;
	case VT_INT:
		value = var.intVal;
		break;
	case VT_BSTR://字符串
	case VT_LPSTR://字符串
	case VT_LPWSTR://字符串
		value = atof((LPCTSTR)(_bstr_t)var);
		break;
	case VT_NULL:
	case VT_EMPTY:
		value = 0;
		break;
	default:
		value = 0;
		TRACE(_T("Warning: 未处理的 _variant_t 类型值; 文件: %s; 行: %d\n"), __FILE__, __LINE__);
	}
	return value;
}

CString vartostr(const _variant_t &var)
{
	CString strValue;

	switch (var.vt)
	{
	case VT_BSTR://字符串
	case VT_LPSTR://字符串
	case VT_LPWSTR://字符串
		strValue = (LPCTSTR)(_bstr_t)var;
		break;
	case VT_I1:
	case VT_UI1:
		strValue.Format("%d", var.bVal);
		break;
	case VT_I2://短整型
		strValue.Format("%d", var.iVal);
		break;
	case VT_UI2://无符号短整型
		strValue.Format("%d", var.uiVal);
		break;
	case VT_INT://整型
		strValue.Format("%d", var.intVal);
		break;
	case VT_I4: //整型
		strValue.Format("%d", var.lVal);
		break;
	case VT_I8: //长整型
		strValue.Format("%d", var.lVal);
		break;
	case VT_UINT://无符号整型
		strValue.Format("%d", var.uintVal);
		break;
	case VT_UI4: //无符号整型
		strValue.Format("%d", var.ulVal);
		break;
	case VT_UI8: //无符号长整型
		strValue.Format("%d", var.ulVal);
		break;
	case VT_VOID:
		strValue.Format("%8x", var.byref);
		break;
	case VT_R4://浮点型
		strValue.Format("%.4f", var.fltVal);
		break;
	case VT_R8://双精度型
		strValue.Format("%.8f", var.dblVal);
		break;
	case VT_DECIMAL: //小数
		strValue.Format("%.8f", (double)var);
		break;
	case VT_CY:
		{
			COleCurrency cy = var.cyVal;
			strValue = cy.Format();
		}
		break;
	case VT_BLOB:
	case VT_BLOB_OBJECT:
	case 0x2011:
		strValue = "[BLOB]";
		break;
	case VT_BOOL://布尔型

		strValue = var.boolVal ? "TRUE" : "FALSE";
		break;
	case VT_DATE: //日期型
		{
			DATE dt = var.date;
			COleDateTime da = COleDateTime(dt); 
			strValue = da.Format("%Y-%m-%d %H:%M:%S");
		}
		break;
	case VT_NULL://NULL值
		strValue = "";
		break;
	case VT_EMPTY://空
		strValue = "";
		break;
	case VT_UNKNOWN://未知类型
	default:
		strValue = "UN_KNOW";
		break;
	}
	return strValue;
}

/*########################################################################
------------------------------------------------
CAdoConnection class 构造/析构函数
------------------------------------------------
########################################################################*/

CAdoConnection::CAdoConnection()
{
	//创建 Connection 对象---------------------------
	//::CoInitialize(NULL);
	m_lOptions = adConnectUnspecified;
	m_pConnection.CreateInstance("ADODB.Connection");
#ifdef _DEBUG
	if (m_pConnection == NULL)
	{
		AfxMessageBox("Connection 对象创建失败! 请确认是否初始化了COM环境\r\n");
	}
#endif
	ASSERT(m_pConnection != NULL);
}

CAdoConnection::~CAdoConnection()
{
	//CoUninitialize();
	if (m_pConnection != NULL)
	{
		Release();
	}
}

/*========================================================================
Name:		连接到数据源.
-----------------------------------------------------
Params:		[lpszConnect]: 连接字符串, 包含连接信息.
[lOptions]:	可选. 决定该方法是以同步还是异步的方式连接数据
源. 可以是如下某个常量:
[常量]					[说明] 
----------------------------------
adConnectUnspecified	(默认)同步方式打开连接. 
adAsyncConnect			异步方式打开连接. Ado用 ConnectComplete 事
件来通知已经完成连接. 
==========================================================================*/
BOOL CAdoConnection::Open(LPCTSTR lpszConnect, long lOptions)
{
	ASSERT(m_pConnection != NULL);
	ASSERT(AfxIsValidString(lpszConnect));

	if (strcmp(lpszConnect, _T("")) != 0)
	{
		m_strConnect = lpszConnect;
	}

	if (m_strConnect.IsEmpty())
	{
		ASSERT(FALSE);
		return FALSE;
	}
	m_lOptions = lOptions;
	if (IsOpen()) Close();

	try
	{
		// 连接数据库 ---------------------------------------------
		return (m_pConnection->Open(_bstr_t(LPCTSTR(m_strConnect)), "", "", m_lOptions) == S_OK);
	}
	catch (_com_error& e)
	{
		TRACE(_T("Warning: 连接数据库发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		TRACE(_T("%s\r\n"), GetLastErrorText());
		return FALSE;
	} 
	catch (...)
	{
		TRACE(_T("Warning: 连接数据库时发生未知错误:"));
	}
	return FALSE;
}

/*========================================================================
Name:	连接 SQL SERVER 数据库. 
-----------------------------------------------------
Params:		[dbsrc]:	SQL SERVER 服务器名.
[dbname]:	默认的数据库名.
[user]:		用户名.
[pass]:		密码.
==========================================================================*/
BOOL CAdoConnection::ConnectSQLServer(CString dbsrc, CString dbname, CString user, CString pass, long lOptions)
{
	CString strConnect = _T("Provider=SQLOLEDB.1; Data Source=") + dbsrc + 
		_T("; Initial Catalog=") + dbname  +
		_T("; User ID=") + user + 
		_T("; PWD=") + pass;
	return Open(LPCTSTR(strConnect), lOptions);
}

/*========================================================================
Name:	连接 ACCESS 数据库. 
-----------------------------------------------------
Params:		[dbpath]:	MDB 数据库文件路径名.
[pass]:		访问密码.
===========================================================================*/
BOOL CAdoConnection::ConnectAccess(CString dbpath, CString pass, long lOptions)
{
	CString strConnect = _T("Provider=Microsoft.Jet.OLEDB.4.0; Data Source=") + dbpath;
	if (pass != _T("")) 
	{
		strConnect += _T("Jet OLEDB:Database Password=") + pass + _T(";");
	}
	return Open(LPCTSTR(strConnect), lOptions);
}

/*========================================================================
Name:	通过 udl 文件连接数据库. 
-----------------------------------------------------
Params:		[strFileName]:	UDL 数据库连接文件路径名.
==========================================================================*/
BOOL CAdoConnection::OpenUDLFile(LPCTSTR strFileName, long lOptions)
{
	CString strConnect = _T("File Name=");
	strConnect += strFileName;
	return Open(LPCTSTR(strConnect), lOptions);
}

/*========================================================================
Name:	关闭与数据源的连接.
-----------------------------------------------------
Remarks: 使用 Close 方法可关闭 Connection 对象以便释放所有关联的系统资源. 
==========================================================================*/
void CAdoConnection::Close()
{
	try
	{
		if (m_pConnection != NULL && IsOpen()) 
		{
			m_pConnection->Close();
		}
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: 关闭数据库发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
	} 
}

/*========================================================================
Name:	关闭连接并释放对象.
-----------------------------------------------------
Remarks: 关闭连接并释放connection对象.
==========================================================================*/
void CAdoConnection::Release()
{
	if (IsOpen()) Close();
	m_pConnection.Release();
}

/*========================================================================
Name:	重新连接数据库
-----------------------------------------------------
Remarks: 根据现在的连接串重联数据库.
==========================================================================*/
BOOL CAdoConnection::ReConnect()
{
	return Open(m_strConnect,m_lOptions);
}

/*========================================================================
Name:		执行指定的查询、SQL 语句、存储过程等.
----------------------------------------------------------
Remarks:	请参考 CADORecordset 类的Open方法. 返回的 Recordset 对象始
终为只读、仅向前的游标.

[常量]				[说明] 
-------------------------------------------------
adCmdText			指示strSQL为命令文本, 即普通的SQL语句. 
adCmdTable			指示ADO生成SQL查询返回以 strSQL 命名的表中的
所有行. 
adCmdTableDirect	指示所作的更改在strSQL中命名的表中返回所有行. 
adCmdStoredProc		指示strSQL为存储过程. 
adCmdUnknown		指示strSQL参数中的命令类型为未知. 
adCmdFile			指示应从在strSQL中命名的文件中恢复保留(保存的)
Recordset. 
adAsyncExecute		指示应异步执行strSQL. 
adAsyncFetch		指示在提取 Initial Fetch Size 属性中指定的初始
数量后, 应该异步提取所有剩余的行. 如果所需的行尚未
提取, 主要的线程将被堵塞直到行重新可用. 
adAsyncFetchNonBlocking 指示主要线程在提取期间从未堵塞. 如果所请求
的行尚未提取, 当前行自动移到文件末尾. 
==========================================================================*/
_RecordsetPtr CAdoConnection::Execute(LPCTSTR lpszSQL, VARIANT* pRecordAfter,long lOptions)
{
	ASSERT(m_pConnection != NULL);
	ASSERT(AfxIsValidString(lpszSQL));

	try
	{
		if(pRecordAfter == NULL)
			return m_pConnection->Execute(_bstr_t(lpszSQL), NULL, lOptions);
		else
		{
			pRecordAfter->vt = VT_I4;
			return m_pConnection->Execute(_bstr_t(pRecordAfter), NULL, lOptions);
		}
	}
	catch (_com_error& e)
	{	
		TRACE(_T("Warning: Execute 方法发生异常. 错误信息:%d , %s; 文件: %s; 行: %d\n"),e.WCode(), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return FALSE;
}

/*========================================================================
Remarks:	请参考 CADORecordset 类 Cancel 方法.
==========================================================================*/
BOOL CAdoConnection::Cancel()
{
	ASSERT(m_pConnection != NULL);
	try
	{
		return m_pConnection->Cancel();
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: Cancel 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return FALSE;
}

/*========================================================================
Name:	取得最后发生的错误信息.
==========================================================================*/
CString CAdoConnection::GetLastErrorText()
{
	ASSERT(m_pConnection != NULL);
	CString strErrors = "";
	try
	{
		if (m_pConnection != NULL)
		{
			ErrorsPtr pErrors = m_pConnection->Errors;
			CString strError;
			for (long n = 0; n < pErrors->Count; n++)
			{
				ErrorPtr pError = pErrors->GetItem(n);
				strError.Format(_T("Description: %s\r\nState: %s, Native: %d, Source: %s\r\n"),
					(LPCTSTR)pError->Description,
					(LPCTSTR)pError->SQLState,
					pError->NativeError,
					(LPCTSTR)pError->Source);
				strErrors += strError;
			}
		}
		return strErrors;
	}
	catch (_com_error& e)
	{
		TRACE(_T("Warning: GetLastError 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return strErrors;
	} 
	return strErrors;
}

ErrorsPtr CAdoConnection::GetErrors()
{
	ASSERT(m_pConnection != NULL);
	try
	{
		if (m_pConnection != NULL)
		{
			return m_pConnection->Errors;
		}
		return NULL;
	}
	catch (_com_error& e)
	{
		TRACE(_T("Warning: GetErrors 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return NULL;
	} 
	return NULL;
}

ErrorPtr CAdoConnection::GetError(long index)
{
	ASSERT(m_pConnection != NULL);
	try
	{
		if (m_pConnection != NULL)
		{
			ErrorsPtr pErrors =  m_pConnection->Errors;
			if (index >= 0 && index < pErrors->Count)
			{
				return pErrors->GetItem(_variant_t(index));
			}
		}
		return NULL;
	}
	catch (_com_error& e)
	{
		TRACE(_T("Warning: GetError 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return NULL;
	} 
	return NULL;
}

/*========================================================================
Name:		取得连接时间.
==========================================================================*/
long CAdoConnection::GetConnectTimeOut()
{
	ASSERT(m_pConnection != NULL);
	try
	{
		return m_pConnection->GetConnectionTimeout();
	}
	catch (_com_error& e)
	{
		TRACE(_T("Warning: GetConnectTimeOut 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return -1;
	} 
	return -1;
}

/*========================================================================
Name:		设置连接时间.
==========================================================================*/
BOOL CAdoConnection::SetConnectTimeOut(long lTime)
{
	ASSERT(m_pConnection != NULL);
	try
	{
		m_pConnection->PutConnectionTimeout(lTime);
		return TRUE;
	}
	catch (_com_error& e)
	{
		TRACE(_T("Warning: SetConnectTimeOut 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
}

/*========================================================================
Name:		取得默认数据库的名称.
==========================================================================*/
CString CAdoConnection::GetDefaultDatabase()
{
	ASSERT(m_pConnection != NULL);
	try
	{
		return CString(LPCTSTR(_bstr_t(m_pConnection->GetDefaultDatabase())));
	}
	catch (_com_error& e)
	{
		TRACE(_T("Warning: GetDefaultDatabase 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return CString(_T(""));
	} 
}

/*========================================================================
Name:		取得 Connection 对象提供者的名称.
==========================================================================*/
CString CAdoConnection::GetProviderName()
{
	ASSERT(m_pConnection != NULL);
	try
	{
		return CString(LPCTSTR(_bstr_t(m_pConnection->GetProvider())));
	}
	catch (_com_error& e)
	{
		TRACE(_T("Warning: GetProviderName 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return CString(_T(""));
	} 
}

/*========================================================================
Name:		取得 ADO 的版本号
==========================================================================*/
CString CAdoConnection::GetVersion()
{
	ASSERT(m_pConnection != NULL);
	try
	{
		return CString(LPCTSTR(_bstr_t(m_pConnection->GetVersion())));
	}
	catch (_com_error& e)
	{
		TRACE(_T("Warning: GetVersion 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return CString(_T(""));
	} 
}

/*========================================================================
Name:		取得对象的状态(同 Recordset 对象的 GetState 方法).
-----------------------------------------------------
returns:	返回下列常量之一的长整型值.
[常量]				[说明] 
----------------------------------
adStateClosed		指示对象是关闭的. 
adStateOpen			指示对象是打开的. 
-----------------------------------------------------
Remarks:		可以随时使用 State 属性取得指定对象的当前状态.
==========================================================================*/
long CAdoConnection::GetState()
{
	ASSERT(m_pConnection != NULL);
	try
	{
		return m_pConnection->GetState();
	}
	catch (_com_error& e)
	{
		TRACE(_T("Warning: GetState 发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return -1;
	} 
	return -1;
}

/*========================================================================
Name:	检测连接对象是否为打开状态.
==========================================================================*/
BOOL CAdoConnection::IsOpen()
{
	try
	{
		return (m_pConnection != NULL && (m_pConnection->State & adStateOpen));
	}
	catch (_com_error& e)
	{
		TRACE(_T("Warning: IsOpen 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return FALSE;
}

/*========================================================================
Name:		取得在 Connection 对象中修改数据的可用权限.
----------------------------------------------------------
returns:	返回以下某个 ConnectModeEnum 的值.
[常量]				 [说明] 
----------------------------------
adModeUnknown		 默认值. 表明权限尚未设置或无法确定. 
adModeRead			 表明权限为只读. 
adModeWrite			 表明权限为只写. 
adModeReadWrite		 表明权限为读/写. 
adModeShareDenyRead  防止其他用户使用读权限打开连接. 
adModeShareDenyWrite 防止其他用户使用写权限打开连接. 
adModeShareExclusive 防止其他用户打开连接. 
adModeShareDenyNone  防止其他用户使用任何权限打开连接.
----------------------------------------------------------
Remarks: 使用 Mode 属性可设置或返回当前连接上提供者正在使用的访问权限.
==========================================================================*/
ConnectModeEnum CAdoConnection::GetMode()
{
	ASSERT(m_pConnection != NULL);
	try
	{
		return m_pConnection->GetMode();
	}
	catch (_com_error& e)
	{
		TRACE(_T("Warning: GetMode 发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return adModeUnknown;
	} 
	return adModeUnknown;
}

/*========================================================================
Name:		设置在 Connection 中修改数据的可用权限. 请参考 GetMode 方法.
----------------------------------------------------------
Remarks:	只能在关闭 Connection 对象时方可设置 Mode 属性.
==========================================================================*/
BOOL CAdoConnection::SetMode(ConnectModeEnum mode)
{
	ASSERT(m_pConnection != NULL);
	ASSERT(!IsOpen());

	try
	{
		m_pConnection->PutMode(mode);
	}
	catch (_com_error& e)
	{
		TRACE(_T("Warning: SetMode 发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return TRUE;
}

/*========================================================================
Name:		从数据源获取数据库信息.
-----------------------------------------------------
Params:		QueryType  所要运行的模式查询类型.
-----------------------------------------------------
returns:	返回包含数据库信息的 Recordset 对象. Recordset 将以只读、静态
游标打开.
==========================================================================*/
_RecordsetPtr CAdoConnection::OpenSchema(SchemaEnum QueryType)
{
	ASSERT(m_pConnection != NULL);
	try
	{
		return m_pConnection->OpenSchema(QueryType, vtMissing, vtMissing);
	}
	catch(_com_error& e)
	{
		TRACE(_T("Warning: OpenSchema方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return NULL;
	} 
	return NULL;
}

/*########################################################################
------------------------------------------------
事务处理
------------------------------------------------
########################################################################*/

/*========================================================================
Name:		开始新事务.
-----------------------------------------------------
returns:	对于支持嵌套事务的数据库来说, 在已打开的事务中调用 BeginTrans 
方法将开始新的嵌套事务. 返回值将指示嵌套层次: 返回值为 1 表示已打开顶层
事务 (即事务不被另一个事务所嵌套), 返回值为 2 表示已打开第二层事务(嵌套
在顶层事务中的事务), 依次类推.
-----------------------------------------------------
Remarks:	一旦调用了 BeginTrans 方法, 在调用 CommitTrans 或 RollbackTrans
结束事务之前, 数据库将不再立即提交所作的任何更改.

==========================================================================*/
long CAdoConnection::BeginTrans()
{
	ASSERT(m_pConnection != NULL);
	try
	{
		return m_pConnection->BeginTrans();
	}
	catch (_com_error& e)
	{
		TRACE(_T("Warning: BeginTrans 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return -1;
	} 
	return -1;
}

/*========================================================================
Name:		保存任何更改并结束当前事务.
-----------------------------------------------------
Remarks:	调用 CommitTrans 或 RollbackTrans 只影响最新打开的事务; 在
处理任何更高层事务之前必须关闭或回卷当前事务.
==========================================================================*/
BOOL CAdoConnection::CommitTrans()
{
	ASSERT(m_pConnection != NULL);
	try
	{
		return SUCCEEDED(m_pConnection->CommitTrans());
	}
	catch (_com_error& e)
	{
		TRACE(_T("Warning: CommitTrans 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return FALSE;
}

/*========================================================================
Name:		取消当前事务中所作的任何更改并结束事务.
==========================================================================*/
BOOL CAdoConnection::RollbackTrans()
{
	ASSERT(m_pConnection != NULL);
	try
	{
		return SUCCEEDED(m_pConnection->RollbackTrans());
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: RollbackTrans 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return FALSE;
}

/*########################################################################
------------------------------------------------
CAdoDatabase class 构造/析构函数
------------------------------------------------
########################################################################*/
DWORD CAdoDatabase::GetRecordCount(_RecordsetPtr m_pRs)
{
	DWORD numRows = 0;

	numRows = m_pRs->GetRecordCount();

	if(numRows == -1)
	{
		if(m_pRs->adoEOF != VARIANT_TRUE)
			m_pRs->MoveFirst();

		while(m_pRs->adoEOF != VARIANT_TRUE)
		{
			numRows++;
			m_pRs->MoveNext();
		}
		if(numRows > 0)
			m_pRs->MoveFirst();
	}
	return numRows;
}

//打开数据源
BOOL CAdoDatabase::Open(LPCTSTR lpstrConnection)
{
	HRESULT hr = S_OK;

	if(IsOpen()) Close();

	if(strcmp(lpstrConnection, _T("")) != 0)
		m_strConnection = lpstrConnection;

	ASSERT(!m_strConnection.IsEmpty());

	try
	{
		hr = m_pConnection->Open(_bstr_t(LPCTSTR(m_strConnection)), "", "", NULL);
		return hr == S_OK;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
	}
	return FALSE;
}

void CAdoDatabase::dump_com_error(_com_error &e)//错误处理程序
{
	CString ErrorStr;

	_bstr_t bstrSource(e.Source());
	_bstr_t bstrDescription(e.Description());
	ErrorStr.Format( "CAdoDatabase Error\n\tCode = %08lx\n\tCode meaning = %s\n\tSource = %s\n\tDescription = %s\n",
		e.Error(), e.ErrorMessage(), (LPCSTR)bstrSource, (LPCSTR)bstrDescription );
	m_strLastError = _T("Connection String = " + GetConnectionString() + '\n' + ErrorStr);
#ifdef _DEBUG
	AfxMessageBox( ErrorStr, MB_OK | MB_ICONERROR );
#endif	
}
//判断是否为连接状态
BOOL CAdoDatabase::IsOpen()
{
	if(m_pConnection)
		return m_pConnection->GetState() != adStateClosed;
	return FALSE;
}
//关掉连接
void CAdoDatabase::Close()
{
	if(IsOpen())
		m_pConnection->Close();
}


BOOL CAdoDatabase::Execute(LPCTSTR lpstrExec)
{
	ASSERT(m_pConnection != NULL);
	ASSERT(strcmp(lpstrExec, _T("")) != 0);

	try
	{
		m_pConnection->Execute(_bstr_t(lpstrExec), NULL, adExecuteNoRecords);
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return FALSE;
	}
	return TRUE;	
}

/*########################################################################
------------------------------------------------
CADORecordset class 构造/析构函数
------------------------------------------------
########################################################################*/

CAdoRecordSet::CAdoRecordSet()
{
	//::CoInitialize(NULL);
	//初始化COM
	m_pXmlStream.CreateInstance(__uuidof(Stream));
	m_pRecordset.CreateInstance(__uuidof(Recordset));
	//m_pRecordset.CreateInstance("ADODB.Recordset");
	//m_pConnection.CreateInstance(__uuidof(Connection));
	//m_pConnection.CreateInstance("ADODB.Connection");


	m_strQuery = _T("");
	m_pConnection = NULL;
	//m_nEditStatus = CAdoRecordSet::dbEditNone;
	m_SearchDirection = adSearchForward;

#ifdef _DEBUG
	if (m_pRecordset == NULL)
	{
		AfxMessageBox("RecordSet 对象创建失败! 请确认是否初始化了COM环境.");
	}
#endif
	ASSERT(m_pRecordset != NULL);
}

CAdoRecordSet::CAdoRecordSet(CAdoConnection *pConnection)
{
	//::CoInitialize(NULL);
	//初始化COM
	m_pXmlStream.CreateInstance(__uuidof(Stream));
	m_pRecordset.CreateInstance(__uuidof(Recordset));

	m_SearchDirection = adSearchForward;
	m_pConnection = pConnection;
	ASSERT(m_pConnection != NULL);
	//m_pRecordset.CreateInstance("ADODB.Recordset");
#ifdef _DEBUG
	if (m_pRecordset == NULL)
	{
		AfxMessageBox("RecordSet 对象创建失败! 请确认是否初始化了COM环境.");
	}
#endif
	ASSERT(m_pRecordset != NULL);
}

CAdoRecordSet::~CAdoRecordSet()
{
	Close();
	if( m_pRecordset!=NULL )
	{
		m_pRecordset.Release();
	}
	m_pRecordset = NULL;
	m_strQuery = _T("");
	m_nEditStatus = dbEditNone;
	//CoUninitialize();	//fix it at 11-27
}

void CAdoRecordSet::SetAdoConnection(CAdoConnection *pConnection)
{
	m_pConnection = pConnection;
}

/*========================================================================
Params:		
- strSQL:		SQL语句, 表名, 存储过程调用或持久 Recordset 文件名.
- CursorType:   可选. CursorTypeEnum 值, 确定打开 Recordset 时应该
使用的游标类型. 可为下列常量之一.
[常量]				[说明] 
-----------------------------------------------
adOpenForwardOnly	打开仅向前类型游标. 
adOpenKeyset		打开键集类型游标. 
adOpenDynamic		打开动态类型游标. 
adOpenStatic		打开静态类型游标. 
-----------------------------------------------
- LockType:     可选, 确定打开 Recordset 时应该使用的锁定类型(并发)
的 LockTypeEnum 值, 可为下列常量之一.
[常量]				[说明] 
-----------------------------------------------
adLockReadOnly		只读 - 不能改变数据. 
adLockPessimistic	保守式锁定 - 通常通过在编辑时立即锁定数据源的记录. 
adLockOptimistic	开放式锁定 - 只在调用 Update 方法时才锁定记录. 
adLockBatchOptimistic 开放式批更新 - 用于批更新模式(与立即更新模式
相对). 
-----------------------------------------------
- lOption		可选. 长整型值, 用于指示 strSQL 参数的类型. 可为下
列常量之一.
[常量]				[说明] 
-------------------------------------------------
adCmdText			指示strSQL为命令文本, 即普通的SQL语句. 
adCmdTable			指示ADO生成SQL查询返回以 strSQL 命名的表中的
所有行. 
adCmdTableDirect	指示所作的更改在strSQL中命名的表中返回所有行. 
adCmdStoredProc		指示strSQL为存储过程. 
adCmdUnknown		指示strSQL参数中的命令类型为未知. 
adCmdFile			指示应从在strSQL中命名的文件中恢复保留(保存的)
Recordset. 
adAsyncExecute		指示应异步执行strSQL. 
adAsyncFetch		指示在提取 Initial Fetch Size 属性中指定的初始
数量后, 应该异步提取所有剩余的行. 如果所需的行尚未
提取, 主要的线程将被堵塞直到行重新可用. 
adAsyncFetchNonBlocking 指示主要线程在提取期间从未堵塞. 如果所请求
的行尚未提取, 当前行自动移到文件末尾. 
==========================================================================*/
BOOL CAdoRecordSet::Open(LPCTSTR strSQL, long lOption, ADODB::CursorTypeEnum CursorType, ADODB::LockTypeEnum LockType)
{
	ASSERT(m_pConnection != NULL);
	ASSERT(m_pRecordset != NULL);
	ASSERT(AfxIsValidString(strSQL));

	if(strcmp(strSQL, _T("")) != 0)
	{
		m_strSQL = strSQL;
	}
	if (m_pConnection == NULL || m_pRecordset == NULL)
	{
		return FALSE;
	}

	if (m_strSQL.IsEmpty())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	try
	{
		if (IsOpen()) Close();
		return SUCCEEDED(m_pRecordset->Open(_variant_t(LPCTSTR(m_strSQL)), 
			_variant_t((IDispatch*)m_pConnection->GetConnection(), true),
			CursorType, LockType, lOption));
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: 打开记录集发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		TRACE(_T("%s\r\n"), GetLastError());
		return FALSE;
	}
}

BOOL CAdoRecordSet::SetRecordset(_RecordsetPtr RecordsetPtr)
{
	this->m_pRecordset = RecordsetPtr;
	return (m_pRecordset != NULL);
}

//获得相应字段名的记录的值
double CAdoRecordSet::GetDoubleFieldValue(LPCTSTR lpFieldName)
{	
	double val = (double)NULL;
	_variant_t vtFld;

	vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
	if(vtFld.vt != VT_NULL)
		val = vtFld.dblVal;
	return val;
}


double CAdoRecordSet::GetDoubleFieldValue(int nIndex)
{	
	double val = (double)NULL;
	_variant_t vtFld;
	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
	if(vtFld.vt != VT_NULL)
		val = vtFld.dblVal;
	return val;
}


long CAdoRecordSet::GetLongFieldValue(LPCTSTR lpFieldName)
{
	long val = (long)NULL;
	_variant_t vtFld;

	try
	{
		vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
	}
	catch (_com_error &e)
	{
		TRACE(_T("Warning: 字段访问失败. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		dump_com_error(e);
	}
	if(vtFld.vt != VT_NULL)
		val = vtFld.lVal;
	return val;
}

long CAdoRecordSet::GetLongFieldValue(int nIndex)
{
	long val = (long)NULL;
	_variant_t vtFld;
	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;
	try
	{
		vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
	}
	catch (_com_error &e)
	{
		TRACE(_T("Warning: 字段访问失败. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		dump_com_error(e);
	}
	if(vtFld.vt != VT_NULL)
		val = vtFld.lVal;
	return val;
}

BOOL CAdoRecordSet::GetBOOLFieldValue(LPCTSTR lpFieldName)
{
	BOOL val = FALSE;
	_variant_t vtFld;

	vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
	switch(vtFld.vt)
	{
	case VT_BOOL:
		val = vtFld.boolVal;
		break;
	default:
		return FALSE;
	}
	return val;
}

int CAdoRecordSet::GetIntFieldValue(LPCTSTR lpFieldName)
{
	int val = NULL;
	_variant_t vtFld;

	try
	{
		vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
	}
	catch (_com_error &e)
	{
		TRACE(_T("Warning: 字段访问失败. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		dump_com_error(e);
	}
	switch(vtFld.vt)
	{
	case VT_I2:
		val = vtFld.iVal;
		break;
	case VT_BOOL:
		val = vtFld.boolVal;
	case VT_NULL:
	case VT_EMPTY:
		break;
	default:
		return vtFld.iVal;
	}	
	return val;
}

int CAdoRecordSet::GetIntFieldValue(int nIndex)
{
	int val = (int)NULL;
	_variant_t vtFld;
	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;
	try
	{
		vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
	}
	catch (_com_error &e)
	{
		TRACE(_T("Warning: 字段访问失败. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
	switch(vtFld.vt)
	{
	case VT_I2:
		val = vtFld.iVal;
		break;
	case VT_NULL:
	case VT_EMPTY:
		val = 0;
		break;
	default:
		return 0;
	}	
	return val;
}

CString CAdoRecordSet::GetStringFieldValue(LPCTSTR lpFieldName)
{
	CString str = _T("");
	_variant_t vtFld;

	try
	{
		vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
	}
	catch (_com_error &e)
	{
		TRACE(_T("Warning: 字段访问失败. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		dump_com_error(e);
	}
	switch(vtFld.vt) 
	{
	case VT_BSTR:
		str = vtFld.bstrVal;
		break;
	case VT_I4:
		str = IntToStr(vtFld.iVal);
		break;
	case VT_DATE:
		{
			COleDateTime dt(vtFld);

			str = dt.Format("%Y-%m-%d %H:%M:%S");
		}
		break;
	case VT_EMPTY:
	case VT_NULL:
		break;
	default:
		return str;
	}
	return str;
}

CString CAdoRecordSet::GetStringFieldValue(int nIndex)
{
	CString str = _T("");
	_variant_t vtFld;
	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
	switch(vtFld.vt) 
	{
	case VT_BSTR:
		str = vtFld.bstrVal;
		break;
	case VT_DATE:
		{
			COleDateTime dt(vtFld);
			str = dt.Format("%Y-%m-%d %H:%M:%S");
		}
		break;
	case VT_EMPTY:
	case VT_NULL:
		break;
	default:
		return str;
	}
	return str;
}

COleDateTime CAdoRecordSet::GetDateTimeFieldValue(LPCTSTR lpFieldName)
{
	COleDateTime time;
	_variant_t vtFld;
	try
	{
		vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
		switch(vtFld.vt) 
		{
		case VT_DATE:
			{
				COleDateTime dt(vtFld);
				time = dt;
			}
			break;
		case VT_EMPTY:
		case VT_NULL:
			time.SetStatus(COleDateTime::null);
			break;
		default:
			time.SetStatus(COleDateTime::null);
			TRACE(_T("Warning: 未处理的 _variant_t 类型值; 文件: %s; 行: %d\n"), __FILE__, __LINE__);
		}
		return time;
	}
	catch (_com_error e)
	{
		time.SetStatus(COleDateTime::null);
		TRACE(_T("Warning: 未处理的 _variant_t 类型值; 文件: %s; 行: %d\n"), __FILE__, __LINE__);
		return time;
	}
}

COleDateTime CAdoRecordSet::GetDateTimeFieldValue(int nIndex)
{
	COleDateTime time;
	_variant_t vtFld;
	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;
	try
	{
		vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
		switch(vtFld.vt) 
		{
		case VT_DATE:
			{
				COleDateTime dt(vtFld);
				time = dt;
			}
			break;
		case VT_EMPTY:
		case VT_NULL:
			break;
		default:
			return time;
		}
		return time;
	}
	catch (_com_error e)
	{
		time.SetStatus(COleDateTime::null);
		return time;
	}
}

BOOL CAdoRecordSet::GetBoolFieldValue(long nIndex)
{
	ASSERT(m_pRecordset != NULL);
	BOOL value;
	_variant_t vtFld;
	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
	switch (vtFld.vt)
	{
	case VT_BOOL:
		value = vtFld.boolVal ? TRUE : FALSE;
	case VT_EMPTY:
	case VT_NULL:
		break;
	default:
		TRACE(_T("Warning: 未处理的 _variant_t 类型值; 文件: %s; 行: %d\n"), __FILE__, __LINE__);
	}

	return value;

}

BOOL CAdoRecordSet::GetBoolFieldValue(LPCSTR lpFieldName)
{
	ASSERT(m_pRecordset != NULL);
	BOOL value;
	_variant_t vtFld;
	vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;

	switch (vtFld.vt)
	{
	case VT_BOOL:
		value = vtFld.boolVal ? TRUE : FALSE;
	case VT_EMPTY:
	case VT_NULL:
		break;
	default:
		TRACE(_T("Warning: 未处理的 _variant_t 类型值; 文件: %s; 行: %d\n"), __FILE__, __LINE__);
	}

	return value;
}

BOOL CAdoRecordSet::IsFieldNull(LPCTSTR lpFieldName)
{
	/*_variant_t vtFld;

	vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
	return vtFld.vt == VT_NULL;*/
	try
	{
		_variant_t vt = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
		return (vt.vt == VT_NULL);
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: IsFieldNull 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

BOOL CAdoRecordSet::IsFieldNull(int nIndex)
{
	_variant_t vtFld;
	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;
	try
	{
		vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
		return (vtFld.vt == VT_NULL);
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: IsFieldNull 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

BOOL CAdoRecordSet::IsFieldEmpty(LPCTSTR lpFieldName)
{
	_variant_t vtFld;
	try
	{
		vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
		return vtFld.vt == VT_EMPTY || vtFld.vt == VT_NULL;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: IsFieldNull 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

BOOL CAdoRecordSet::IsFieldEmpty(int nIndex)
{
	_variant_t vtFld;
	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;
	try
	{
		vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
		return vtFld.vt == VT_EMPTY || vtFld.vt == VT_NULL;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: IsFieldNull 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

//获得记录条数
DWORD CAdoRecordSet::GetRecordCount()
{
	DWORD nRows = 0;
	try
	{	
		nRows = m_pRecordset->GetRecordCount();

		if(nRows == -1)
		{
			nRows = 0;
			if(m_pRecordset->adoEOF != VARIANT_TRUE)
				m_pRecordset->MoveFirst();

			while(m_pRecordset->adoEOF != VARIANT_TRUE)
			{
				nRows++;
				m_pRecordset->MoveNext();
			}
			if(nRows > 0)
				m_pRecordset->MoveFirst();
		}
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetRecordCount 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return -1;
	} 
	return nRows;
}

/*========================================================================
Name:		获取当前记录集中字段数目
==========================================================================*/
long CAdoRecordSet::GetFieldsCount()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		return GetFields()->Count;
	}
	catch(_com_error e)
	{
		TRACE(_T("Warning: GetFieldsCount 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return -1;
	} 
}

//判断记录是否打开
BOOL CAdoRecordSet::IsOpen()
{
	try
	{
		return (m_pRecordset != NULL && (GetState() & adStateOpen));
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: IsOpen方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return FALSE;
}

/*========================================================================
name:		为 Recordset 中的数据指定排序条件.
==========================================================================*/
BOOL CAdoRecordSet::SetSort(LPCTSTR lpszCriteria)
{
	ASSERT(IsOpen());

	try
	{
		m_pRecordset->PutSort(lpszCriteria);
		return TRUE;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: SetFilter 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

/*========================================================================
Name:		关闭打开的对象及任何相关对象.
-----------------------------------------------------
Remarks:	使用 Close 方法可关闭 Recordset 对象以便释放所有关联的系统
资源. 关闭对象并非将它从内存中删除, 可以更改它的属性设置并且在此后
再次打开. 要将对象从内存中完全删除, 可将对象变量设置为 NULL.
如果正在立即更新模式下进行编辑, 调用Close方法将产生错误,应首先
调用 Update 或 CancelUpdat 方法. 如果在批更新期间关闭 Recordset 对
象, 则自上次 UpdateBatch 调用以来所做的修改将全部丢失.
如果使用 Clone 方法创建已打开的 Recordset 对象的副本, 关闭原始
Recordset或其副本将不影响任何其他副本.
==========================================================================*/
void CAdoRecordSet::Close()
{
	try
	{
		if (m_pRecordset != NULL && m_pRecordset->State != adStateClosed)
		{
			if (GetEditMode() == adEditNone)
				CancelUpdate();
			m_pRecordset->Close();
		}
	}
	catch (const _com_error& e)
	{
		TRACE(_T("Warning: 关闭记录集发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
	}
}

ADODB::EditModeEnum CAdoRecordSet::GetEditMode()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			return m_pRecordset->GetEditMode();
		}
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: UpdateBatch 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return adEditNone;
	} 
	return	adEditNone; 
}

/*========================================================================
Name:		通过重新执行对象所基于的查询, 更新 Recordset 对象中的数据.
----------------------------------------------------------
Params:		Options   可选. 指示影响该操作选项的位屏蔽. 如果该参数设置
为 adAsyncExecute, 则该操作将异步执行并在它结束时产生 
RecordsetChangeComplete 事件
----------------------------------------------------------
Remarks:	通过重新发出原始命令并再次检索数据, 可使用 Requery 方法刷新
来自数据源的 Recordset 对象的全部内容. 调用该方法等于相继调用 Close 和 
Open 方法. 如果正在编辑当前记录或者添加新记录将产生错误.
==========================================================================*/
BOOL CAdoRecordSet::Requery(long Options)
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			return (m_pRecordset->Requery(Options) == S_OK);
		}
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: Requery 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return	FALSE; 
}

/*========================================================================
Name:		从基本数据库刷新当前 Recordset 对象中的数据.	
----------------------------------------------------------
Params:		AffectRecords:   可选, AffectEnum 值, 决定 Resync 方法所影
响的记录数目, 可以为下列常量之一.
[常量]				[说明]
------------------------------------
adAffectCurrent		只刷新当前记录. 
adAffectGroup		刷新满足当前 Filter 属性设置的记录.只有将 Filter
属性设置为有效预定义常量之一才能使用该选项. 
adAffectAll			默认值.刷新 Recordset 对象中的所有记录, 包括由
于当前 Filter 属性设置而隐藏的记录. 
adAffectAllChapters 刷新所有子集记录. 

ResyncValues:   可选, ResyncEnum 值. 指定是否覆盖基本值. 可为下列
常量之一.
[常量]				[说明] 
------------------------------------
adResyncAllValues	默认值. 覆盖数据, 取消挂起的更新. 
adResyncUnderlyingValues 不覆盖数据, 不取消挂起的更新. 
----------------------------------------------------------
Remarks:	使用 Resync 方法将当前 Recordset 中的记录与基本的数据库重新
同步. 这在使用静态或仅向前的游标但希望看到基本数据库中的改动时十分有用.
如果将 CursorLocation 属性设置为 adUseClient, 则 Resync 仅对非只读的 
Recordset 对象可用.
与 Requery 方法不同, Resync 方法不重新执行 Recordset 对象的基本的命令, 
基本的数据库中的新记录将不可见.
==========================================================================*/
BOOL CAdoRecordSet::Resync(AffectEnum AffectRecords, ResyncEnum ResyncValues)
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			return (m_pRecordset->Resync(AffectRecords, ResyncValues) == S_OK);
		}
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: Resync 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return	FALSE; 
}

BOOL CAdoRecordSet::RecordBinding(CADORecordBinding &pAdoRecordBinding)
{
	m_pAdoRecordBinding = NULL;

	try
	{
		if (SUCCEEDED(m_pRecordset->QueryInterface(__uuidof(IADORecordBinding), (LPVOID*)&m_pAdoRecordBinding)))
		{
			if (SUCCEEDED(m_pAdoRecordBinding->BindToRecordset(&pAdoRecordBinding)))
			{
				return TRUE;
			}	
		}
		return TRUE;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: RecordBinding 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

void CAdoRecordSet::dump_com_error(_com_error &e)
{
	CString ErrorStr;


	_bstr_t bstrSource(e.Source());
	_bstr_t bstrDescription(e.Description());
	ErrorStr.Format( "CAdoRecordSet Error\n\tCode = %08lx\n\tCode meaning = %s\n\tSource = %s\n\tDescription = %s\n",
		e.Error(), e.ErrorMessage(), (LPCSTR)bstrSource, (LPCSTR)bstrDescription );
	m_strLastError = _T("Query = " + GetQuery() + '\n' + ErrorStr);

#ifdef _DEBUG
	AfxMessageBox( ErrorStr, MB_OK | MB_ICONERROR );
#endif
}

BOOL CAdoRecordSet::GetFieldInfo(LPCTSTR lpFieldName, CADOFieldInfo* fldInfo)
{
	_variant_t vtFld;

	strcpy(fldInfo->m_strName, (LPCTSTR)m_pRecordset->Fields->GetItem(lpFieldName)->GetName());
	fldInfo->m_lSize = m_pRecordset->Fields->GetItem(lpFieldName)->GetActualSize();
	fldInfo->m_lDefinedSize = m_pRecordset->Fields->GetItem(lpFieldName)->GetDefinedSize();
	fldInfo->m_nType = m_pRecordset->Fields->GetItem(lpFieldName)->GetType();
	fldInfo->m_lAttributes = m_pRecordset->Fields->GetItem(lpFieldName)->GetAttributes();
	return TRUE;
}

BOOL CAdoRecordSet::GetFieldInfo(int nIndex, CADOFieldInfo* fldInfo)
{
	_variant_t vtFld;
	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	strcpy(fldInfo->m_strName, (LPCTSTR)m_pRecordset->Fields->GetItem(vtIndex)->GetName());
	fldInfo->m_lSize = m_pRecordset->Fields->GetItem(vtIndex)->GetActualSize();
	fldInfo->m_lDefinedSize = m_pRecordset->Fields->GetItem(vtIndex)->GetDefinedSize();
	fldInfo->m_nType = m_pRecordset->Fields->GetItem(vtIndex)->GetType();
	fldInfo->m_lAttributes = m_pRecordset->Fields->GetItem(vtIndex)->GetAttributes();
	return TRUE;
}


BOOL CAdoRecordSet::GetChunk(LPCTSTR lpFieldName, CString& strValue)
{
	CString str = _T("");
	long lngSize, lngOffSet = 0;
	_variant_t varChunk;
	int ChunkSize = 10000;

	try
	{
		lngSize = m_pRecordset->Fields->GetItem(lpFieldName)->ActualSize;
	}
	catch (_com_error &e)
	{
		TRACE(_T("Warning: 字段访问失败. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		dump_com_error(e);
		return FALSE;
	}


	str.Empty();
	while(lngOffSet < lngSize)
	{
		try
		{
			varChunk = m_pRecordset->Fields->GetItem(lpFieldName)->GetChunk(ChunkSize);
		}
		catch (_com_error &e)
		{
			TRACE(_T("Warning: 字段访问失败. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
			dump_com_error(e);
			return FALSE;
		}
		str += varChunk.bstrVal;
		lngOffSet += ChunkSize;
	}

	lngOffSet = 0;
	strValue = str;
	return TRUE;
}

CString CAdoRecordSet::GetString(LPCTSTR lpCols, LPCTSTR lpRows, LPCTSTR lpNull, long numRows)
{
	_bstr_t varOutput;
	_bstr_t varNull("");
	_bstr_t varCols("\t");
	_bstr_t varRows("\r");

	if(strlen(lpCols) != 0)
		varCols = _bstr_t(lpCols);

	if(strlen(lpRows) != 0)
		varRows = _bstr_t(lpRows);

	if(numRows == 0)
		numRows =(long)GetRecordCount();			

	varOutput = m_pRecordset->GetString(adClipString, numRows, varCols, varRows, varNull);

	return (LPCTSTR)varOutput;
}

CString IntToStr(int nVal)
{
	CString strRet;
	char buff[10];

	itoa(nVal, buff, 10);
	strRet = buff;
	return strRet;
}

CString LongToStr(long lVal)
{
	CString strRet;
	char buff[20];

	ltoa(lVal, buff, 10);
	strRet = buff;
	return strRet;
}

void CAdoRecordSet::Edit()
{
	m_nEditStatus = dbEdit;
}

/*========================================================================
Remarks:	开始添加新的纪录. 
==========================================================================*/
BOOL CAdoRecordSet::AddNew()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			if (m_pRecordset->AddNew() == S_OK)
			{
				return TRUE;
			}
		}
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: AddNew 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return	FALSE;
}

BOOL CAdoRecordSet::AddNew(CADORecordBinding &pAdoRecordBinding)
{
	try
	{
		if (m_pAdoRecordBinding->AddNew(&pAdoRecordBinding) == S_OK)
		{
			m_pAdoRecordBinding->Update(&pAdoRecordBinding);
			return TRUE;
		}
		return FALSE;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: AddNew 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}	
}

//保存记录
BOOL CAdoRecordSet::Update()
{
	BOOL bret = TRUE;

	if(m_nEditStatus != dbEditNone)
	{
		if(m_pRecordset->Update() != S_OK)
			bret = FALSE;
	}

	m_nEditStatus = dbEditNone;
	return bret;
}

/*========================================================================
Name:		取消在调用 Update 方法前对当前记录或新记录所作的任何更改.
-----------------------------------------------------
Remarks:	使用 CancelUpdate 方法可取消对当前记录所作的任何更改或放弃
新添加的记录. 在调用 Update 方法后将无法撤消对当前记录或新记录所做的更
改, 除非更改是可以用 RollbackTrans 方法回卷的事务的一部分, 或者是可以
用 CancelBatch 方法取消的批更新的一部分.
如果在调用 CancelUpdate 方法时添加新记录, 则调用 AddNew 之前的当前
记录将再次成为当前记录.
如果尚未更改当前记录或添加新记录, 调用 CancelUpdate 方法将产生错误.
==========================================================================*/
BOOL CAdoRecordSet::CancelUpdate()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			if (GetEditMode() == adEditNone || m_pRecordset->CancelUpdate() == S_OK)
			{
				return TRUE;
			}
		}
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: CancelUpdate 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return FALSE;
}
//给对应字段名的记录赋值
BOOL CAdoRecordSet::SetFieldValue(int nIndex, CString strValue)
{
	_variant_t vtFld;
	vtFld.vt = VT_BSTR;
	vtFld.bstrVal = _bstr_t(strValue);

	_variant_t vtIndex;
	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	try
	{
		m_pRecordset->Fields->GetItem(vtIndex)->Value = _bstr_t(vtFld);//_bstr_t(strValue);
	}
	catch (_com_error &e)
	{
		TRACE(_T("Warning: 字段访问失败. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		dump_com_error(e);
		return FALSE;
	}
	return TRUE;

}

BOOL CAdoRecordSet::SetFieldValue(LPCTSTR lpFieldName, CString strValue)
{
	_variant_t vtFld;
	vtFld.vt = VT_BSTR;
	vtFld.bstrVal = _bstr_t(strValue);

	try
	{
		m_pRecordset->Fields->GetItem(lpFieldName)->Value = _bstr_t(vtFld);
	}
	catch (_com_error &e)
	{
		TRACE(_T("Warning: 字段访问失败. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		dump_com_error(e);
		return FALSE;
	}
	return TRUE;

}

BOOL CAdoRecordSet::SetFieldValue(int nIndex, int nValue)
{
	_variant_t vtFld;
	vtFld.vt = VT_I2;
	vtFld.iVal = nValue;

	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	try
	{
		m_pRecordset->Fields->GetItem(vtIndex)->Value = vtFld;
	}
	catch (_com_error &e)
	{
		TRACE(_T("Warning: 字段访问失败. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		dump_com_error(e);
		return FALSE;
	}
	return TRUE;

}

BOOL CAdoRecordSet::SetFieldValue(LPCTSTR lpFieldName, int nValue)
{
	_variant_t vtFld;
	vtFld.vt = VT_I2;
	vtFld.iVal = nValue;

	try
	{
		m_pRecordset->Fields->GetItem(lpFieldName)->Value = vtFld;
	}
	catch (_com_error &e)
	{
		TRACE(_T("Warning: 字段访问失败. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		dump_com_error(e);
		return FALSE;
	}
	return TRUE;

}

BOOL CAdoRecordSet::SetFieldValue(int nIndex, long lValue)
{
	_variant_t vtFld;
	vtFld.vt = VT_I4;
	vtFld.lVal = lValue;

	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	try
	{
		m_pRecordset->Fields->GetItem(vtIndex)->Value = vtFld;
	}
	catch (_com_error &e)
	{
		TRACE(_T("Warning: 字段访问失败. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		dump_com_error(e);
		return FALSE;
	}
	return TRUE;

}

BOOL CAdoRecordSet::SetFieldValue(LPCTSTR lpFieldName, long lValue)
{
	_variant_t vtFld;
	vtFld.vt = VT_I4;
	vtFld.lVal = lValue;

	try
	{
		m_pRecordset->Fields->GetItem(lpFieldName)->Value = vtFld;
	}
	catch (_com_error &e)
	{
		TRACE(_T("Warning: 字段访问失败. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		dump_com_error(e);
		return FALSE;
	}
	return TRUE;

}

BOOL CAdoRecordSet::SetFieldValue(int nIndex, double dblValue)
{
	_variant_t vtFld;
	vtFld.vt = VT_R8;
	vtFld.dblVal = dblValue;

	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;
	try
	{
		m_pRecordset->Fields->GetItem(vtIndex)->Value = vtFld;
	}
	catch (_com_error &e)
	{
		TRACE(_T("Warning: 字段访问失败. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		dump_com_error(e);
		return FALSE;
	}
	return TRUE;

}

BOOL CAdoRecordSet::SetFieldValue(LPCTSTR lpFieldName, double dblValue)
{
	_variant_t vtFld;
	vtFld.vt = VT_R8;
	vtFld.dblVal = dblValue;
	try
	{
		m_pRecordset->Fields->GetItem(lpFieldName)->Value = vtFld;
	}
	catch (_com_error &e)
	{
		TRACE(_T("Warning: 字段访问失败. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		dump_com_error(e);
		return FALSE;
	}
	return TRUE;

}

BOOL CAdoRecordSet::SetFieldValue(int nIndex, COleDateTime time)
{
	_variant_t vtFld;
	vtFld.vt = VT_DATE;
	vtFld.date = time;

	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;
	try
	{
		m_pRecordset->Fields->GetItem(vtIndex)->Value = vtFld;
	}
	catch (_com_error &e)
	{
		TRACE(_T("Warning: 字段访问失败. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		dump_com_error(e);
		return FALSE;
	}
	return TRUE;

}

BOOL CAdoRecordSet::SetFieldValue(LPCTSTR lpFieldName, COleDateTime time)
{
	_variant_t vtFld;
	vtFld.vt = VT_DATE;
	vtFld.date = time;
	try
	{
		m_pRecordset->Fields->GetItem(lpFieldName)->Value = vtFld;
	}
	catch (_com_error &e)
	{
		TRACE(_T("Warning: 字段访问失败. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		dump_com_error(e);
		return FALSE;
	}
	return TRUE;

}

BOOL CAdoRecordSet::SetFieldValue(int nIndex, bool value)
{
	_variant_t vtFld;
	vtFld.vt = VT_BOOL;
	vtFld.date = value;

	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;
	try
	{
		m_pRecordset->Fields->GetItem(vtIndex)->Value = vtFld;
	}
	catch (_com_error &e)
	{
		TRACE(_T("Warning: 字段访问失败. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		dump_com_error(e);
		return FALSE;
	}
	return TRUE;
}

BOOL CAdoRecordSet::SetFieldValue(LPCTSTR lpFieldName, bool value)
{
	_variant_t vtFld;
	vtFld.vt = VT_BOOL;
	vtFld.date = value;
	try
	{
		m_pRecordset->Fields->GetItem(lpFieldName)->Value = vtFld;
	}
	catch (_com_error &e)
	{
		TRACE(_T("Warning: 字段访问失败. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		dump_com_error(e);
		return FALSE;
	}
	return TRUE;
}

BOOL CAdoRecordSet::SetBookmark(_variant_t varBookMark)
{
	ASSERT(m_pRecordset != NULL);

	try
	{
		if (IsOpen() && varBookMark.vt != VT_EMPTY && varBookMark.vt != VT_NULL)
		{
			m_pRecordset->PutBookmark(varBookMark);
			return TRUE;
		}	
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: SetBookmark 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
	}
	return FALSE;
}

BOOL CAdoRecordSet::Delete() //删除
{
	if(m_pRecordset->Delete(adAffectCurrent) != S_OK)
		return FALSE;

	if(m_pRecordset->Update() != S_OK)
		return FALSE;

	return TRUE;
}

/*========================================================================
Params:		 AffectRecords:  AffectEnum 值, 确定 Delete 方法所影响的记
录数目, 该值可以是下列常量之一.
[常量]				[说明 ]
-------------------------------------------------
AdAffectCurrent		默认. 仅删除当前记录. 
AdAffectGroup		删除满足当前 Filter 属性设置的记录. 要使用该选
项, 必须将 Filter 属性设置为有效的预定义常量之一. 
adAffectAll			删除所有记录. 
adAffectAllChapters 删除所有子集记录. 
==========================================================================*/
BOOL CAdoRecordSet::Delete(AffectEnum AffectRecords)
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			return (m_pRecordset->Delete(AffectRecords) == S_OK);
		}
	}
	catch (_com_error &e)
	{
		TRACE(_T("Warning: Delete发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return	FALSE;
}

/*========================================================================
Name:		指针是否在在记录集头
==========================================================================*/
BOOL CAdoRecordSet::IsBOF()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		return m_pRecordset->adoBOF;
	}
	catch(_com_error e)
	{
		TRACE(_T("Warning: IsBOF 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return FALSE;
}

/*========================================================================
Name:		指针是否在在记录集尾
==========================================================================*/
BOOL CAdoRecordSet::IsEOF()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		return m_pRecordset->adoEOF;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: IsEOF 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

/*========================================================================
name:		搜索 Recordset 中满足指定标准的记录. 如果满足标准，则记录集
位置设置在找到的记录上，否则位置将设置在记录集的末尾. 
----------------------------------------------------------
params:		[criteria]:   字符串，包含指定用于搜索的列名、比较操作符和
值的语句. 
[searchDirection]:    可选的 SearchDirectionEnum 值，指定搜
索应从当前行还是下一个有效行开始. 其值可为 adSearchForward 或 adSearchBackward. 
搜索是在记录集的开始还是末尾结束由 searchDirection 值决定. 
[常量]			[说明]
---------------------------------
adSearchForward 	
adSearchBackward	
----------------------------------------------------------
Remarks:	criteria 中的"比较操作符"可以是">"(大于)、"<"(小于)、"="(等
于)、">="(大于或等于)、"<="(小于或等于)、"<>"(不等于)或"like"(模式匹配).  
criteria 中的值可以是字符串、浮点数或者日期. 字符串值以单引号分界(如
"state = 'WA'"). 日期值以"#"(数字记号)分界(如"start_date > #7/22/97#"). 
如"比较操作符"为"like"，则字符串"值"可以包含"*"(某字符可出现一次或
多次)或者"_"(某字符只出现一次). (如"state like M_*"与 Maine 和 Massachusetts 
匹配.). 
==========================================================================*/
BOOL CAdoRecordSet::Find(LPCTSTR lpszFind, SearchDirectionEnum SearchDirection)
{
	ASSERT(m_pRecordset != NULL);
	ASSERT(AfxIsValidString(lpszFind));

	try
	{
		if (strcmp(lpszFind, _T("")) != 0)
		{
			m_strFind = lpszFind;
		}

		if (m_strFind.IsEmpty()) return FALSE;

		m_pRecordset->Find(_bstr_t(m_strFind), 0, SearchDirection, "");

		if ((IsEOF() || IsBOF()) )
		{
			return FALSE;
		}
		else
		{
			m_SearchDirection = SearchDirection;
			GetBookmark();
			return TRUE;
		}
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: Find 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

BOOL CAdoRecordSet::FindFirst(LPCTSTR lpFind)
{
	try
	{
		m_pRecordset->MoveFirst();
	}
	catch (_com_error &e)
	{
		TRACE(_T("Warning: SetBookmark 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		dump_com_error(e);
		return FALSE;
	}

	return Find(lpFind);
}

/*========================================================================
name:		查找下一条满足条件的记录.
==========================================================================*/
BOOL CAdoRecordSet::FindNext()
{
	ASSERT(m_pRecordset != NULL);

	try
	{
		if (m_strFind.IsEmpty()) return FALSE;

		m_pRecordset->Find(_bstr_t(m_strFind), 1, m_SearchDirection, m_varBookmark);

		if ((IsEOF() || IsBOF()) )
		{
			return FALSE;
		}
		else
		{
			GetBookmark();
			return TRUE;
		}
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: FindNext 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

// 得记录指
_RecordsetPtr CAdoRecordSet::GetRecordsetPtr()
{
	return m_pRecordset;
}


//////////////////以下是XML处理代码,lincoln增加=============2005.8///////

/*=====数据集-->Binary=====*/
BOOL CAdoRecordSet::SaveToBinary()
{

	ASSERT(m_pRecordset != NULL);
	ASSERT(IsOpen());
	try
	{
		if (m_pRecordset != NULL) 
		{
			//m_pXmlStream->Type = adTypeBinary;
			return(m_pRecordset->Save(m_pXmlStream.GetInterfacePtr(), adPersistADTG) == S_OK);	//serve->client
		}
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: Save 发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
	return FALSE;
}

/*=====数据集-->XML=====*/
BOOL CAdoRecordSet::SaveToXML()
{

	ASSERT(m_pRecordset != NULL);
	ASSERT(IsOpen());
	try
	{
		if (m_pRecordset != NULL) 
		{
			//m_pXmlStream->Type = adTypeBinary;
			return(m_pRecordset->Save(m_pXmlStream.GetInterfacePtr(), adPersistXML) == S_OK);	//serve->client
		}
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: Save 发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return FALSE;
}
/*==================XML--->数据集=================================
Params:
- XmlTmp  :		将要导入的XML流
- LockMode:     可选, 确定打开 Recordset 时应该使用的锁定类型(并发)
的 LockTypeEnum 值, 可为下列常量之一.
[常量]				[说明] 
-----------------------------------------------
adLockReadOnly		只读 - 不能改变数据. 
adLockPessimistic	保守式锁定 - 通常通过在编辑时立即锁定数据源的记录. 
adLockOptimistic	开放式锁定 - 只在调用 Update 方法时才锁定记录. 
adLockBatchOptimistic 开放式批更新 - 用于批更新模式(与立即更新模式
相对). 
-----------------------------------------------
==========================================================================*/
BOOL CAdoRecordSet::LoadXMLOrBuffer(_StreamPtr XmlTmp,ADODB::LockTypeEnum LockMode)
{
	if (IsOpen()) Close();
	//::CoInitialize(NULL);
	try
	{
		return (m_pRecordset->Open(m_pXmlStream.GetInterfacePtr(), vtMissing, adOpenForwardOnly, LockMode, adCmdFile) == S_OK);//serve->client
	}
	catch (_com_error &e)
	{
		TRACE(_T("Warning: Load 发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		//::CoUninitialize();
		return FALSE;
	}
}

/*数据集保存文件为文件格式,默认为ADTG格式*/
BOOL CAdoRecordSet::SaveTOFile(LPCTSTR FileName,PersistFormatEnum PersistFormat)
{
	ASSERT(m_pRecordset != NULL);
	ASSERT(IsOpen());
	try
	{
		if (m_pRecordset != NULL) 
		{
			//m_pXmlStream->Type = adTypeBinary;
			return(m_pRecordset->Save(FileName,PersistFormat) == S_OK);	//serve->client
		}
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: Save 发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return FALSE;
}

/*从文件中载入数据集*/
BOOL CAdoRecordSet::LoadFile(LPCTSTR FileName,ADODB::LockTypeEnum LockMode)
{
	if (IsOpen()) Close();
	//::CoInitialize(NULL);
	try
	{
		return (m_pRecordset->Open(FileName, "Provider=MSPersist;", adOpenForwardOnly, LockMode, adCmdFile) == S_OK);//serve->client
	}
	catch (_com_error &e)
	{
		TRACE(_T("Warning: Load 发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

/*========获取XML数据流=======*/
_StreamPtr CAdoRecordSet::GetXMLStream()
{
	return m_pXmlStream;
}

/*==================XML代码结束===============================*/

////下面代码于2005.8.16新增，功能：对BLOB存取操作　

/*########################################################################
------------------------------------------------
将数据追加到大型文本、二进制数据 Field 对象. 
------------------------------------------------
########################################################################*/
BOOL CAdoRecordSet::AppendChunk(FieldPtr pField, LPVOID lpData, UINT nBytes)
{
	SAFEARRAY FAR *pSafeArray = NULL;
	SAFEARRAYBOUND rgsabound[1];

	try
	{
		rgsabound[0].lLbound = 0;
		rgsabound[0].cElements = nBytes;
		pSafeArray = SafeArrayCreate(VT_UI1, 1, rgsabound);

		//存储代码开始,读取所有数据至SafeArray数组
		for (long i = 0; i < (long)nBytes; i++)
		{
			UCHAR &chData	= ((UCHAR*)lpData)[i];
			HRESULT hr = SafeArrayPutElement(pSafeArray, &i, &chData);
			if (FAILED(hr))	return FALSE;
		}

		//将SaveArray数组存入varChunk
		_variant_t varChunk;
		varChunk.vt = VT_ARRAY | VT_UI1;
		varChunk.parray = pSafeArray;

		return (pField->AppendChunk(varChunk) == S_OK);
	}
	catch (_com_error &e)
	{
		TRACE(_T("Warning: AppendChunk 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

/*-----------将数据追加到大型文本、二进制数据 Field 对象-------------
params: index	: 指定字段索引值
lpData	: 指向二进制数据的指针
nBytes	: 二进制数据的长度
---------------------------------------------------------------------*/
BOOL CAdoRecordSet::AppendChunk(int index, LPVOID lpData, UINT nBytes)
{
	ASSERT(m_pRecordset != NULL);
	ASSERT(lpData != NULL);

	CADOFieldInfo fldInfo;
	GetFieldInfo(index,&fldInfo);
	if (adFldLong & fldInfo.m_lAttributes)
	{
		return AppendChunk(GetField(index), lpData, nBytes);
	}
	else return FALSE;
}

/*-----------------------添加指定字段的大文本，二进制数据-----------------------
Params: strFileIdName : 指定存入的字段名
lpData		  : 指向二进制数据的针
nBytes		  : 二进制数据的长度
-------------------------------------------------------------------------------*/
BOOL CAdoRecordSet::AppendChunk(LPCSTR strFieldName, LPVOID lpData, UINT nBytes)
{
	ASSERT(m_pRecordset != NULL);
	ASSERT(lpData != NULL);
	CADOFieldInfo fldInfo;
	GetFieldInfo(strFieldName,&fldInfo);
	if (adFldLong & fldInfo.m_lAttributes)
	{
		return AppendChunk(GetField(strFieldName), lpData, nBytes);
	}
	else return FALSE;
}

/*------------------往指定二进制字段添加，二进制文件(多媒体文件)----------
params:		strFileIdName : 指定的字段名索引值，需为二进制类型
lpszFileName  :	二进制文件名
--------------------------------------------------------------------------*/
BOOL CAdoRecordSet::AppendChunk(int index, LPCTSTR lpszFileName)
{
	ASSERT(m_pRecordset != NULL);
	ASSERT(lpszFileName != NULL);
	BOOL bret = FALSE;
	CADOFieldInfo fldInfo;
	GetFieldInfo(index,&fldInfo);
	if (adFldLong & fldInfo.m_lAttributes)
	{
		CFile file;
		if (file.Open(lpszFileName, CFile::modeRead))
		{
			long length = (long)file.GetLength();
			char *pbuf = new char[length];
			if (pbuf != NULL && file.Read(pbuf, length) == (DWORD)length)
			{
				bret = AppendChunk(GetField(index), pbuf, length);
			}
			if (pbuf != NULL) delete[] pbuf;
		}
		file.Close();
	}
	return bret;
}

/*------------------往指定二进制字段添加，二进制文件(多媒体文件)----------
params:		strFileIdName : 指定存入的字段名，需为二进制类型
lpszFileName  :	二进制文件名
--------------------------------------------------------------------------*/
BOOL CAdoRecordSet::AppendChunk(LPCSTR strFieldName, LPCTSTR lpszFileName)
{
	ASSERT(m_pRecordset != NULL);
	ASSERT(lpszFileName != NULL);
	BOOL bret = FALSE;
	CADOFieldInfo fldInfo;
	GetFieldInfo(strFieldName,&fldInfo);
	if (adFldLong & fldInfo.m_lAttributes)
	{
		CFile file;
		if (file.Open(lpszFileName, CFile::modeRead))
		{
			long length = (long)file.GetLength();
			char *pbuf = new char[length];
			if (pbuf != NULL && file.Read(pbuf, length) == (DWORD)length)
			{
				bret = AppendChunk(GetField(strFieldName), pbuf, length);
			}
			if (pbuf != NULL) delete[] pbuf;
		}
		file.Close();
	}
	return bret;
}

/*-----------------------获得二进制字段二进制数据----------
params:	pField	: 字段指针
lpData	: 二进制数据指针
-----------------------------------------------------------*/
BOOL CAdoRecordSet::GetChunk(FieldPtr pField, LPVOID lpData)
{
	ASSERT(pField != NULL);
	ASSERT(lpData != NULL);

	UCHAR chData;
	long index = 0;

	while (index < pField->ActualSize)
	{ 
		try
		{
			//读取数据,以100字节为单位
			_variant_t varChunk = pField->GetChunk(100);
			if (varChunk.vt != (VT_ARRAY | VT_UI1))
			{
				return FALSE;
			}
			//数据依顺存入lpData空间
			for (long i = 0; i < 100; i++)
			{
				if (SUCCEEDED( SafeArrayGetElement(varChunk.parray, &i, &chData) ))
				{
					((UCHAR*)lpData)[index] = chData;
					index++;
				}
				else
				{
					break;
				}
			}
		}
		catch (_com_error e)
		{
			TRACE(_T("Warning: GetChunk 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
			return FALSE;
		}
	}

	return TRUE;
}

/*------------获得指定字段的二进制数据(索引)-----------
params :	index	: 字段索引值
lpData	: 存储的二进制数据地址
------------------------------------------------------*/
BOOL CAdoRecordSet::GetChunk(int index, LPVOID lpData)
{
	CADOFieldInfo fldInfo;
	GetFieldInfo(index,&fldInfo);
	if (adFldLong & fldInfo.m_lAttributes)
		return  GetChunk(GetField(index), lpData);
	else return FALSE;
}

/*-----------------------获取指定字段的二进制数据-------------
params:		strFieldName : 要存入数据的二进制字段名
lpData		 : 二进制数据的首地址
--------------------------------------------------------------*/
BOOL CAdoRecordSet::GetChunk(LPCSTR strFieldName, LPVOID lpData)
{
	CADOFieldInfo fldInfo;
	GetFieldInfo(strFieldName,&fldInfo);
	if (adFldLong & fldInfo.m_lAttributes)
		return  GetChunk(GetField(strFieldName), lpData);
	else return FALSE;
}

/*BOOL CAdoRecordSet::GetChunk(int index, CBitmap &bitmap)
{
CADOFieldInfo fldInfo
GetFieldInfo(index,&fldInfo);
CString fldInfo.m_strName;
return GetChunk(fldInfo.m_strName, bitmap);
}*/

/*-----------------字段的记录存入位图文件-------------------------
params : strFieldName	: 要读取的字段名
bitmap			: 要保存的文件名
------------------------------------------------------------------*/
BOOL CAdoRecordSet::GetChunk(LPCSTR strFieldName, CBitmap &bitmap)
{		
	BOOL bret = FALSE;
	CADOFieldInfo fldInfo;
	GetFieldInfo(strFieldName,&fldInfo);
	long size = fldInfo.m_lSize;
	if ((adFldLong & fldInfo.m_lAttributes) && size > 0)
	{
		BYTE *lpData = new BYTE[size];

		if (GetChunk(GetField(strFieldName), (LPVOID)lpData))
		{
			BITMAPFILEHEADER bmpHeader;
			DWORD bmfHeaderLen = sizeof(bmpHeader);
			strncpy((LPSTR)&bmpHeader, (LPSTR)lpData, bmfHeaderLen);

			// 是否是位图 ----------------------------------------
			if (bmpHeader.bfType == (*(WORD*)"BM"))
			{
				BYTE* lpDIBBits = lpData + bmfHeaderLen;
				BITMAPINFOHEADER &bmpiHeader = *(LPBITMAPINFOHEADER)lpDIBBits;
				BITMAPINFO &bmpInfo = *(LPBITMAPINFO)lpDIBBits;
				lpDIBBits = lpData + ((BITMAPFILEHEADER *)lpData)->bfOffBits;

				// 创建位图 --------------------------------------
				CDC dc;
				HDC hdc = GetDC(NULL);
				dc.Attach(hdc);
				HBITMAP hBmp = CreateDIBitmap(dc.m_hDC, &bmpiHeader, CBM_INIT, lpDIBBits, &bmpInfo, DIB_RGB_COLORS);
				if (bitmap.GetSafeHandle() != NULL) bitmap.DeleteObject();
				bitmap.Attach(hBmp);
				dc.Detach();
				ReleaseDC(NULL, hdc);
				bret = TRUE;
			}
		}
		delete[] lpData;
		lpData = NULL;
	}
	return bret;
}

BOOL CAdoRecordSet::GetChunkFile(LPCSTR strFieldName, LPCSTR strFileName)
{
	BOOL bret = FALSE;
	CADOFieldInfo fldInfo;
	CFile File(strFileName,CFile::modeCreate|CFile::modeWrite);
	GetFieldInfo(strFieldName,&fldInfo);
	long size = fldInfo.m_lSize;
	if ((adFldLong & fldInfo.m_lAttributes) && size > 0)
	{
		BYTE *lpData = new BYTE[size];

		if (GetChunk(GetField(strFieldName), (LPVOID)lpData))
		{
			File.Write(lpData,size);
			bret = TRUE;
			File.Close();
		}
		delete[] lpData;
		lpData = NULL;
	}
	return bret;
}

/*========================================================================
Name:	取得指定列的字段对象的指针.	
==========================================================================*/
FieldPtr CAdoRecordSet::GetField(long lIndex)
{
	try
	{
		return GetFields()->GetItem(_variant_t(lIndex));
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetField发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return NULL;
	}
}

/*==============返回指定字段的字段指针================*/
FieldPtr CAdoRecordSet::GetField(LPCTSTR lpszFieldName)
{
	try
	{
		return GetFields()->GetItem(_variant_t(lpszFieldName));
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetField发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return NULL;
	}
}

/*========================================================================
Remarks:	Recordset 对象包括 Field 对象组成的 Fields 集合. 每个Field
对象对应 Recordset 集中的一列.
==========================================================================*/
FieldsPtr CAdoRecordSet::GetFields()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		return m_pRecordset->GetFields();
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetFields 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return NULL;
	} 
	return NULL;
}

/*========================================================================
Remarks:	取得指定列字段的字段名.
==========================================================================*/
CString CAdoRecordSet::GetFieldName(long lIndex)
{
	ASSERT(m_pRecordset != NULL);
	CString strFieldName;
	try
	{
		strFieldName = LPCTSTR(m_pRecordset->Fields->GetItem(_variant_t(lIndex))->GetName());
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetFieldName 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
	}
	return strFieldName;
}

/*========================================================================
name:		取得 Field 对象一项或多项属性.
----------------------------------------------------------
returns:	对于 Field 对象, Attributes 属性为只读, 其值可能为以下任意
一个或多个 FieldAttributeEnum 值的和.
[常量]				[说明] 
-------------------------------------------
adFldMayDefer			指示字段被延迟, 即不从拥有整个记录的数据源检索
字段值, 仅在显式访问这些字段时才进行检索. 
adFldUpdatable		指示可以写入该字段. 
adFldUnknownUpdatable 指示提供者无法确定是否可以写入该字段. 
adFldFixed			指示该字段包含定长数据. 
adFldIsNullable		指示该字段接受 Null 值. 
adFldMayBeNull		指示可以从该字段读取 Null 值. 
adFldLong				指示该字段为长二进制字段. 并指示可以使用 AppendChunk 
和 GetChunk 方法. 
adFldRowID			指示字段包含持久的行标识符, 该标识符无法被写入
并且除了对行进行标识(如记录号、唯一标识符等)外不
存在有意义的值. 
adFldRowVersion		指示该字段包含用来跟踪更新的某种时间或日期标记. 
adFldCacheDeferred	指示提供者缓存了字段值, 并已完成随后对缓存的读取. 
==========================================================================*/
long CAdoRecordSet::GetFieldAttributes(long lIndex)
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		return m_pRecordset->Fields->GetItem(_variant_t(lIndex))->GetAttributes();
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetFieldAttributes 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return -1;
	}
}

long CAdoRecordSet::GetFieldAttributes(LPCTSTR lpszFieldName)
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		return m_pRecordset->Fields->GetItem(_variant_t(lpszFieldName))->GetAttributes();
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetFieldAttributes 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return -1;
	}
}

/*========================================================================
Name:		指示 Field 对象所定义的长度.
----------------------------------------------------------
returns:	返回某个字段定义的长度(按字节数)的长整型值.
----------------------------------------------------------
Remarks:	使用 DefinedSize 属性可确定 Field 对象的数据容量.
==========================================================================*/
long CAdoRecordSet::GetFieldDefineSize(long lIndex)
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		return m_pRecordset->Fields->GetItem(_variant_t(lIndex))->DefinedSize;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetDefineSize 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return -1;
	}
}

long CAdoRecordSet::GetFieldDefineSize(LPCTSTR lpszFieldName)
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		return m_pRecordset->Fields->GetItem(_variant_t(lpszFieldName))->DefinedSize;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetDefineSize 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return -1;
	}
}

/*========================================================================
Name:	取得字段的值的实际长度.
----------------------------------------------------------
returns:	返回长整型值.某些提供者允许设置该属性以便为 BLOB 数据预留
空间, 在此情况下默认值为 0.
----------------------------------------------------------
Remarks:	使用 ActualSize 属性可返回 Field 对象值的实际长度.对于所有
字段,ActualSize 属性为只读.如果 ADO 无法确定 Field 对象值的实
际长度, ActualSize 属性将返回 adUnknown.
如以下范例所示, ActualSize 和  DefinedSize 属性有所不同: 
adVarChar 声明类型且最大长度为 50 个字符的 Field 对象将返回为 
50 的 DefinedSize 属性值, 但是返回的 ActualSize 属性值是当前记
录的字段中存储的数据的长度.
==========================================================================*/
long CAdoRecordSet::GetFieldActualSize(long lIndex)
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		return m_pRecordset->Fields->GetItem(_variant_t(lIndex))->ActualSize;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetFieldActualSize 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return -1;
	}	
}

long CAdoRecordSet::GetFieldActualSize(LPCTSTR lpszFieldName)
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		return m_pRecordset->Fields->GetItem(_variant_t(lpszFieldName))->ActualSize;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetFieldActualSize 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return -1;
	}	
}

/*========================================================================
returns:	返回下列值之一. 相应的 OLE DB 类型标识符在下表的说明栏的括
号中给出.
[常量]			[说明] 
---------------------------------------------------------
adArray			与其他类型一起加入逻辑 OR 以指示该数据是那种类型的
安全数组 (DBTYPE_ARRAY). 
adBigInt			8 字节带符号的整数 (DBTYPE_I8). 
adBinary			二进制值 (DBTYPE_BYTES). 
adBoolean			布尔型值 (DBTYPE_BOOL). 
adByRef			与其他类型一起加入逻辑 OR 以指示该数据是其他类型数
据的指针 (DBTYPE_BYREF). 
adBSTR			以空结尾的字符串 (Unicode) (DBTYPE_BSTR). 
adChar			字符串值 (DBTYPE_STR). 
adCurrency		货币值 (DBTYPE_CY).货币数字的小数点位置固定、小数
点右侧有四位数字.该值保存为 8 字节范围为10,000 的带符
号整型值. 
adDate			日期值 (DBTYPE_DATE).日期按双精度型数值来保存, 数
字全部表示从 1899 年 12 月 30 开始的日期数.小数部分是
一天当中的片段时间. 
adDBDate			日期值 (yyyymmdd) (DBTYPE_DBDATE). 
adDBTime			时间值 (hhmmss) (DBTYPE_DBTIME). 
adDBTimeStamp		时间戳 (yyyymmddhhmmss 加 10 亿分之一的小数)(DBTYPE_DBTIMESTAMP). 
adDecimal			具有固定精度和范围的精确数字值 (DBTYPE_DECIMAL). 
adDouble			双精度浮点值 (DBTYPE_R8). 
adEmpty			未指定值 (DBTYPE_EMPTY). 
adError			32 - 位错误代码 (DBTYPE_ERROR). 
adGUID			全局唯一的标识符 (GUID) (DBTYPE_GUID). 
adIDispatch		OLE 对象上 Idispatch 接口的指针 (DBTYPE_IDISPATCH). 
adInteger			4 字节的带符号整型 (DBTYPE_I4). 
adIUnknown		OLE 对象上 IUnknown 接口的指针 (DBTYPE_IUNKNOWN).
adLongVarBinary	长二进制值. 
adLongVarChar		长字符串值. 
adLongVarWChar	以空结尾的长字符串值. 
adNumeric			具有固定精度和范围的精确数字值 (DBTYPE_NUMERIC). 
adSingle			单精度浮点值 (DBTYPE_R4). 
adSmallInt		2 字节带符号整型 (DBTYPE_I2). 
adTinyInt			1 字节带符号整型 (DBTYPE_I1). 
adUnsignedBigInt	8 字节不带符号整型 (DBTYPE_UI8). 
adUnsignedInt		4 字节不带符号整型 (DBTYPE_UI4). 
adUnsignedSmallInt 2 字节不带符号整型 (DBTYPE_UI2). 
adUnsignedTinyInt 1 字节不带符号整型 (DBTYPE_UI1). 
adUserDefined		用户定义的变量 (DBTYPE_UDT). 
adVarBinary		二进制值. 
adVarChar			字符串值. 
adVariant			自动变体型 (DBTYPE_VARIANT). 
adVector			与其他类型一起加入逻辑 OR 中, 指示数据是 DBVECTOR 
结构(由 OLE DB 定义).该结构含有元素的计数和其他类型 
(DBTYPE_VECTOR) 数据的指针. 
adVarWChar		以空结尾的 Unicode 字符串. 
adWChar			以空结尾的 Unicode 字符串 (DBTYPE_WSTR). 
----------------------------------------------------------
Remarks:	返回指定字段的数据类型.
==========================================================================*/
ADODB::DataTypeEnum CAdoRecordSet::GetFieldType(long lIndex)
{
	ASSERT(m_pRecordset != NULL);
	try 
	{
		return m_pRecordset->Fields->GetItem(_variant_t(lIndex))->GetType();
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetField 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return adEmpty;
	}	
}

ADODB::DataTypeEnum CAdoRecordSet::GetFieldType(LPCTSTR lpszFieldName)
{
	ASSERT(m_pRecordset != NULL);
	try 
	{
		return m_pRecordset->Fields->GetItem(_variant_t(lpszFieldName))->GetType();
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetField发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return adEmpty;
	}	
}

/*========================================================================
name:		为 Recordset 中的数据指定筛选条件.
==========================================================================*/
BOOL CAdoRecordSet::SetFilter(LPCTSTR lpszFilter)
{
	ASSERT(m_pRecordset != NULL);
	ASSERT(IsOpen());

	try
	{
		m_pRecordset->PutFilter(lpszFilter);
		return TRUE;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: SetFilter 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

/*=========================获取联接状态====================================

Name:		取得对象的状态(同 Recordset 对象的 GetState 方法).
-----------------------------------------------------
returns:	返回下列常量之一的长整型值.
[常量]				[说明] 
----------------------------------
adStateClosed		指示对象是关闭的. 
adStateOpen			指示对象是打开的. 
-----------------------------------------------------
Remarks:		可以随时使用 State 属性取得指定对象的当前状态.
==========================================================================*/
long CAdoRecordSet::GetConnetionState()
{
	ASSERT(m_pConnection != NULL);
	try
	{
		return m_pConnection->GetState();
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetState 发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return -1;
	} 
	return -1;
}

/*=======================获取记录集属性==================================

Name:		取得记录集对象的状态(是打开状态还是关闭状态). 对异步方式执
行的 Recordset 对象, 则说明当前的对象状态是连接、执行还是获取状态.
-----------------------------------------------------
returns:	返回下列常量之一的长整型值.
[常量]				[说明] 
----------------------------------
adStateClosed		指示对象是关闭的. 
adStateOpen			指示对象是打开的. 
adStateConnecting	指示 Recordset 对象正在连接. 
adStateExecuting	指示 Recordset 对象正在执行命令. 
adStateFetching		指示 Recordset 对象的行正在被读取. 
-----------------------------------------------------
Remarks:	 可以随时使用 State 属性确定指定对象的当前状态. 该属性是只
读的. Recordset 对象的 State 属性可以是组合值. 例如: 如果正在执行语句,
该属性将是 adStateOpen 和 adStateExecuting 的组合值.
==========================================================================*/
long CAdoRecordSet::GetDataSetState()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		return m_pRecordset->GetState();
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetState 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return -1;
	} 
	return -1;
}

/*========================================================================
name:	创建与现有 Recordset 对象相同的复制 Recordset 对象。可选择指定
该副本为只读。
==========================================================================*/
BOOL CAdoRecordSet::Clone(CAdoRecordSet &pRecordSet)
{
	ASSERT(m_pRecordset != NULL);

	try
	{
		pRecordSet.m_pRecordset = m_pRecordset->Clone(adLockUnspecified);
		return TRUE;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: Clone 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

/*========================================================================
name:		返回唯一标识 Recordset 对象中当前记录的书签。
==========================================================================*/
_variant_t CAdoRecordSet::GetBookmark()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (IsOpen())
		{
			m_varBookmark = m_pRecordset->GetBookmark();
			return m_varBookmark;
		}		
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetBookmark 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
	}
	return _variant_t((long)adBookmarkFirst);
}

BOOL CAdoRecordSet::MoveFirst()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			return SUCCEEDED(m_pRecordset->MoveFirst());
		}
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: MoveFirst 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		dump_com_error(e);
		return FALSE;
	} 
	return	FALSE;
}

BOOL CAdoRecordSet::MoveNext()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			return SUCCEEDED(m_pRecordset->MoveNext());
		}
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: MoveNext 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		dump_com_error(e);
		return FALSE;
	}
	return FALSE;
}

BOOL CAdoRecordSet::MoveLast()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			return SUCCEEDED(m_pRecordset->MoveLast());
		}
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: MoveLast 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		dump_com_error(e);
		return FALSE;
	} 
	return	FALSE;
}
BOOL CAdoRecordSet::MovePrevious()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			return SUCCEEDED(m_pRecordset->MovePrevious());
		}
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: MovePrevious 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		dump_com_error(e);
		return FALSE;
	}
	return FALSE;
}

/*========================================================================
Name:		移动 Recordset 对象中当前记录的位置.
----------------------------------------------------------
Params:		
- lRecords    带符号长整型表达式, 指定当前记录位置移动的记录数.
- Start    可选, 字符串或变体型, 用于计算书签. 也可为下列 
BookmarkEnum 值之一: 
[常量]				[说明] 
--------------------------------
adBookmarkCurrent	默认. 从当前记录开始. 
adBookmarkFirst		从首记录开始. 
adBookmarkLast		从尾记录开始. 
==========================================================================*/
BOOL CAdoRecordSet::Move(long lRecords, _variant_t Start)
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			return SUCCEEDED(m_pRecordset->Move(lRecords, _variant_t(Start)));
		}
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: Move 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return	FALSE;
}

/*========================================================================
Name:		将所有挂起的批更新写入磁盘.
----------------------------------------------------------
Params:		AffectRecords   可选, AffectEnum 值. 决定 UpdateBatch 方法
所影响的记录数目.可以为如下常量之一.
[常量]				[说明] 
------------------------------------
adAffectCurrent		只写入当前记录的挂起更改. 
adAffectGroup		写入满足当前 Filter 属性设置的记录所发生的挂起
更改. 必须将 Filter 属性设置为某个有效的预定义常量才能使用该选项. 
adAffectAll			(默认值). 写入 Recordset 对象中所有记录的挂起
更改, 包括由于当前 Filter 属性设置而隐藏的任何记录. 
adAffectAllChapters 写入所有子集的挂起更改. 
----------------------------------------------------------
Remarks:	按批更新模式修改 Recordset 对象时, 使用 UpdateBatch 方法可
将 Recordset 对象中的所有更改传递到基本数据库.
如果 Recordset 对象支持批更新, 那么可以将一个或多个记录的多重更改缓存在
本地, 然后再调用 UpdateBatch 方法. 如果在调用 UpdateBatch 方法时正在编
辑当前记录或者添加新的记录, 那么在将批更新传送到提供者之前, ADO 将自动
调用 Update 方法保存对当前记录的所有挂起更改.
只能对键集或静态游标使用批更新.
==========================================================================*/
BOOL CAdoRecordSet::UpdateBatch(AffectEnum AffectRecords)
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			return (m_pRecordset->UpdateBatch(adAffectAll) == S_OK);
		}
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: UpdateBatch 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		dump_com_error(e);
		return FALSE;
	}
	return	FALSE;
}

/*================DataProvide数据集存入Buffer====================
Param 1.char* &buffer : 缓存区地址
　2.size		:缓存区大小
 注：这两个参数只要在调用处定义、初始化
 ================================================================*/
BOOL CAdoRecordSet::SaveToBuffer(char* &buffer,LONG &size)
{
	buffer = NULL;
	size = 0;
	if( NULL == this->m_pRecordset || !this->IsOpen() ){
		return FALSE;
	}
	ASSERT(NULL == buffer);

	if(this->SaveToBinary())	//调用数据集-->二进制转换函数
	{
		size = m_pXmlStream->Size;	//获得二进制流长度
		_variant_t varData;
		buffer = new char[size+1];	//建立缓存区空间
		memset(buffer, 0x0, size+1);
		char *pBuf=NULL;
		m_pXmlStream->Position = 0;
		varData = m_pXmlStream->Read(adReadAll);	//二进制流存入_variant_t对像

		SafeArrayAccessData(varData.parray,(void**)&pBuf);//可以注释

		memcpy(buffer,varData.parray->pvData,size);		//二进制数据填充至缓存

		SafeArrayUnaccessData(varData.parray);	//可以注释
		m_pXmlStream->Close();			//关闭数据流
		return true;
	}
	return false;
}

/*================DataProvide Buffer存入数据集=================
Param 1.char* &buffer : 缓存区地址
==============================================================*/
BOOL CAdoRecordSet::LoadBuffer(char* &buffer,LONG size)
{
	ASSERT(NULL != buffer);
	variant_t varOptional(DISP_E_PARAMNOTFOUND,VT_ERROR);

	m_pXmlStream->PutType(adTypeBinary);//类型为二进制
	m_pXmlStream->Open( varOptional,adModeUnknown, adOpenStreamUnspecified, _bstr_t(), _bstr_t());//对流对像初始化

	/*接收缓存初始化*/
	char *pBuf=NULL;
	_variant_t varData;
	varData.vt = 8209;
	varData.parray = SafeArrayCreateVector(VT_UI1,0,size);
	/*缓存初始化结束*/

	SafeArrayAccessData(varData.parray,(void **)&pBuf);//这句不太确定会对程序稳定有否影响,可以注释

	memcpy(varData.parray->pvData,buffer,size);		//从缓存得到二进制数据

	SafeArrayUnaccessData (varData.parray);//这句不太确定会对程序稳定有否影响,可以注释

	//ADD:here
	m_pXmlStream->Write(&varData);	//XML数据流读入数据
	m_pXmlStream->Position = 0;
	if(!LoadXMLOrBuffer(m_pXmlStream))	//数据集读入XML数据流
	{
		m_pXmlStream->Close();
		return false;
	}

	m_pXmlStream->Close();
	//m_pRecordset->PutActiveConnection(_variant_t((IDispatch*)m_pConnection));	//重新激活联接
	return true;
}

/*########################################################################
------------------------------------------------
设置字段的值
------------------------------------------------
########################################################################*/
BOOL CAdoRecordSet::PutCollect(long index, const _variant_t &value)
{
	ASSERT(m_pRecordset != NULL);
	ASSERT(index < GetFieldsCount());
	try
	{
		if (m_pRecordset != NULL) 
		{
			m_pRecordset->PutCollect(_variant_t(index), value);
			return	TRUE;
		}
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: PutCollect 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return	FALSE;
}

BOOL CAdoRecordSet::PutCollect(LPCSTR strFieldName, const _variant_t &value)
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			m_pRecordset->put_Collect(_variant_t(strFieldName), value);
			return TRUE;
		}
	}
	catch (_com_error e)
	{
		return FALSE;
		TRACE(_T("Warning: PutCollect 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
	} 
	return	FALSE;
}

BOOL CAdoRecordSet::PutCollect(long index, const bool &value)
{
	ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
	if (GetFieldType(index) != adBoolean)
		TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d\n"), __FILE__, __LINE__);
#endif

	return PutCollect(index, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const bool &value)
{
	ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
	if (GetFieldType(strFieldName) != adBoolean)
		TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d\n"), __FILE__, __LINE__);
#endif

	return PutCollect(strFieldName, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(long index, const BYTE &value)
{
	ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
	if (GetFieldType(index) != adUnsignedTinyInt)
		TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d\n"), __FILE__, __LINE__);
#endif

	return PutCollect(index, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const BYTE &value)
{
	ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
	if (GetFieldType(strFieldName) != adUnsignedTinyInt)
		TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d\n"), __FILE__, __LINE__);
#endif

	return PutCollect(strFieldName, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(long index, const short &value)
{
	ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
	if (GetFieldType(index) != adSmallInt)
		TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d\n"), __FILE__, __LINE__);
#endif

	return PutCollect(index, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const short &value)
{
	ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
	if (GetFieldType(strFieldName) != adSmallInt)
		TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d\n"), __FILE__, __LINE__);
#endif

	return PutCollect(strFieldName, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(long index, const int &value)
{
	ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
	if (GetFieldType(index) != adInteger)
		TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d\n"), __FILE__, __LINE__);
#endif

	return PutCollect(index, _variant_t(long(value)));
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const int &value)
{
	ASSERT(m_pRecordset != NULL);

#ifdef _DEBUG
	if (GetFieldType(strFieldName) != adInteger)
		TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d\n"), __FILE__, __LINE__);
#endif

	return PutCollect(strFieldName, _variant_t(long(value)));
}

BOOL CAdoRecordSet::PutCollect(long index, const long &value)
{
	ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
	if (GetFieldType(index) != adBigInt)
		TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d\n"), __FILE__, __LINE__);
#endif

	return PutCollect(index, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const long &value)
{
	ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
	if (GetFieldType(strFieldName) != adBigInt)
		TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d\n"), __FILE__, __LINE__);
#endif

	return PutCollect(strFieldName, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(long index, const DWORD &value)
{
	ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
	if (GetFieldType(index) != adUnsignedBigInt)
		TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d\n"), __FILE__, __LINE__);
#endif

	_variant_t vt;
	vt.vt = VT_UI4;
	vt.ulVal = value;
	return PutCollect(index, vt);
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const DWORD &value)
{
	ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
	if (GetFieldType(strFieldName) != adUnsignedBigInt)
		TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d\n"), __FILE__, __LINE__);
#endif

	_variant_t vt;
	vt.vt = VT_UI4;
	vt.ulVal = value;
	return PutCollect(strFieldName, vt);
}

BOOL CAdoRecordSet::PutCollect(long index, const float &value)
{
	ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
	if (GetFieldType(index) != adSingle)
		TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d\n"), __FILE__, __LINE__);
#endif

	return PutCollect(index, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const float &value)
{
	ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
	if (GetFieldType(strFieldName) != adSingle)
		TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d\n"), __FILE__, __LINE__);
#endif

	return PutCollect(strFieldName, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(long index, const double &value)
{
	ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
	if (GetFieldType(index) != adDouble)
		TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d\n"), __FILE__, __LINE__);
#endif

	return PutCollect(index, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const double &value)
{
	ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
	if (GetFieldType(strFieldName) != adDouble)
		TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d\n"), __FILE__, __LINE__);
#endif

	return PutCollect(strFieldName, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(long index, const COleDateTime &value)
{
	ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
	if (   GetFieldType(index) != adDate
		&& GetFieldType(index) != adDBDate
		&& GetFieldType(index) != adDBTime
		&& GetFieldType(index) != adDBTimeStamp)
	{
		TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d\n"), __FILE__, __LINE__);
	}
#endif

	_variant_t vt;
	vt.vt = VT_DATE;
	vt.date = value;
	return PutCollect(index, vt);
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const COleDateTime &value)
{
	ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
	if (   GetFieldType(strFieldName) != adDate
		&& GetFieldType(strFieldName) != adDBDate
		&& GetFieldType(strFieldName) != adDBTime
		&& GetFieldType(strFieldName) != adDBTimeStamp)
	{
		TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d\n"), __FILE__, __LINE__);
	}
#endif

	_variant_t vt;
	vt.vt = VT_DATE;
	vt.date = value;
	return PutCollect(strFieldName, vt);
}

BOOL CAdoRecordSet::PutCollect(long index, const COleCurrency &value)
{
	ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
	if (GetFieldType(index) != adCurrency)
		TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d\n"), __FILE__, __LINE__);
#endif

	if (value.m_status == COleCurrency::invalid) return FALSE;

	_variant_t vt;
	vt.vt = VT_CY;
	vt.cyVal = value.m_cur;
	return PutCollect(index, vt);
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const COleCurrency &value)
{
	ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
	if (GetFieldType(strFieldName) != adCurrency)
		TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d\n"), __FILE__, __LINE__);
#endif

	if (value.m_status == COleCurrency::invalid) return FALSE;

	_variant_t vt;
	vt.vt = VT_CY;
	vt.cyVal = value.m_cur;
	return PutCollect(strFieldName, vt);
}

BOOL CAdoRecordSet::PutCollect(long index, const CString &value)
{
	ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
	if (! (GetFieldType(index) == adVarChar
		|| GetFieldType(index) == adChar
		|| GetFieldType(index) == adLongVarChar
		|| GetFieldType(index) == adVarWChar
		|| GetFieldType(index) == adWChar
		|| GetFieldType(index) == adLongVarWChar))
		TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d\n"), __FILE__, __LINE__);
#endif

	_variant_t vt;
	vt.vt = value.IsEmpty() ? VT_NULL : VT_BSTR;
	vt.bstrVal = value.AllocSysString();
	return PutCollect(index, vt);
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const CString &value)
{
	ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
	if (! (GetFieldType(strFieldName) == adVarChar
		|| GetFieldType(strFieldName) == adChar
		|| GetFieldType(strFieldName) == adLongVarChar
		|| GetFieldType(strFieldName) == adVarWChar
		|| GetFieldType(strFieldName) == adWChar
		|| GetFieldType(strFieldName) == adLongVarWChar))
		TRACE(_T("Warning: 你要存贮的字段与变量的数据类型不符; 文件: %s; 行: %d\n"), __FILE__, __LINE__);
#endif

	_variant_t vt;
	vt.vt = value.IsEmpty() ? VT_NULL : VT_BSTR;
	vt.bstrVal = value.AllocSysString();
	return PutCollect(strFieldName, vt);
}

/*########################################################################
------------------------------------------------
记录集属性
------------------------------------------------
########################################################################*/

/*========================================================================
Name:		取得记录集对象的状态(是打开状态还是关闭状态). 对异步方式执
行的 Recordset 对象, 则说明当前的对象状态是连接、执行还是获取状态.
-----------------------------------------------------
returns:	返回下列常量之一的长整型值.
[常量]				[说明] 
----------------------------------
adStateClosed		指示对象是关闭的. 
adStateOpen			指示对象是打开的. 
adStateConnecting	指示 Recordset 对象正在连接. 
adStateExecuting	指示 Recordset 对象正在执行命令. 
adStateFetching		指示 Recordset 对象的行正在被读取. 
-----------------------------------------------------
Remarks:	 可以随时使用 State 属性确定指定对象的当前状态. 该属性是只
读的. Recordset 对象的 State 属性可以是组合值. 例如: 如果正在执行语句,
该属性将是 adStateOpen 和 adStateExecuting 的组合值.
==========================================================================*/
long CAdoRecordSet::GetState()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		return m_pRecordset->GetState();
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetState 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return -1;
	} 
	return -1;
}


/*========================================================================
Name:		指示有关批更新或其他大量操作的当前记录的状态.
-----------------------------------------------------
returns:	返回下列一个或多个 RecordStatusEnum 值之和.
[常量]						[说明]
-------------------------------------------------
adRecOK						成功地更新记录. 
adRecNew					记录是新建的. 
adRecModified				记录被修改. 
adRecDeleted				记录被删除. 
adRecUnmodified				记录没有修改. 
adRecInvalid				由于书签无效, 记录没有保存. 
adRecMultipleChanges		由于影响多个记录, 因此记录未被保存. 
adRecPendingChanges			由于记录引用挂起的插入, 因此未被保存. 
adRecCanceled				由于操作被取消, 未保存记录. 
adRecCantRelease			由于现有记录锁定, 没有保存新记录. 
adRecConcurrencyViolation	由于开放式并发在使用中, 记录未被保存. 
adRecIntegrityViolation		由于用户违反完整性约束, 记录未被保存. 
adRecMaxChangesExceeded		由于存在过多挂起更改, 记录未被保存. 
adRecObjectOpen				由于与打开的储存对象冲突, 记录未被保存. 
adRecOutOfMemory			由于计算机内存不足, 记录未被保存. 
adRecPermissionDenied		由于用户没有足够的权限, 记录未被保存. 
adRecSchemaViolation		由于记录违反基本数据库的结构, 因此未被保存. 
adRecDBDeleted				记录已经从数据源中删除. 
-----------------------------------------------------
Remarks:	使用 Status 属性查看在批更新中被修改的记录有哪些更改被挂起.
也可使用 Status 属性查看大量操作时失败记录的状态. 例如, 调用 Recordset
对象的 Resync、UpdateBatch 或 CancelBatch 方法, 或者设置 Recordset 对象
的 Filter 属性为书签数组. 使用该属性, 可检查指定记录为何失败并将问题解
决.
==========================================================================*/
long CAdoRecordSet::GetStatus()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		return m_pRecordset->GetStatus();
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetStatus 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return -1;
	} 
	return -1;
}

/*########################################################################
------------------------------------------------
读取字段的值
------------------------------------------------
########################################################################*/
BOOL CAdoRecordSet::GetCollect(long index, COleDateTime &value)
{
	ASSERT(m_pRecordset != NULL);

	try
	{
		value = vartodate(m_pRecordset->GetCollect(_variant_t(index)));
		return TRUE;
	}
	catch (_com_error e)
	{
		value.SetStatus(COleDateTime::null);
		return FALSE;
	}
}

BOOL CAdoRecordSet::GetCollect(LPCTSTR strFieldName, COleDateTime &value)
{
	ASSERT(m_pRecordset != NULL);

	try
	{
		value = vartodate(m_pRecordset->GetCollect(_variant_t(strFieldName)));
		return TRUE;
	}
	catch (_com_error e)
	{
		value.SetStatus(COleDateTime::null);
		return FALSE;
	}
}

BOOL CAdoRecordSet::GetCollect(long index, COleCurrency &value)
{
	ASSERT(m_pRecordset != NULL);

	try
	{
		value = vartocy(m_pRecordset->GetCollect(_variant_t(index)));
		return TRUE;
	}
	catch (_com_error e)
	{
		value.m_status = COleCurrency::null;
		return FALSE;
	}
}

BOOL CAdoRecordSet::GetCollect(LPCTSTR strFieldName, COleCurrency &value)
{
	ASSERT(m_pRecordset != NULL);

	try
	{
		value = vartocy(m_pRecordset->GetCollect(_variant_t(strFieldName)));
		return TRUE;
	}
	catch (_com_error e)
	{
		value.m_status = COleCurrency::null;
		return FALSE;
	}
}

BOOL CAdoRecordSet::GetCollect(long index,  bool &value)
{
	ASSERT(m_pRecordset != NULL);

	try
	{
		value = vartobool(m_pRecordset->GetCollect(_variant_t(index)));
		return TRUE;
	}
	catch (_com_error e)
	{
		value = false;
		return FALSE;
	} 	
}

BOOL CAdoRecordSet::GetCollect(LPCSTR strFieldName,  bool &value)
{
	ASSERT(m_pRecordset != NULL);

	try
	{
		value = vartobool(m_pRecordset->GetCollect(_variant_t(strFieldName)));
		return TRUE;
	}
	catch (_com_error e)
	{
		value = false;
		return FALSE;
	} 	
}


BOOL CAdoRecordSet::GetCollect(long index,  BYTE &value)
{
	ASSERT(m_pRecordset != NULL);

	try
	{
		value = vartoby(m_pRecordset->GetCollect(_variant_t(index)));
		return TRUE;
	}
	catch (_com_error e)
	{
		value = 0;
		return FALSE;
	} 	
}

BOOL CAdoRecordSet::GetCollect(LPCSTR strFieldName,  BYTE &value)
{
	ASSERT(m_pRecordset != NULL);

	try
	{
		value = vartoby(m_pRecordset->GetCollect(_variant_t(strFieldName)));
		return TRUE;
	}
	catch (_com_error e)
	{
		value = 0;
		return FALSE;
	} 	
}

BOOL CAdoRecordSet::GetCollect(long index,  short &value)
{
	ASSERT(m_pRecordset != NULL);

	try
	{
		value = vartoi(m_pRecordset->GetCollect(_variant_t(index)));
		return TRUE;
	}
	catch (_com_error e)
	{
		value = 0;
		return FALSE;
	} 	
}

BOOL CAdoRecordSet::GetCollect(LPCSTR strFieldName,  short &value)
{
	ASSERT(m_pRecordset != NULL);

	try
	{
		value = vartoi(m_pRecordset->GetCollect(_variant_t(strFieldName)));
		return TRUE;
	}
	catch (_com_error e)
	{
		value = 0;
		return FALSE;
	} 	
}

BOOL CAdoRecordSet::GetCollect(long index,  int &value)
{
	ASSERT(m_pRecordset != NULL);

	try
	{
		value = (int)vartol(m_pRecordset->GetCollect(_variant_t(index)));
		return TRUE;
	}
	catch (_com_error e)
	{
		value = 0;
		return FALSE;
	} 	
	return FALSE;
}

BOOL CAdoRecordSet::GetCollect(LPCSTR strFieldName,  int &value)
{
	ASSERT(m_pRecordset != NULL);

	try
	{
		value = (int)vartol(m_pRecordset->GetCollect(_variant_t(strFieldName)));
		return TRUE;
	}
	catch (_com_error e)
	{
		value = 0;
		return FALSE;
	} 	
}

BOOL CAdoRecordSet::GetCollect(long index,  long &value)
{
	ASSERT(m_pRecordset != NULL);

	try
	{
		value = vartol(m_pRecordset->GetCollect(_variant_t(index)));
		return TRUE;
	}
	catch (_com_error e)
	{
		value = 0;
		return FALSE;
	} 	
}

BOOL CAdoRecordSet::GetCollect(LPCSTR strFieldName,  long &value)
{
	ASSERT(m_pRecordset != NULL);

	try
	{
		value = vartol(m_pRecordset->GetCollect(_variant_t(strFieldName)));
		return TRUE;
	}
	catch (_com_error e)
	{
		value = 0;
		return FALSE;
	} 	
	return FALSE;
}

BOOL CAdoRecordSet::GetCollect(long index,  DWORD &value)
{
	ASSERT(m_pRecordset != NULL);

	try
	{
		_variant_t result = m_pRecordset->GetCollect(_variant_t(index));
		switch (result.vt)
		{
		case VT_UI4:
		case VT_I4:
			value = result.ulVal;
			break;
		case VT_NULL:
		case VT_EMPTY:
			value = 0;
			break;
		default:
			TRACE(_T("Warning: 无法读取相应的字段, 数据类型不匹配; 文件: %s; 行: %d\n"), __FILE__, __LINE__);
			return FALSE;
		}		
		return TRUE;
	}
	catch (_com_error e)
	{
		value = 0;
		return FALSE;
	} 	
}

BOOL CAdoRecordSet::GetCollect(LPCSTR strFieldName,  DWORD &value)
{
	ASSERT(m_pRecordset != NULL);

	try
	{
		_variant_t result = m_pRecordset->GetCollect(_variant_t(strFieldName));
		switch (result.vt)
		{
		case VT_UI4:
		case VT_I4:
			value = result.ulVal;
			break;
		case VT_NULL:
		case VT_EMPTY:
			value = 0;
			break;
		default:
			TRACE(_T("Warning: 无法读取相应的字段, 数据类型不匹配; 文件: %s; 行: %d\n"), __FILE__, __LINE__);
			return FALSE;
		}		
		return TRUE;
	}
	catch (_com_error e)
	{
		value = 0;
		return FALSE;
	} 	
}

BOOL CAdoRecordSet::GetCollect(long index,  float &value)
{
	ASSERT(m_pRecordset != NULL);

	try
	{
		_variant_t result = m_pRecordset->GetCollect(_variant_t(index));
		switch (result.vt)
		{
		case VT_R4:
			value = result.fltVal;
			break;
		case VT_UI1:
		case VT_I1:
			value = result.bVal;
			break;
		case VT_UI2:
		case VT_I2:
			value = result.iVal;
			break;
		case VT_NULL:
		case VT_EMPTY:
			value = 0;
			break;
		default:
			TRACE(_T("Warning: 无法读取相应的字段, 数据类型不匹配; 文件: %s; 行: %d\n"), __FILE__, __LINE__);
			return FALSE;
		}		
		return TRUE;
	}
	catch (_com_error e)
	{
		value = 0;
		return FALSE;
	} 	
}

BOOL CAdoRecordSet::GetCollect(LPCSTR strFieldName,  float &value)
{
	ASSERT(m_pRecordset != NULL);

	try
	{
		_variant_t result = m_pRecordset->GetCollect(_variant_t(strFieldName));
		switch (result.vt)
		{
		case VT_R4:
			value = result.fltVal;
			break;
		case VT_UI1:
		case VT_I1:
			value = result.bVal;
			break;
		case VT_UI2:
		case VT_I2:
			value = result.iVal;
			break;
		case VT_NULL:
		case VT_EMPTY:
			value = 0;
			break;
		default:
			TRACE(_T("Warning: 无法读取相应的字段, 数据类型不匹配; 文件: %s; 行: %d\n"), __FILE__, __LINE__);
			return FALSE;
		}		
		return TRUE;
	}
	catch (_com_error e)
	{
		value = 0;
		return FALSE;
	} 	
}

BOOL CAdoRecordSet::GetCollect(long index,  double &value)
{
	ASSERT(m_pRecordset != NULL);

	try
	{
		value = vartof(m_pRecordset->GetCollect(_variant_t(index)));
		return TRUE;
	}
	catch (_com_error e)
	{
		value = 0;
		return FALSE;
	} 	
}

BOOL CAdoRecordSet::GetCollect(LPCSTR strFieldName,  double &value)
{
	ASSERT(m_pRecordset != NULL);

	try
	{
		value = vartof(m_pRecordset->GetCollect(_variant_t(strFieldName)));
		return TRUE;
	}
	catch (_com_error e)
	{
		value = 0;
		return FALSE;
	} 	
}

BOOL CAdoRecordSet::GetCollect(long index, CString& strValue)
{
	ASSERT(m_pRecordset != NULL);
	if (index < 0 || index >= GetFieldsCount())
	{
		return FALSE;
	}

	try
	{
		if (!IsOpen())
		{
			MessageBox(NULL, _T("数据库可能已经断开,\r\n请重新连接、然后重试."), _T("提示"), MB_ICONINFORMATION);
			return FALSE;
		} 
		if (m_pRecordset->adoEOF)
		{
			return FALSE;
		}
		_variant_t value = m_pRecordset->GetCollect(_variant_t(index));
		strValue = vartostr(value);
		return TRUE;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: 字段访问失败. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}

	return FALSE;
}

/*BOOL CAdoRecordSet::GetCollect(LPCTSTR strFieldName, CString &strValue)
{
ASSERT(m_pRecordset != NULL);
try
{
if (!IsOpen())
{
MessageBox(NULL, _T("数据库可能已经断开,\r\n请重新连接、然后重试."), _T("提示"), MB_ICONINFORMATION);
return FALSE;
} 
if (m_pRecordset->adoEOF)
{
return FALSE;
}
_variant_t value = m_pRecordset->GetCollect(_variant_t(LPCTSTR(strFieldName)));
strValue = vartostr(value);
return TRUE;	
}
catch (_com_error e)
{
TRACE(_T("Warning: 字段访问失败. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
return FALSE;
}

return FALSE;
}*/


/*########################################################################
------------------------------------------------
CAdoParameter class
------------------------------------------------
########################################################################*/
CAdoParameter::CAdoParameter()
{
	m_pParameter = NULL;
	m_pParameter.CreateInstance("ADODB.Parameter");
#ifdef _DEBUG
	if (m_pParameter == NULL)
	{
		AfxMessageBox("Parameter 对象创建失败! 请确认是否初始化了Com环境.");
	}
#endif
	ASSERT(m_pParameter != NULL);
	m_strName = _T("");
}

CAdoParameter::CAdoParameter(ADODB::DataTypeEnum DataType, long lSize, ADODB::ParameterDirectionEnum Direction, CString strName)
{
	m_pParameter = NULL;
	m_pParameter.CreateInstance("ADODB.Parameter");
#ifdef _DEBUG
	if (m_pParameter == NULL)
	{
		AfxMessageBox("Parameter 对象创建失败! 请确认是否初始化了Com环境.");
	}
#endif
	ASSERT(m_pParameter != NULL);

	m_pParameter->Direction = Direction;
	m_strName = strName;
	m_pParameter->Name = m_strName.AllocSysString();
	m_pParameter->Type = DataType;
	m_pParameter->Size = lSize;
}

_ParameterPtr& CAdoParameter::operator =(_ParameterPtr& pParameter)
{
	if (pParameter != NULL)
	{
		m_pParameter = pParameter;
	}
	else
	{
		return pParameter;
	}
	return m_pParameter;
}

CAdoParameter::~CAdoParameter()
{
	m_pParameter.Release();
	m_pParameter = NULL;
	m_strName = _T("");
}

/*========================================================================
Name:		指示在 Parameter 对象中数字值或数字 Field 对象的精度。
----------------------------------------------------------
Params:		设置或返回 Byte 值，用来表示值的最大位数。该值在 Parameter
对象上为读/写，而在 Field 对象上为只读。
----------------------------------------------------------
Remarks:	使用 Precision 属性可确定表示数字 Parameter 或 Field 对象值
的最大位数
==========================================================================*/
BOOL CAdoParameter::SetPrecision(char nPrecision)
{
	ASSERT(m_pParameter != NULL);
	try
	{
		m_pParameter->PutPrecision(nPrecision);
		return TRUE;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: SetPrecision 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

/*========================================================================
Name:		指出 Parameter 或 Field 对象中数字值的范围。
----------------------------------------------------------
Params:		设置或返回字节值，指示数字值所精确到的小数点位数。
----------------------------------------------------------
Remarks:	使用 NumericScale 属性可确定用于表明数字型 Parameter 或 Field 
对象的值的小数位数。
对于 Parameter 对象，NumericScale 属性为读/写。对于 Field 对象，
NumericScale 属性为只读。

==========================================================================*/
BOOL CAdoParameter::SetNumericScale(int nScale)
{
	ASSERT(m_pParameter != NULL);
	try
	{
		m_pParameter->PutNumericScale(nScale);
		return TRUE;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: SetPrecision 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}	
}

/*========================================================================
Name:		指示 Parameter 对象的数据类型.
----------------------------------------------------------
Params:		[DataType]: DataTypeEnum 类型值, 请参考 CRecordSet 类相关
函数.
==========================================================================*/
BOOL CAdoParameter::SetType(ADODB::DataTypeEnum DataType)
{
	ASSERT(m_pParameter != NULL);
	try
	{
		m_pParameter->PutType(DataType);
		return TRUE;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: SetType 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}	
}

ADODB::DataTypeEnum CAdoParameter::GetType()
{
	ASSERT(m_pParameter != NULL);
	try
	{
		return m_pParameter->GetType();
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: SetDirection 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return adEmpty;
	}
}

/*========================================================================
Name:		表示 Parameter 对象的最大大小（按字节或字符）。
----------------------------------------------------------
Params:		[size]: 表示 Parameter 对象的最大大小（按字节或字符）的长
整型值。
==========================================================================*/
BOOL CAdoParameter::SetSize(int size)
{
	ASSERT(m_pParameter != NULL);
	try
	{
		m_pParameter->PutSize(long(size));
		return TRUE;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: SetSize 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}	
}

int CAdoParameter::GetSize()
{
	ASSERT(m_pParameter != NULL);
	try
	{
		return (int)m_pParameter->GetSize();

	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: SetDirection 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return -1;
	}
}

/*========================================================================
Name:		指示对象的名称。
==========================================================================*/
BOOL CAdoParameter::SetName(CString strName)
{
	ASSERT(m_pParameter != NULL);
	try
	{
		m_pParameter->PutName(_bstr_t(LPCTSTR(strName)));
		return TRUE;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: SetName 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}	
}

CString CAdoParameter::GetName()
{
	ASSERT(m_pParameter != NULL);
	try
	{
		return CString(LPCTSTR(m_pParameter->GetName()));
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: SetName 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return CString(_T(""));
	}
}

/*========================================================================
Name:		指示 Parameter 所标明的是输入参数、输出参数还是既是输出又
是输入参数，或该参数是否为存储过程返回的值。
----------------------------------------------------------
Params:		[Direction]: 设置以下某个 ParameterDirectionEnum 值。
[常量]				[说明] 
-------------------------------------------
AdParamUnknown		指示参数方向未知。 
AdParamInput		默认值。指示输入参数。 
AdParamOutput		指示输出参数。 
AdParamInputOutput	同时指示输入参数和输出参数。 
AdParamReturnValue	指示返回值。 
==========================================================================*/
BOOL CAdoParameter::SetDirection(ADODB::ParameterDirectionEnum Direction)
{
	ASSERT(m_pParameter != NULL);
	try
	{
		m_pParameter->PutDirection(Direction);
		return TRUE;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: SetDirection 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}	
}

ADODB::ParameterDirectionEnum CAdoParameter::GetDirection()
{
	ASSERT(m_pParameter != NULL);
	try
	{
		return m_pParameter->GetDirection();
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: SetDirection 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return adParamUnknown;
	}	
}

BOOL CAdoParameter::SetValue(const  _variant_t &value)
{
	ASSERT(m_pParameter != NULL);

	try
	{
		if (m_pParameter->Size == 0)
		{
			m_pParameter->Size = sizeof(VARIANT);
		}
		m_pParameter->Value = value;
		return TRUE;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetValue 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

BOOL CAdoParameter::SetValue(const bool &value)
{
	try
	{
		if (m_pParameter->Size == 0)
		{
			m_pParameter->Size = sizeof(short);
		}
		m_pParameter->Value = _variant_t(value);
		return TRUE;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: SetValue 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

BOOL CAdoParameter::SetValue(const int &value)
{
	try
	{
		if (m_pParameter->Size == 0)
		{
			m_pParameter->Size = sizeof(int);
		}
		m_pParameter->Value = _variant_t(long(value));
		return TRUE;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: SetValue 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

BOOL CAdoParameter::SetValue(const long &value)
{
	try
	{
		if (m_pParameter->Size == 0)
		{
			m_pParameter->Size = sizeof(long);
		}
		m_pParameter->Value = _variant_t(value);
		return TRUE;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: SetValue 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

BOOL CAdoParameter::SetValue(const double &value)
{
	try
	{
		if (m_pParameter->Size == 0)
		{
			m_pParameter->Size = sizeof(double);
		}
		m_pParameter->Value = _variant_t(value);
		return TRUE;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: SetValue 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

BOOL CAdoParameter::SetValue(const CString &value)
{
	_variant_t var;
	var.vt = value.IsEmpty() ? VT_NULL : VT_BSTR;
	var.bstrVal = value.AllocSysString();

	try
	{
		if (m_pParameter->Size == 0)
		{
			m_pParameter->Size = value.GetLength();
		}
		m_pParameter->Value = var;
		return TRUE;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: SetValue 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

BOOL CAdoParameter::SetValue(const COleDateTime &value)
{
	_variant_t var;
	var.vt = VT_DATE;
	var.date = value;

	try
	{
		if (m_pParameter->Size == 0)
		{
			m_pParameter->Size = sizeof(DATE);
		}
		m_pParameter->Value = var;
		return TRUE;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: SetValue 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
	return TRUE;
}

BOOL CAdoParameter::SetValue(const BYTE &value)
{
	try
	{
		if (m_pParameter->Size == 0)
		{
			m_pParameter->Size = sizeof(BYTE);
		}
		m_pParameter->Value = _variant_t(value);
		return TRUE;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: SetValue 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

BOOL CAdoParameter::SetValue(const short &value)
{
	try
	{
		if (m_pParameter->Size == 0)
		{
			m_pParameter->Size = sizeof(short);
		}
		m_pParameter->Value = _variant_t(value);
		return TRUE;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: SetValue 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

BOOL CAdoParameter::SetValue(const float &value)
{
	try
	{
		if (m_pParameter->Size == 0)
		{
			m_pParameter->Size = sizeof(float);
		}
		m_pParameter->Value = _variant_t(value);
		return TRUE;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: SetValue 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}


BOOL CAdoParameter::GetValue(bool &value)
{
	try
	{
		value = vartobool(m_pParameter->Value);
		return TRUE;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetValue 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

BOOL CAdoParameter::GetValue(BYTE &value)
{
	try
	{
		value = vartoby(m_pParameter->Value);
		return TRUE;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetValue 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

BOOL CAdoParameter::GetValue(short &value)
{
	try
	{
		value = vartoi(m_pParameter->Value);
		return TRUE;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetValue 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

BOOL CAdoParameter::GetValue(int &value)
{
	try
	{
		value = (int)vartol(m_pParameter->Value);
		return TRUE;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetValue 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

BOOL CAdoParameter::GetValue(long &value)
{
	try
	{
		value = vartol(m_pParameter->Value);
		return TRUE;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetValue 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

BOOL CAdoParameter::GetValue(double &value)
{
	try
	{
		value = vartof(m_pParameter->Value);
		return TRUE;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetValue 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

BOOL CAdoParameter::GetValue(CString &value)
{
	try
	{
		value = vartostr(m_pParameter->Value);
		return TRUE;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetValue 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

BOOL CAdoParameter::GetValue(COleDateTime &value)
{
	try
	{
		value = vartodate(m_pParameter->Value);
		return TRUE;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetValue 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}


/*########################################################################
------------------------------------------------
CAdoCommand class
------------------------------------------------
########################################################################*/

CAdoCommand::CAdoCommand()
{
	///创建 Connection 对象---------------------------
	m_pCommand.CreateInstance("ADODB.Command");
#ifdef _DEBUG
	if (m_pCommand == NULL)
	{
		AfxMessageBox("Command 对象创建失败! 请确认是否初始化了Com环境.");
	}
#endif
	ASSERT(m_pCommand != NULL);
}

CAdoCommand::CAdoCommand(CAdoConnection* pAdoConnection, CString strCommandText, CommandTypeEnum CommandType)
{
	///创建 Connection 对象---------------------------
	m_pCommand.CreateInstance("ADODB.Command");
#ifdef _DEBUG
	if (m_pCommand == NULL)
	{
		AfxMessageBox("Command 对象创建失败! 请确认是否初始化了Com环境.");
	}
#endif
	ASSERT(m_pCommand != NULL);
	ASSERT(pAdoConnection != NULL);
	SetConnection(pAdoConnection);
	if (strCommandText != _T(""))
	{
		SetCommandText(LPCTSTR(strCommandText));
	}
	SetCommandType(CommandType);
}

CAdoCommand::~CAdoCommand()
{
	Release();
}

void CAdoCommand::Release()
{
	try
	{
		m_pCommand.Release();
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: Release方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
	} 
}

_RecordsetPtr CAdoCommand::Execute(long Options)
{
	ASSERT(m_pCommand != NULL);
	try
	{
		return m_pCommand->Execute(NULL, NULL, Options);
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: Execute 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return NULL;
	} 
}

BOOL CAdoCommand::Cancel()
{
	ASSERT(m_pCommand != NULL);

	try
	{
		return (m_pCommand->Cancel() == S_OK);
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: Cancel 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
}

_ParameterPtr CAdoCommand::CreateParameter(LPCTSTR lpstrName, 
										   ADODB::DataTypeEnum Type, 
										   ADODB::ParameterDirectionEnum Direction, 
										   long Size, 
										   _variant_t Value)
{

	ASSERT(m_pCommand != NULL);
	try
	{
		return m_pCommand->CreateParameter(_bstr_t(lpstrName), Type, Direction, Size, Value);
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: CreateParameter 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return NULL;
	} 
}

BOOL CAdoCommand::SetCommandText(LPCTSTR lpstrCommand)
{
	ASSERT(m_pCommand != NULL);
	try
	{
		m_pCommand->PutCommandText(_bstr_t(lpstrCommand));
		return TRUE;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: PutCommandText 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
}

BOOL CAdoCommand::SetConnection(CAdoConnection *pConnect)
{
	ASSERT(pConnect != NULL);
	ASSERT(pConnect->GetConnection() != NULL);
	ASSERT(m_pCommand != NULL);

	try
	{
		m_pCommand->PutActiveConnection(_variant_t((IDispatch*)pConnect->GetConnection(), true));
		return TRUE;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: SetConnection 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
}
/*========================================================================
Name:		指示 Command 对象的类型。
----------------------------------------------------------
returns:	返回以下某个 CommandTypeEnum 的值.
[常量]				 [说明] 
----------------------------------
adCmdText			指示strSQL为命令文本, 即普通的SQL语句. 
adCmdTable			指示ADO生成SQL查询返回以 strSQL 命名的表中的
所有行. 
adCmdTableDirect	指示所作的更改在strSQL中命名的表中返回所有行. 
adCmdStoredProc		指示strSQL为存储过程. 
adCmdUnknown		指示strSQL参数中的命令类型为未知. 
adCmdFile			指示应从在strSQL中命名的文件中恢复保留(保存的)
Recordset. 
adAsyncExecute		指示应异步执行strSQL. 
adAsyncFetch		指示在提取 Initial Fetch Size 属性中指定的初始
数量后, 应该异步提取所有剩余的行. 如果所需的行尚未
提取, 主要的线程将被堵塞直到行重新可用. 
adAsyncFetchNonBlocking 指示主要线程在提取期间从未堵塞. 如果所请求
的行尚未提取, 当前行自动移到文件末尾. 
----------------------------------------------------------
Remarks: 使用 CommandType 属性可优化 CommandText 属性的计算。
如果 CommandType 属性的值等于 adCmdUnknown(默认值), 系统的性能将会
降低, 因为 ADO 必须调用提供者以确定 CommandText 属性是 SQL 语句、还是存
储过程或表格名称。如果知道正在使用的命令的类型, 可通过设置 CommandType 
属性指令 ADO 直接转到相关代码。如果 CommandType 属性与 CommandText 属
性中的命令类型不匹配, 调用 Execute 方法时将产生错误。
==========================================================================*/
BOOL CAdoCommand::SetCommandType(CommandTypeEnum CommandType)
{
	ASSERT(m_pCommand != NULL);

	try
	{
		m_pCommand->PutCommandType(CommandType);
		return TRUE;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: PutCommandType 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
}

long CAdoCommand::GetState()
{
	ASSERT(m_pCommand != NULL);

	try
	{
		return m_pCommand->GetState();
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetState 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return -1;
	} 
}

BOOL CAdoCommand::SetCommandTimeOut(long lTime)
{
	ASSERT(m_pCommand != NULL);

	try
	{
		m_pCommand->PutCommandTimeout(lTime);
		return TRUE;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: SetCommandTimeOut 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
}

ParametersPtr CAdoCommand::GetParameters()
{
	ASSERT(m_pCommand != NULL);

	try
	{
		return m_pCommand->GetParameters();
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: SetCommandTimeOut 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return NULL;
	} 
}

BOOL CAdoCommand::Append(_ParameterPtr param)
{
	ASSERT(m_pCommand != NULL);

	try
	{
		return m_pCommand->GetParameters()->Append((IDispatch*)param);
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: Append 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
}


_ParameterPtr CAdoCommand::GetParamter(LPCTSTR lpstrName)
{
	ASSERT(m_pCommand != NULL);

	try
	{
		return m_pCommand->GetParameters()->GetItem(_variant_t(lpstrName));
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetParamter 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return NULL;
	} 
}

_ParameterPtr CAdoCommand::GetParameter(long index)
{
	ASSERT(m_pCommand != NULL);

	try
	{
		return m_pCommand->GetParameters()->GetItem(_variant_t(index));
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetParamter 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return NULL;
	} 
}

_variant_t CAdoCommand::GetValue(long index)
{
	ASSERT(m_pCommand != NULL);

	try
	{
		return m_pCommand->GetParameters()->GetItem(_variant_t(index))->Value;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetValue 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		_variant_t vt;
		vt.vt = VT_NULL;
		return vt;
	} 
}

_variant_t CAdoCommand::GetValue(LPCTSTR lpstrName)
{
	ASSERT(m_pCommand != NULL);

	try
	{
		return m_pCommand->GetParameters()->GetItem(_variant_t(lpstrName))->Value;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetValue 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		_variant_t vt;
		vt.vt = VT_NULL;
		return vt;
	} 
}

_CommandPtr& CAdoCommand::GetCommand()
{
	return m_pCommand;
}

CAdoParameter CAdoCommand::operator [](int index)
{
	CAdoParameter pParameter;
	ASSERT(m_pCommand != NULL);
	try
	{
		pParameter = m_pCommand->GetParameters()->GetItem(_variant_t(long(index)));
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: operator [] 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
	}
	return pParameter;
}

CAdoParameter CAdoCommand::operator [](LPCTSTR lpszParamName)
{
	CAdoParameter pParameter;
	ASSERT(m_pCommand != NULL);
	try
	{
		pParameter = m_pCommand->GetParameters()->GetItem(_variant_t(lpszParamName));
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: operator [] 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
	}
	return pParameter;
}

