
#include "stdafx.h"
#include <afxdisp.h>
#include "ado.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*########################################################################
------------------------------------------------
��ֵ����ת��
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
		TRACE(_T("Warning: δ����� _variant_t ����ֵ; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
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
		TRACE(_T("Warning: δ����� _variant_t ����ֵ; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
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
		TRACE(_T("Warning: δ����� _variant_t ����ֵ; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
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
		TRACE(_T("Warning: δ����� _variant_t ����ֵ; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
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
		TRACE(_T("Warning: δ����� _variant_t ����ֵ; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
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
	case VT_BSTR://�ַ���
	case VT_LPSTR://�ַ���
	case VT_LPWSTR://�ַ���
		value = atol((LPCTSTR)(_bstr_t)var);
		break;
	case VT_NULL:
	case VT_EMPTY:
		value = 0;
		break;
	default:
		TRACE(_T("Warning: δ����� _variant_t ����ֵ; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
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
	case VT_BSTR://�ַ���
	case VT_LPSTR://�ַ���
	case VT_LPWSTR://�ַ���
		value = atof((LPCTSTR)(_bstr_t)var);
		break;
	case VT_NULL:
	case VT_EMPTY:
		value = 0;
		break;
	default:
		value = 0;
		TRACE(_T("Warning: δ����� _variant_t ����ֵ; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
	}
	return value;
}

CString vartostr(const _variant_t &var)
{
	CString strValue;

	switch (var.vt)
	{
	case VT_BSTR://�ַ���
	case VT_LPSTR://�ַ���
	case VT_LPWSTR://�ַ���
		strValue = (LPCTSTR)(_bstr_t)var;
		break;
	case VT_I1:
	case VT_UI1:
		strValue.Format("%d", var.bVal);
		break;
	case VT_I2://������
		strValue.Format("%d", var.iVal);
		break;
	case VT_UI2://�޷��Ŷ�����
		strValue.Format("%d", var.uiVal);
		break;
	case VT_INT://����
		strValue.Format("%d", var.intVal);
		break;
	case VT_I4: //����
		strValue.Format("%d", var.lVal);
		break;
	case VT_I8: //������
		strValue.Format("%d", var.lVal);
		break;
	case VT_UINT://�޷�������
		strValue.Format("%d", var.uintVal);
		break;
	case VT_UI4: //�޷�������
		strValue.Format("%d", var.ulVal);
		break;
	case VT_UI8: //�޷��ų�����
		strValue.Format("%d", var.ulVal);
		break;
	case VT_VOID:
		strValue.Format("%8x", var.byref);
		break;
	case VT_R4://������
		strValue.Format("%.4f", var.fltVal);
		break;
	case VT_R8://˫������
		strValue.Format("%.8f", var.dblVal);
		break;
	case VT_DECIMAL: //С��
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
	case VT_BOOL://������

		strValue = var.boolVal ? "TRUE" : "FALSE";
		break;
	case VT_DATE: //������
		{
			DATE dt = var.date;
			COleDateTime da = COleDateTime(dt); 
			strValue = da.Format("%Y-%m-%d %H:%M:%S");
		}
		break;
	case VT_NULL://NULLֵ
		strValue = "";
		break;
	case VT_EMPTY://��
		strValue = "";
		break;
	case VT_UNKNOWN://δ֪����
	default:
		strValue = "UN_KNOW";
		break;
	}
	return strValue;
}

/*########################################################################
------------------------------------------------
CAdoConnection class ����/��������
------------------------------------------------
########################################################################*/

CAdoConnection::CAdoConnection()
{
	//���� Connection ����---------------------------
	//::CoInitialize(NULL);
	m_lOptions = adConnectUnspecified;
	m_pConnection.CreateInstance("ADODB.Connection");
#ifdef _DEBUG
	if (m_pConnection == NULL)
	{
		AfxMessageBox("Connection ���󴴽�ʧ��! ��ȷ���Ƿ��ʼ����COM����\r\n");
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
Name:		���ӵ�����Դ.
-----------------------------------------------------
Params:		[lpszConnect]: �����ַ���, ����������Ϣ.
[lOptions]:	��ѡ. �����÷�������ͬ�������첽�ķ�ʽ��������
Դ. ����������ĳ������:
[����]					[˵��] 
----------------------------------
adConnectUnspecified	(Ĭ��)ͬ����ʽ������. 
adAsyncConnect			�첽��ʽ������. Ado�� ConnectComplete ��
����֪ͨ�Ѿ��������. 
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
		// �������ݿ� ---------------------------------------------
		return (m_pConnection->Open(_bstr_t(LPCTSTR(m_strConnect)), "", "", m_lOptions) == S_OK);
	}
	catch (_com_error& e)
	{
		TRACE(_T("Warning: �������ݿⷢ���쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		TRACE(_T("%s\r\n"), GetLastErrorText());
		return FALSE;
	} 
	catch (...)
	{
		TRACE(_T("Warning: �������ݿ�ʱ����δ֪����:"));
	}
	return FALSE;
}

/*========================================================================
Name:	���� SQL SERVER ���ݿ�. 
-----------------------------------------------------
Params:		[dbsrc]:	SQL SERVER ��������.
[dbname]:	Ĭ�ϵ����ݿ���.
[user]:		�û���.
[pass]:		����.
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
Name:	���� ACCESS ���ݿ�. 
-----------------------------------------------------
Params:		[dbpath]:	MDB ���ݿ��ļ�·����.
[pass]:		��������.
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
Name:	ͨ�� udl �ļ��������ݿ�. 
-----------------------------------------------------
Params:		[strFileName]:	UDL ���ݿ������ļ�·����.
==========================================================================*/
BOOL CAdoConnection::OpenUDLFile(LPCTSTR strFileName, long lOptions)
{
	CString strConnect = _T("File Name=");
	strConnect += strFileName;
	return Open(LPCTSTR(strConnect), lOptions);
}

/*========================================================================
Name:	�ر�������Դ������.
-----------------------------------------------------
Remarks: ʹ�� Close �����ɹر� Connection �����Ա��ͷ����й�����ϵͳ��Դ. 
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
		TRACE(_T("Warning: �ر����ݿⷢ���쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
	} 
}

/*========================================================================
Name:	�ر����Ӳ��ͷŶ���.
-----------------------------------------------------
Remarks: �ر����Ӳ��ͷ�connection����.
==========================================================================*/
void CAdoConnection::Release()
{
	if (IsOpen()) Close();
	m_pConnection.Release();
}

/*========================================================================
Name:	�����������ݿ�
-----------------------------------------------------
Remarks: �������ڵ����Ӵ��������ݿ�.
==========================================================================*/
BOOL CAdoConnection::ReConnect()
{
	return Open(m_strConnect,m_lOptions);
}

/*========================================================================
Name:		ִ��ָ���Ĳ�ѯ��SQL ��䡢�洢���̵�.
----------------------------------------------------------
Remarks:	��ο� CADORecordset ���Open����. ���ص� Recordset ����ʼ
��Ϊֻ��������ǰ���α�.

[����]				[˵��] 
-------------------------------------------------
adCmdText			ָʾstrSQLΪ�����ı�, ����ͨ��SQL���. 
adCmdTable			ָʾADO����SQL��ѯ������ strSQL �����ı��е�
������. 
adCmdTableDirect	ָʾ�����ĸ�����strSQL�������ı��з���������. 
adCmdStoredProc		ָʾstrSQLΪ�洢����. 
adCmdUnknown		ָʾstrSQL�����е���������Ϊδ֪. 
adCmdFile			ָʾӦ����strSQL���������ļ��лָ�����(�����)
Recordset. 
adAsyncExecute		ָʾӦ�첽ִ��strSQL. 
adAsyncFetch		ָʾ����ȡ Initial Fetch Size ������ָ���ĳ�ʼ
������, Ӧ���첽��ȡ����ʣ�����. ������������δ
��ȡ, ��Ҫ���߳̽�������ֱ�������¿���. 
adAsyncFetchNonBlocking ָʾ��Ҫ�߳�����ȡ�ڼ��δ����. ���������
������δ��ȡ, ��ǰ���Զ��Ƶ��ļ�ĩβ. 
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
		TRACE(_T("Warning: Execute ���������쳣. ������Ϣ:%d , %s; �ļ�: %s; ��: %d\n"),e.WCode(), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return FALSE;
}

/*========================================================================
Remarks:	��ο� CADORecordset �� Cancel ����.
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
		TRACE(_T("Warning: Cancel ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return FALSE;
}

/*========================================================================
Name:	ȡ��������Ĵ�����Ϣ.
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
		TRACE(_T("Warning: GetLastError ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: GetErrors ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: GetError ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return NULL;
	} 
	return NULL;
}

/*========================================================================
Name:		ȡ������ʱ��.
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
		TRACE(_T("Warning: GetConnectTimeOut ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return -1;
	} 
	return -1;
}

/*========================================================================
Name:		��������ʱ��.
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
		TRACE(_T("Warning: SetConnectTimeOut ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
}

/*========================================================================
Name:		ȡ��Ĭ�����ݿ������.
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
		TRACE(_T("Warning: GetDefaultDatabase ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return CString(_T(""));
	} 
}

/*========================================================================
Name:		ȡ�� Connection �����ṩ�ߵ�����.
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
		TRACE(_T("Warning: GetProviderName ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return CString(_T(""));
	} 
}

/*========================================================================
Name:		ȡ�� ADO �İ汾��
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
		TRACE(_T("Warning: GetVersion ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return CString(_T(""));
	} 
}

/*========================================================================
Name:		ȡ�ö����״̬(ͬ Recordset ����� GetState ����).
-----------------------------------------------------
returns:	�������г���֮һ�ĳ�����ֵ.
[����]				[˵��] 
----------------------------------
adStateClosed		ָʾ�����ǹرյ�. 
adStateOpen			ָʾ�����Ǵ򿪵�. 
-----------------------------------------------------
Remarks:		������ʱʹ�� State ����ȡ��ָ������ĵ�ǰ״̬.
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
		TRACE(_T("Warning: GetState �����쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return -1;
	} 
	return -1;
}

/*========================================================================
Name:	������Ӷ����Ƿ�Ϊ��״̬.
==========================================================================*/
BOOL CAdoConnection::IsOpen()
{
	try
	{
		return (m_pConnection != NULL && (m_pConnection->State & adStateOpen));
	}
	catch (_com_error& e)
	{
		TRACE(_T("Warning: IsOpen ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return FALSE;
}

/*========================================================================
Name:		ȡ���� Connection �������޸����ݵĿ���Ȩ��.
----------------------------------------------------------
returns:	��������ĳ�� ConnectModeEnum ��ֵ.
[����]				 [˵��] 
----------------------------------
adModeUnknown		 Ĭ��ֵ. ����Ȩ����δ���û��޷�ȷ��. 
adModeRead			 ����Ȩ��Ϊֻ��. 
adModeWrite			 ����Ȩ��Ϊֻд. 
adModeReadWrite		 ����Ȩ��Ϊ��/д. 
adModeShareDenyRead  ��ֹ�����û�ʹ�ö�Ȩ�޴�����. 
adModeShareDenyWrite ��ֹ�����û�ʹ��дȨ�޴�����. 
adModeShareExclusive ��ֹ�����û�������. 
adModeShareDenyNone  ��ֹ�����û�ʹ���κ�Ȩ�޴�����.
----------------------------------------------------------
Remarks: ʹ�� Mode ���Կ����û򷵻ص�ǰ�������ṩ������ʹ�õķ���Ȩ��.
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
		TRACE(_T("Warning: GetMode �����쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return adModeUnknown;
	} 
	return adModeUnknown;
}

/*========================================================================
Name:		������ Connection ���޸����ݵĿ���Ȩ��. ��ο� GetMode ����.
----------------------------------------------------------
Remarks:	ֻ���ڹر� Connection ����ʱ�������� Mode ����.
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
		TRACE(_T("Warning: SetMode �����쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return TRUE;
}

/*========================================================================
Name:		������Դ��ȡ���ݿ���Ϣ.
-----------------------------------------------------
Params:		QueryType  ��Ҫ���е�ģʽ��ѯ����.
-----------------------------------------------------
returns:	���ذ������ݿ���Ϣ�� Recordset ����. Recordset ����ֻ������̬
�α��.
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
		TRACE(_T("Warning: OpenSchema���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return NULL;
	} 
	return NULL;
}

/*########################################################################
------------------------------------------------
������
------------------------------------------------
########################################################################*/

/*========================================================================
Name:		��ʼ������.
-----------------------------------------------------
returns:	����֧��Ƕ����������ݿ���˵, ���Ѵ򿪵������е��� BeginTrans 
��������ʼ�µ�Ƕ������. ����ֵ��ָʾǶ�ײ��: ����ֵΪ 1 ��ʾ�Ѵ򿪶���
���� (�����񲻱���һ��������Ƕ��), ����ֵΪ 2 ��ʾ�Ѵ򿪵ڶ�������(Ƕ��
�ڶ��������е�����), ��������.
-----------------------------------------------------
Remarks:	һ�������� BeginTrans ����, �ڵ��� CommitTrans �� RollbackTrans
��������֮ǰ, ���ݿ⽫���������ύ�������κθ���.

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
		TRACE(_T("Warning: BeginTrans ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return -1;
	} 
	return -1;
}

/*========================================================================
Name:		�����κθ��Ĳ�������ǰ����.
-----------------------------------------------------
Remarks:	���� CommitTrans �� RollbackTrans ֻӰ�����´򿪵�����; ��
�����κθ��߲�����֮ǰ����رջ�ؾ�ǰ����.
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
		TRACE(_T("Warning: CommitTrans ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return FALSE;
}

/*========================================================================
Name:		ȡ����ǰ�������������κθ��Ĳ���������.
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
		TRACE(_T("Warning: RollbackTrans ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return FALSE;
}

/*########################################################################
------------------------------------------------
CAdoDatabase class ����/��������
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

//������Դ
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

void CAdoDatabase::dump_com_error(_com_error &e)//���������
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
//�ж��Ƿ�Ϊ����״̬
BOOL CAdoDatabase::IsOpen()
{
	if(m_pConnection)
		return m_pConnection->GetState() != adStateClosed;
	return FALSE;
}
//�ص�����
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
CADORecordset class ����/��������
------------------------------------------------
########################################################################*/

CAdoRecordSet::CAdoRecordSet()
{
	//::CoInitialize(NULL);
	//��ʼ��COM
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
		AfxMessageBox("RecordSet ���󴴽�ʧ��! ��ȷ���Ƿ��ʼ����COM����.");
	}
#endif
	ASSERT(m_pRecordset != NULL);
}

CAdoRecordSet::CAdoRecordSet(CAdoConnection *pConnection)
{
	//::CoInitialize(NULL);
	//��ʼ��COM
	m_pXmlStream.CreateInstance(__uuidof(Stream));
	m_pRecordset.CreateInstance(__uuidof(Recordset));

	m_SearchDirection = adSearchForward;
	m_pConnection = pConnection;
	ASSERT(m_pConnection != NULL);
	//m_pRecordset.CreateInstance("ADODB.Recordset");
#ifdef _DEBUG
	if (m_pRecordset == NULL)
	{
		AfxMessageBox("RecordSet ���󴴽�ʧ��! ��ȷ���Ƿ��ʼ����COM����.");
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
- strSQL:		SQL���, ����, �洢���̵��û�־� Recordset �ļ���.
- CursorType:   ��ѡ. CursorTypeEnum ֵ, ȷ���� Recordset ʱӦ��
ʹ�õ��α�����. ��Ϊ���г���֮һ.
[����]				[˵��] 
-----------------------------------------------
adOpenForwardOnly	�򿪽���ǰ�����α�. 
adOpenKeyset		�򿪼��������α�. 
adOpenDynamic		�򿪶�̬�����α�. 
adOpenStatic		�򿪾�̬�����α�. 
-----------------------------------------------
- LockType:     ��ѡ, ȷ���� Recordset ʱӦ��ʹ�õ���������(����)
�� LockTypeEnum ֵ, ��Ϊ���г���֮һ.
[����]				[˵��] 
-----------------------------------------------
adLockReadOnly		ֻ�� - ���ܸı�����. 
adLockPessimistic	����ʽ���� - ͨ��ͨ���ڱ༭ʱ������������Դ�ļ�¼. 
adLockOptimistic	����ʽ���� - ֻ�ڵ��� Update ����ʱ��������¼. 
adLockBatchOptimistic ����ʽ������ - ����������ģʽ(����������ģʽ
���). 
-----------------------------------------------
- lOption		��ѡ. ������ֵ, ����ָʾ strSQL ����������. ��Ϊ��
�г���֮һ.
[����]				[˵��] 
-------------------------------------------------
adCmdText			ָʾstrSQLΪ�����ı�, ����ͨ��SQL���. 
adCmdTable			ָʾADO����SQL��ѯ������ strSQL �����ı��е�
������. 
adCmdTableDirect	ָʾ�����ĸ�����strSQL�������ı��з���������. 
adCmdStoredProc		ָʾstrSQLΪ�洢����. 
adCmdUnknown		ָʾstrSQL�����е���������Ϊδ֪. 
adCmdFile			ָʾӦ����strSQL���������ļ��лָ�����(�����)
Recordset. 
adAsyncExecute		ָʾӦ�첽ִ��strSQL. 
adAsyncFetch		ָʾ����ȡ Initial Fetch Size ������ָ���ĳ�ʼ
������, Ӧ���첽��ȡ����ʣ�����. ������������δ
��ȡ, ��Ҫ���߳̽�������ֱ�������¿���. 
adAsyncFetchNonBlocking ָʾ��Ҫ�߳�����ȡ�ڼ��δ����. ���������
������δ��ȡ, ��ǰ���Զ��Ƶ��ļ�ĩβ. 
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
		TRACE(_T("Warning: �򿪼�¼�������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		TRACE(_T("%s\r\n"), GetLastError());
		return FALSE;
	}
}

BOOL CAdoRecordSet::SetRecordset(_RecordsetPtr RecordsetPtr)
{
	this->m_pRecordset = RecordsetPtr;
	return (m_pRecordset != NULL);
}

//�����Ӧ�ֶ����ļ�¼��ֵ
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
		TRACE(_T("Warning: �ֶη���ʧ��. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: �ֶη���ʧ��. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: �ֶη���ʧ��. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: �ֶη���ʧ��. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: �ֶη���ʧ��. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
			TRACE(_T("Warning: δ����� _variant_t ����ֵ; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
		}
		return time;
	}
	catch (_com_error e)
	{
		time.SetStatus(COleDateTime::null);
		TRACE(_T("Warning: δ����� _variant_t ����ֵ; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
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
		TRACE(_T("Warning: δ����� _variant_t ����ֵ; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
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
		TRACE(_T("Warning: δ����� _variant_t ����ֵ; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
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
		TRACE(_T("Warning: IsFieldNull ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: IsFieldNull ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: IsFieldNull ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: IsFieldNull ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

//��ü�¼����
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
		TRACE(_T("Warning: GetRecordCount ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return -1;
	} 
	return nRows;
}

/*========================================================================
Name:		��ȡ��ǰ��¼�����ֶ���Ŀ
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
		TRACE(_T("Warning: GetFieldsCount ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return -1;
	} 
}

//�жϼ�¼�Ƿ��
BOOL CAdoRecordSet::IsOpen()
{
	try
	{
		return (m_pRecordset != NULL && (GetState() & adStateOpen));
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: IsOpen���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return FALSE;
}

/*========================================================================
name:		Ϊ Recordset �е�����ָ����������.
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
		TRACE(_T("Warning: SetFilter ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

/*========================================================================
Name:		�رմ򿪵Ķ����κ���ض���.
-----------------------------------------------------
Remarks:	ʹ�� Close �����ɹر� Recordset �����Ա��ͷ����й�����ϵͳ
��Դ. �رն��󲢷ǽ������ڴ���ɾ��, ���Ը��������������ò����ڴ˺�
�ٴδ�. Ҫ��������ڴ�����ȫɾ��, �ɽ������������Ϊ NULL.
���������������ģʽ�½��б༭, ����Close��������������,Ӧ����
���� Update �� CancelUpdat ����. ������������ڼ�ر� Recordset ��
��, �����ϴ� UpdateBatch ���������������޸Ľ�ȫ����ʧ.
���ʹ�� Clone ���������Ѵ򿪵� Recordset ����ĸ���, �ر�ԭʼ
Recordset���丱������Ӱ���κ���������.
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
		TRACE(_T("Warning: �رռ�¼�������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: UpdateBatch ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return adEditNone;
	} 
	return	adEditNone; 
}

/*========================================================================
Name:		ͨ������ִ�ж��������ڵĲ�ѯ, ���� Recordset �����е�����.
----------------------------------------------------------
Params:		Options   ��ѡ. ָʾӰ��ò���ѡ���λ����. ����ò�������
Ϊ adAsyncExecute, ��ò������첽ִ�в���������ʱ���� 
RecordsetChangeComplete �¼�
----------------------------------------------------------
Remarks:	ͨ�����·���ԭʼ����ٴμ�������, ��ʹ�� Requery ����ˢ��
��������Դ�� Recordset �����ȫ������. ���ø÷���������̵��� Close �� 
Open ����. ������ڱ༭��ǰ��¼��������¼�¼����������.
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
		TRACE(_T("Warning: Requery ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return	FALSE; 
}

/*========================================================================
Name:		�ӻ������ݿ�ˢ�µ�ǰ Recordset �����е�����.	
----------------------------------------------------------
Params:		AffectRecords:   ��ѡ, AffectEnum ֵ, ���� Resync ������Ӱ
��ļ�¼��Ŀ, ����Ϊ���г���֮һ.
[����]				[˵��]
------------------------------------
adAffectCurrent		ֻˢ�µ�ǰ��¼. 
adAffectGroup		ˢ�����㵱ǰ Filter �������õļ�¼.ֻ�н� Filter
��������Ϊ��ЧԤ���峣��֮һ����ʹ�ø�ѡ��. 
adAffectAll			Ĭ��ֵ.ˢ�� Recordset �����е����м�¼, ������
�ڵ�ǰ Filter �������ö����صļ�¼. 
adAffectAllChapters ˢ�������Ӽ���¼. 

ResyncValues:   ��ѡ, ResyncEnum ֵ. ָ���Ƿ񸲸ǻ���ֵ. ��Ϊ����
����֮һ.
[����]				[˵��] 
------------------------------------
adResyncAllValues	Ĭ��ֵ. ��������, ȡ������ĸ���. 
adResyncUnderlyingValues ����������, ��ȡ������ĸ���. 
----------------------------------------------------------
Remarks:	ʹ�� Resync ��������ǰ Recordset �еļ�¼����������ݿ�����
ͬ��. ����ʹ�þ�̬�����ǰ���α굫ϣ�������������ݿ��еĸĶ�ʱʮ������.
����� CursorLocation ��������Ϊ adUseClient, �� Resync ���Է�ֻ���� 
Recordset �������.
�� Requery ������ͬ, Resync ����������ִ�� Recordset ����Ļ���������, 
���������ݿ��е��¼�¼�����ɼ�.
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
		TRACE(_T("Warning: Resync ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: RecordBinding ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: �ֶη���ʧ��. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
			TRACE(_T("Warning: �ֶη���ʧ��. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
Remarks:	��ʼ����µļ�¼. 
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
		TRACE(_T("Warning: AddNew ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: AddNew ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}	
}

//�����¼
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
Name:		ȡ���ڵ��� Update ����ǰ�Ե�ǰ��¼���¼�¼�������κθ���.
-----------------------------------------------------
Remarks:	ʹ�� CancelUpdate ������ȡ���Ե�ǰ��¼�������κθ��Ļ����
����ӵļ�¼. �ڵ��� Update �������޷������Ե�ǰ��¼���¼�¼�����ĸ�
��, ���Ǹ����ǿ����� RollbackTrans �����ؾ�������һ����, �����ǿ���
�� CancelBatch ����ȡ���������µ�һ����.
����ڵ��� CancelUpdate ����ʱ����¼�¼, ����� AddNew ֮ǰ�ĵ�ǰ
��¼���ٴγ�Ϊ��ǰ��¼.
�����δ���ĵ�ǰ��¼������¼�¼, ���� CancelUpdate ��������������.
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
		TRACE(_T("Warning: CancelUpdate ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return FALSE;
}
//����Ӧ�ֶ����ļ�¼��ֵ
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
		TRACE(_T("Warning: �ֶη���ʧ��. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: �ֶη���ʧ��. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: �ֶη���ʧ��. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: �ֶη���ʧ��. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: �ֶη���ʧ��. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: �ֶη���ʧ��. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: �ֶη���ʧ��. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: �ֶη���ʧ��. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: �ֶη���ʧ��. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: �ֶη���ʧ��. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: �ֶη���ʧ��. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: �ֶη���ʧ��. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: SetBookmark ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
	}
	return FALSE;
}

BOOL CAdoRecordSet::Delete() //ɾ��
{
	if(m_pRecordset->Delete(adAffectCurrent) != S_OK)
		return FALSE;

	if(m_pRecordset->Update() != S_OK)
		return FALSE;

	return TRUE;
}

/*========================================================================
Params:		 AffectRecords:  AffectEnum ֵ, ȷ�� Delete ������Ӱ��ļ�
¼��Ŀ, ��ֵ���������г���֮һ.
[����]				[˵�� ]
-------------------------------------------------
AdAffectCurrent		Ĭ��. ��ɾ����ǰ��¼. 
AdAffectGroup		ɾ�����㵱ǰ Filter �������õļ�¼. Ҫʹ�ø�ѡ
��, ���뽫 Filter ��������Ϊ��Ч��Ԥ���峣��֮һ. 
adAffectAll			ɾ�����м�¼. 
adAffectAllChapters ɾ�������Ӽ���¼. 
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
		TRACE(_T("Warning: Delete�����쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return	FALSE;
}

/*========================================================================
Name:		ָ���Ƿ����ڼ�¼��ͷ
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
		TRACE(_T("Warning: IsBOF ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return FALSE;
}

/*========================================================================
Name:		ָ���Ƿ����ڼ�¼��β
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
		TRACE(_T("Warning: IsEOF ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

/*========================================================================
name:		���� Recordset ������ָ����׼�ļ�¼. ��������׼�����¼��
λ���������ҵ��ļ�¼�ϣ�����λ�ý������ڼ�¼����ĩβ. 
----------------------------------------------------------
params:		[criteria]:   �ַ���������ָ�������������������Ƚϲ�������
ֵ�����. 
[searchDirection]:    ��ѡ�� SearchDirectionEnum ֵ��ָ����
��Ӧ�ӵ�ǰ�л�����һ����Ч�п�ʼ. ��ֵ��Ϊ adSearchForward �� adSearchBackward. 
�������ڼ�¼���Ŀ�ʼ����ĩβ������ searchDirection ֵ����. 
[����]			[˵��]
---------------------------------
adSearchForward 	
adSearchBackward	
----------------------------------------------------------
Remarks:	criteria �е�"�Ƚϲ�����"������">"(����)��"<"(С��)��"="(��
��)��">="(���ڻ����)��"<="(С�ڻ����)��"<>"(������)��"like"(ģʽƥ��).  
criteria �е�ֵ�������ַ�������������������. �ַ���ֵ�Ե����ŷֽ�(��
"state = 'WA'"). ����ֵ��"#"(���ּǺ�)�ֽ�(��"start_date > #7/22/97#"). 
��"�Ƚϲ�����"Ϊ"like"�����ַ���"ֵ"���԰���"*"(ĳ�ַ��ɳ���һ�λ�
���)����"_"(ĳ�ַ�ֻ����һ��). (��"state like M_*"�� Maine �� Massachusetts 
ƥ��.). 
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
		TRACE(_T("Warning: Find ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: SetBookmark ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		dump_com_error(e);
		return FALSE;
	}

	return Find(lpFind);
}

/*========================================================================
name:		������һ�����������ļ�¼.
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
		TRACE(_T("Warning: FindNext ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

// �ü�¼ָ
_RecordsetPtr CAdoRecordSet::GetRecordsetPtr()
{
	return m_pRecordset;
}


//////////////////������XML�������,lincoln����=============2005.8///////

/*=====���ݼ�-->Binary=====*/
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
		TRACE(_T("Warning: Save �����쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
	return FALSE;
}

/*=====���ݼ�-->XML=====*/
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
		TRACE(_T("Warning: Save �����쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return FALSE;
}
/*==================XML--->���ݼ�=================================
Params:
- XmlTmp  :		��Ҫ�����XML��
- LockMode:     ��ѡ, ȷ���� Recordset ʱӦ��ʹ�õ���������(����)
�� LockTypeEnum ֵ, ��Ϊ���г���֮һ.
[����]				[˵��] 
-----------------------------------------------
adLockReadOnly		ֻ�� - ���ܸı�����. 
adLockPessimistic	����ʽ���� - ͨ��ͨ���ڱ༭ʱ������������Դ�ļ�¼. 
adLockOptimistic	����ʽ���� - ֻ�ڵ��� Update ����ʱ��������¼. 
adLockBatchOptimistic ����ʽ������ - ����������ģʽ(����������ģʽ
���). 
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
		TRACE(_T("Warning: Load �����쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		//::CoUninitialize();
		return FALSE;
	}
}

/*���ݼ������ļ�Ϊ�ļ���ʽ,Ĭ��ΪADTG��ʽ*/
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
		TRACE(_T("Warning: Save �����쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return FALSE;
}

/*���ļ����������ݼ�*/
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
		TRACE(_T("Warning: Load �����쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

/*========��ȡXML������=======*/
_StreamPtr CAdoRecordSet::GetXMLStream()
{
	return m_pXmlStream;
}

/*==================XML�������===============================*/

////���������2005.8.16���������ܣ���BLOB��ȡ������

/*########################################################################
------------------------------------------------
������׷�ӵ������ı������������� Field ����. 
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

		//�洢���뿪ʼ,��ȡ����������SafeArray����
		for (long i = 0; i < (long)nBytes; i++)
		{
			UCHAR &chData	= ((UCHAR*)lpData)[i];
			HRESULT hr = SafeArrayPutElement(pSafeArray, &i, &chData);
			if (FAILED(hr))	return FALSE;
		}

		//��SaveArray�������varChunk
		_variant_t varChunk;
		varChunk.vt = VT_ARRAY | VT_UI1;
		varChunk.parray = pSafeArray;

		return (pField->AppendChunk(varChunk) == S_OK);
	}
	catch (_com_error &e)
	{
		TRACE(_T("Warning: AppendChunk ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

/*-----------������׷�ӵ������ı������������� Field ����-------------
params: index	: ָ���ֶ�����ֵ
lpData	: ָ����������ݵ�ָ��
nBytes	: ���������ݵĳ���
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

/*-----------------------���ָ���ֶεĴ��ı�������������-----------------------
Params: strFileIdName : ָ��������ֶ���
lpData		  : ָ����������ݵ���
nBytes		  : ���������ݵĳ���
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

/*------------------��ָ���������ֶ���ӣ��������ļ�(��ý���ļ�)----------
params:		strFileIdName : ָ�����ֶ�������ֵ����Ϊ����������
lpszFileName  :	�������ļ���
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

/*------------------��ָ���������ֶ���ӣ��������ļ�(��ý���ļ�)----------
params:		strFileIdName : ָ��������ֶ�������Ϊ����������
lpszFileName  :	�������ļ���
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

/*-----------------------��ö������ֶζ���������----------
params:	pField	: �ֶ�ָ��
lpData	: ����������ָ��
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
			//��ȡ����,��100�ֽ�Ϊ��λ
			_variant_t varChunk = pField->GetChunk(100);
			if (varChunk.vt != (VT_ARRAY | VT_UI1))
			{
				return FALSE;
			}
			//������˳����lpData�ռ�
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
			TRACE(_T("Warning: GetChunk ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
			return FALSE;
		}
	}

	return TRUE;
}

/*------------���ָ���ֶεĶ���������(����)-----------
params :	index	: �ֶ�����ֵ
lpData	: �洢�Ķ��������ݵ�ַ
------------------------------------------------------*/
BOOL CAdoRecordSet::GetChunk(int index, LPVOID lpData)
{
	CADOFieldInfo fldInfo;
	GetFieldInfo(index,&fldInfo);
	if (adFldLong & fldInfo.m_lAttributes)
		return  GetChunk(GetField(index), lpData);
	else return FALSE;
}

/*-----------------------��ȡָ���ֶεĶ���������-------------
params:		strFieldName : Ҫ�������ݵĶ������ֶ���
lpData		 : ���������ݵ��׵�ַ
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

/*-----------------�ֶεļ�¼����λͼ�ļ�-------------------------
params : strFieldName	: Ҫ��ȡ���ֶ���
bitmap			: Ҫ������ļ���
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

			// �Ƿ���λͼ ----------------------------------------
			if (bmpHeader.bfType == (*(WORD*)"BM"))
			{
				BYTE* lpDIBBits = lpData + bmfHeaderLen;
				BITMAPINFOHEADER &bmpiHeader = *(LPBITMAPINFOHEADER)lpDIBBits;
				BITMAPINFO &bmpInfo = *(LPBITMAPINFO)lpDIBBits;
				lpDIBBits = lpData + ((BITMAPFILEHEADER *)lpData)->bfOffBits;

				// ����λͼ --------------------------------------
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
Name:	ȡ��ָ���е��ֶζ����ָ��.	
==========================================================================*/
FieldPtr CAdoRecordSet::GetField(long lIndex)
{
	try
	{
		return GetFields()->GetItem(_variant_t(lIndex));
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetField�����쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return NULL;
	}
}

/*==============����ָ���ֶε��ֶ�ָ��================*/
FieldPtr CAdoRecordSet::GetField(LPCTSTR lpszFieldName)
{
	try
	{
		return GetFields()->GetItem(_variant_t(lpszFieldName));
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetField�����쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return NULL;
	}
}

/*========================================================================
Remarks:	Recordset ������� Field ������ɵ� Fields ����. ÿ��Field
�����Ӧ Recordset ���е�һ��.
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
		TRACE(_T("Warning: GetFields ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return NULL;
	} 
	return NULL;
}

/*========================================================================
Remarks:	ȡ��ָ�����ֶε��ֶ���.
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
		TRACE(_T("Warning: GetFieldName ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
	}
	return strFieldName;
}

/*========================================================================
name:		ȡ�� Field ����һ����������.
----------------------------------------------------------
returns:	���� Field ����, Attributes ����Ϊֻ��, ��ֵ����Ϊ��������
һ������ FieldAttributeEnum ֵ�ĺ�.
[����]				[˵��] 
-------------------------------------------
adFldMayDefer			ָʾ�ֶα��ӳ�, ������ӵ��������¼������Դ����
�ֶ�ֵ, ������ʽ������Щ�ֶ�ʱ�Ž��м���. 
adFldUpdatable		ָʾ����д����ֶ�. 
adFldUnknownUpdatable ָʾ�ṩ���޷�ȷ���Ƿ����д����ֶ�. 
adFldFixed			ָʾ���ֶΰ�����������. 
adFldIsNullable		ָʾ���ֶν��� Null ֵ. 
adFldMayBeNull		ָʾ���ԴӸ��ֶζ�ȡ Null ֵ. 
adFldLong				ָʾ���ֶ�Ϊ���������ֶ�. ��ָʾ����ʹ�� AppendChunk 
�� GetChunk ����. 
adFldRowID			ָʾ�ֶΰ����־õ��б�ʶ��, �ñ�ʶ���޷���д��
���ҳ��˶��н��б�ʶ(���¼�š�Ψһ��ʶ����)�ⲻ
�����������ֵ. 
adFldRowVersion		ָʾ���ֶΰ����������ٸ��µ�ĳ��ʱ������ڱ��. 
adFldCacheDeferred	ָʾ�ṩ�߻������ֶ�ֵ, ����������Ի���Ķ�ȡ. 
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
		TRACE(_T("Warning: GetFieldAttributes ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: GetFieldAttributes ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return -1;
	}
}

/*========================================================================
Name:		ָʾ Field ����������ĳ���.
----------------------------------------------------------
returns:	����ĳ���ֶζ���ĳ���(���ֽ���)�ĳ�����ֵ.
----------------------------------------------------------
Remarks:	ʹ�� DefinedSize ���Կ�ȷ�� Field �������������.
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
		TRACE(_T("Warning: GetDefineSize ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: GetDefineSize ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return -1;
	}
}

/*========================================================================
Name:	ȡ���ֶε�ֵ��ʵ�ʳ���.
----------------------------------------------------------
returns:	���س�����ֵ.ĳЩ�ṩ���������ø������Ա�Ϊ BLOB ����Ԥ��
�ռ�, �ڴ������Ĭ��ֵΪ 0.
----------------------------------------------------------
Remarks:	ʹ�� ActualSize ���Կɷ��� Field ����ֵ��ʵ�ʳ���.��������
�ֶ�,ActualSize ����Ϊֻ��.��� ADO �޷�ȷ�� Field ����ֵ��ʵ
�ʳ���, ActualSize ���Խ����� adUnknown.
�����·�����ʾ, ActualSize ��  DefinedSize ����������ͬ: 
adVarChar ������������󳤶�Ϊ 50 ���ַ��� Field ���󽫷���Ϊ 
50 �� DefinedSize ����ֵ, ���Ƿ��ص� ActualSize ����ֵ�ǵ�ǰ��
¼���ֶ��д洢�����ݵĳ���.
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
		TRACE(_T("Warning: GetFieldActualSize ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: GetFieldActualSize ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return -1;
	}	
}

/*========================================================================
returns:	��������ֵ֮һ. ��Ӧ�� OLE DB ���ͱ�ʶ�����±��˵��������
���и���.
[����]			[˵��] 
---------------------------------------------------------
adArray			����������һ������߼� OR ��ָʾ���������������͵�
��ȫ���� (DBTYPE_ARRAY). 
adBigInt			8 �ֽڴ����ŵ����� (DBTYPE_I8). 
adBinary			������ֵ (DBTYPE_BYTES). 
adBoolean			������ֵ (DBTYPE_BOOL). 
adByRef			����������һ������߼� OR ��ָʾ������������������
�ݵ�ָ�� (DBTYPE_BYREF). 
adBSTR			�Կս�β���ַ��� (Unicode) (DBTYPE_BSTR). 
adChar			�ַ���ֵ (DBTYPE_STR). 
adCurrency		����ֵ (DBTYPE_CY).�������ֵ�С����λ�ù̶���С��
���Ҳ�����λ����.��ֵ����Ϊ 8 �ֽڷ�ΧΪ10,000 �Ĵ���
������ֵ. 
adDate			����ֵ (DBTYPE_DATE).���ڰ�˫��������ֵ������, ��
��ȫ����ʾ�� 1899 �� 12 �� 30 ��ʼ��������.С��������
һ�쵱�е�Ƭ��ʱ��. 
adDBDate			����ֵ (yyyymmdd) (DBTYPE_DBDATE). 
adDBTime			ʱ��ֵ (hhmmss) (DBTYPE_DBTIME). 
adDBTimeStamp		ʱ��� (yyyymmddhhmmss �� 10 �ڷ�֮һ��С��)(DBTYPE_DBTIMESTAMP). 
adDecimal			���й̶����Ⱥͷ�Χ�ľ�ȷ����ֵ (DBTYPE_DECIMAL). 
adDouble			˫���ȸ���ֵ (DBTYPE_R8). 
adEmpty			δָ��ֵ (DBTYPE_EMPTY). 
adError			32 - λ������� (DBTYPE_ERROR). 
adGUID			ȫ��Ψһ�ı�ʶ�� (GUID) (DBTYPE_GUID). 
adIDispatch		OLE ������ Idispatch �ӿڵ�ָ�� (DBTYPE_IDISPATCH). 
adInteger			4 �ֽڵĴ��������� (DBTYPE_I4). 
adIUnknown		OLE ������ IUnknown �ӿڵ�ָ�� (DBTYPE_IUNKNOWN).
adLongVarBinary	��������ֵ. 
adLongVarChar		���ַ���ֵ. 
adLongVarWChar	�Կս�β�ĳ��ַ���ֵ. 
adNumeric			���й̶����Ⱥͷ�Χ�ľ�ȷ����ֵ (DBTYPE_NUMERIC). 
adSingle			�����ȸ���ֵ (DBTYPE_R4). 
adSmallInt		2 �ֽڴ��������� (DBTYPE_I2). 
adTinyInt			1 �ֽڴ��������� (DBTYPE_I1). 
adUnsignedBigInt	8 �ֽڲ����������� (DBTYPE_UI8). 
adUnsignedInt		4 �ֽڲ����������� (DBTYPE_UI4). 
adUnsignedSmallInt 2 �ֽڲ����������� (DBTYPE_UI2). 
adUnsignedTinyInt 1 �ֽڲ����������� (DBTYPE_UI1). 
adUserDefined		�û�����ı��� (DBTYPE_UDT). 
adVarBinary		������ֵ. 
adVarChar			�ַ���ֵ. 
adVariant			�Զ������� (DBTYPE_VARIANT). 
adVector			����������һ������߼� OR ��, ָʾ������ DBVECTOR 
�ṹ(�� OLE DB ����).�ýṹ����Ԫ�صļ������������� 
(DBTYPE_VECTOR) ���ݵ�ָ��. 
adVarWChar		�Կս�β�� Unicode �ַ���. 
adWChar			�Կս�β�� Unicode �ַ��� (DBTYPE_WSTR). 
----------------------------------------------------------
Remarks:	����ָ���ֶε���������.
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
		TRACE(_T("Warning: GetField ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: GetField�����쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return adEmpty;
	}	
}

/*========================================================================
name:		Ϊ Recordset �е�����ָ��ɸѡ����.
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
		TRACE(_T("Warning: SetFilter ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

/*=========================��ȡ����״̬====================================

Name:		ȡ�ö����״̬(ͬ Recordset ����� GetState ����).
-----------------------------------------------------
returns:	�������г���֮һ�ĳ�����ֵ.
[����]				[˵��] 
----------------------------------
adStateClosed		ָʾ�����ǹرյ�. 
adStateOpen			ָʾ�����Ǵ򿪵�. 
-----------------------------------------------------
Remarks:		������ʱʹ�� State ����ȡ��ָ������ĵ�ǰ״̬.
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
		TRACE(_T("Warning: GetState �����쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return -1;
	} 
	return -1;
}

/*=======================��ȡ��¼������==================================

Name:		ȡ�ü�¼�������״̬(�Ǵ�״̬���ǹر�״̬). ���첽��ʽִ
�е� Recordset ����, ��˵����ǰ�Ķ���״̬�����ӡ�ִ�л��ǻ�ȡ״̬.
-----------------------------------------------------
returns:	�������г���֮һ�ĳ�����ֵ.
[����]				[˵��] 
----------------------------------
adStateClosed		ָʾ�����ǹرյ�. 
adStateOpen			ָʾ�����Ǵ򿪵�. 
adStateConnecting	ָʾ Recordset ������������. 
adStateExecuting	ָʾ Recordset ��������ִ������. 
adStateFetching		ָʾ Recordset ����������ڱ���ȡ. 
-----------------------------------------------------
Remarks:	 ������ʱʹ�� State ����ȷ��ָ������ĵ�ǰ״̬. ��������ֻ
����. Recordset ����� State ���Կ��������ֵ. ����: �������ִ�����,
�����Խ��� adStateOpen �� adStateExecuting �����ֵ.
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
		TRACE(_T("Warning: GetState ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return -1;
	} 
	return -1;
}

/*========================================================================
name:	���������� Recordset ������ͬ�ĸ��� Recordset ���󡣿�ѡ��ָ��
�ø���Ϊֻ����
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
		TRACE(_T("Warning: Clone ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

/*========================================================================
name:		����Ψһ��ʶ Recordset �����е�ǰ��¼����ǩ��
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
		TRACE(_T("Warning: GetBookmark ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: MoveFirst ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: MoveNext ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: MoveLast ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: MovePrevious ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		dump_com_error(e);
		return FALSE;
	}
	return FALSE;
}

/*========================================================================
Name:		�ƶ� Recordset �����е�ǰ��¼��λ��.
----------------------------------------------------------
Params:		
- lRecords    �����ų����ͱ��ʽ, ָ����ǰ��¼λ���ƶ��ļ�¼��.
- Start    ��ѡ, �ַ����������, ���ڼ�����ǩ. Ҳ��Ϊ���� 
BookmarkEnum ֵ֮һ: 
[����]				[˵��] 
--------------------------------
adBookmarkCurrent	Ĭ��. �ӵ�ǰ��¼��ʼ. 
adBookmarkFirst		���׼�¼��ʼ. 
adBookmarkLast		��β��¼��ʼ. 
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
		TRACE(_T("Warning: Move ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return	FALSE;
}

/*========================================================================
Name:		�����й����������д�����.
----------------------------------------------------------
Params:		AffectRecords   ��ѡ, AffectEnum ֵ. ���� UpdateBatch ����
��Ӱ��ļ�¼��Ŀ.����Ϊ���³���֮һ.
[����]				[˵��] 
------------------------------------
adAffectCurrent		ֻд�뵱ǰ��¼�Ĺ������. 
adAffectGroup		д�����㵱ǰ Filter �������õļ�¼�������Ĺ���
����. ���뽫 Filter ��������Ϊĳ����Ч��Ԥ���峣������ʹ�ø�ѡ��. 
adAffectAll			(Ĭ��ֵ). д�� Recordset ���������м�¼�Ĺ���
����, �������ڵ�ǰ Filter �������ö����ص��κμ�¼. 
adAffectAllChapters д�������Ӽ��Ĺ������. 
----------------------------------------------------------
Remarks:	��������ģʽ�޸� Recordset ����ʱ, ʹ�� UpdateBatch ������
�� Recordset �����е����и��Ĵ��ݵ��������ݿ�.
��� Recordset ����֧��������, ��ô���Խ�һ��������¼�Ķ��ظ��Ļ�����
����, Ȼ���ٵ��� UpdateBatch ����. ����ڵ��� UpdateBatch ����ʱ���ڱ�
����ǰ��¼��������µļ�¼, ��ô�ڽ������´��͵��ṩ��֮ǰ, ADO ���Զ�
���� Update ��������Ե�ǰ��¼�����й������.
ֻ�ܶԼ�����̬�α�ʹ��������.
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
		TRACE(_T("Warning: UpdateBatch ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		dump_com_error(e);
		return FALSE;
	}
	return	FALSE;
}

/*================DataProvide���ݼ�����Buffer====================
Param 1.char* &buffer : ��������ַ
��2.size		:��������С
 ע������������ֻҪ�ڵ��ô����塢��ʼ��
 ================================================================*/
BOOL CAdoRecordSet::SaveToBuffer(char* &buffer,LONG &size)
{
	buffer = NULL;
	size = 0;
	if( NULL == this->m_pRecordset || !this->IsOpen() ){
		return FALSE;
	}
	ASSERT(NULL == buffer);

	if(this->SaveToBinary())	//�������ݼ�-->������ת������
	{
		size = m_pXmlStream->Size;	//��ö�����������
		_variant_t varData;
		buffer = new char[size+1];	//�����������ռ�
		memset(buffer, 0x0, size+1);
		char *pBuf=NULL;
		m_pXmlStream->Position = 0;
		varData = m_pXmlStream->Read(adReadAll);	//������������_variant_t����

		SafeArrayAccessData(varData.parray,(void**)&pBuf);//����ע��

		memcpy(buffer,varData.parray->pvData,size);		//�������������������

		SafeArrayUnaccessData(varData.parray);	//����ע��
		m_pXmlStream->Close();			//�ر�������
		return true;
	}
	return false;
}

/*================DataProvide Buffer�������ݼ�=================
Param 1.char* &buffer : ��������ַ
==============================================================*/
BOOL CAdoRecordSet::LoadBuffer(char* &buffer,LONG size)
{
	ASSERT(NULL != buffer);
	variant_t varOptional(DISP_E_PARAMNOTFOUND,VT_ERROR);

	m_pXmlStream->PutType(adTypeBinary);//����Ϊ������
	m_pXmlStream->Open( varOptional,adModeUnknown, adOpenStreamUnspecified, _bstr_t(), _bstr_t());//���������ʼ��

	/*���ջ����ʼ��*/
	char *pBuf=NULL;
	_variant_t varData;
	varData.vt = 8209;
	varData.parray = SafeArrayCreateVector(VT_UI1,0,size);
	/*�����ʼ������*/

	SafeArrayAccessData(varData.parray,(void **)&pBuf);//��䲻̫ȷ����Գ����ȶ��з�Ӱ��,����ע��

	memcpy(varData.parray->pvData,buffer,size);		//�ӻ���õ�����������

	SafeArrayUnaccessData (varData.parray);//��䲻̫ȷ����Գ����ȶ��з�Ӱ��,����ע��

	//ADD:here
	m_pXmlStream->Write(&varData);	//XML��������������
	m_pXmlStream->Position = 0;
	if(!LoadXMLOrBuffer(m_pXmlStream))	//���ݼ�����XML������
	{
		m_pXmlStream->Close();
		return false;
	}

	m_pXmlStream->Close();
	//m_pRecordset->PutActiveConnection(_variant_t((IDispatch*)m_pConnection));	//���¼�������
	return true;
}

/*########################################################################
------------------------------------------------
�����ֶε�ֵ
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
		TRACE(_T("Warning: PutCollect ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: PutCollect ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
	} 
	return	FALSE;
}

BOOL CAdoRecordSet::PutCollect(long index, const bool &value)
{
	ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
	if (GetFieldType(index) != adBoolean)
		TRACE(_T("Warning: ��Ҫ�������ֶ���������������Ͳ���; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
#endif

	return PutCollect(index, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const bool &value)
{
	ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
	if (GetFieldType(strFieldName) != adBoolean)
		TRACE(_T("Warning: ��Ҫ�������ֶ���������������Ͳ���; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
#endif

	return PutCollect(strFieldName, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(long index, const BYTE &value)
{
	ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
	if (GetFieldType(index) != adUnsignedTinyInt)
		TRACE(_T("Warning: ��Ҫ�������ֶ���������������Ͳ���; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
#endif

	return PutCollect(index, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const BYTE &value)
{
	ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
	if (GetFieldType(strFieldName) != adUnsignedTinyInt)
		TRACE(_T("Warning: ��Ҫ�������ֶ���������������Ͳ���; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
#endif

	return PutCollect(strFieldName, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(long index, const short &value)
{
	ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
	if (GetFieldType(index) != adSmallInt)
		TRACE(_T("Warning: ��Ҫ�������ֶ���������������Ͳ���; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
#endif

	return PutCollect(index, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const short &value)
{
	ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
	if (GetFieldType(strFieldName) != adSmallInt)
		TRACE(_T("Warning: ��Ҫ�������ֶ���������������Ͳ���; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
#endif

	return PutCollect(strFieldName, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(long index, const int &value)
{
	ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
	if (GetFieldType(index) != adInteger)
		TRACE(_T("Warning: ��Ҫ�������ֶ���������������Ͳ���; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
#endif

	return PutCollect(index, _variant_t(long(value)));
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const int &value)
{
	ASSERT(m_pRecordset != NULL);

#ifdef _DEBUG
	if (GetFieldType(strFieldName) != adInteger)
		TRACE(_T("Warning: ��Ҫ�������ֶ���������������Ͳ���; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
#endif

	return PutCollect(strFieldName, _variant_t(long(value)));
}

BOOL CAdoRecordSet::PutCollect(long index, const long &value)
{
	ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
	if (GetFieldType(index) != adBigInt)
		TRACE(_T("Warning: ��Ҫ�������ֶ���������������Ͳ���; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
#endif

	return PutCollect(index, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const long &value)
{
	ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
	if (GetFieldType(strFieldName) != adBigInt)
		TRACE(_T("Warning: ��Ҫ�������ֶ���������������Ͳ���; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
#endif

	return PutCollect(strFieldName, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(long index, const DWORD &value)
{
	ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
	if (GetFieldType(index) != adUnsignedBigInt)
		TRACE(_T("Warning: ��Ҫ�������ֶ���������������Ͳ���; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
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
		TRACE(_T("Warning: ��Ҫ�������ֶ���������������Ͳ���; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
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
		TRACE(_T("Warning: ��Ҫ�������ֶ���������������Ͳ���; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
#endif

	return PutCollect(index, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const float &value)
{
	ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
	if (GetFieldType(strFieldName) != adSingle)
		TRACE(_T("Warning: ��Ҫ�������ֶ���������������Ͳ���; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
#endif

	return PutCollect(strFieldName, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(long index, const double &value)
{
	ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
	if (GetFieldType(index) != adDouble)
		TRACE(_T("Warning: ��Ҫ�������ֶ���������������Ͳ���; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
#endif

	return PutCollect(index, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const double &value)
{
	ASSERT(m_pRecordset != NULL);
#ifdef _DEBUG
	if (GetFieldType(strFieldName) != adDouble)
		TRACE(_T("Warning: ��Ҫ�������ֶ���������������Ͳ���; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
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
		TRACE(_T("Warning: ��Ҫ�������ֶ���������������Ͳ���; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
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
		TRACE(_T("Warning: ��Ҫ�������ֶ���������������Ͳ���; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
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
		TRACE(_T("Warning: ��Ҫ�������ֶ���������������Ͳ���; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
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
		TRACE(_T("Warning: ��Ҫ�������ֶ���������������Ͳ���; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
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
		TRACE(_T("Warning: ��Ҫ�������ֶ���������������Ͳ���; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
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
		TRACE(_T("Warning: ��Ҫ�������ֶ���������������Ͳ���; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
#endif

	_variant_t vt;
	vt.vt = value.IsEmpty() ? VT_NULL : VT_BSTR;
	vt.bstrVal = value.AllocSysString();
	return PutCollect(strFieldName, vt);
}

/*########################################################################
------------------------------------------------
��¼������
------------------------------------------------
########################################################################*/

/*========================================================================
Name:		ȡ�ü�¼�������״̬(�Ǵ�״̬���ǹر�״̬). ���첽��ʽִ
�е� Recordset ����, ��˵����ǰ�Ķ���״̬�����ӡ�ִ�л��ǻ�ȡ״̬.
-----------------------------------------------------
returns:	�������г���֮һ�ĳ�����ֵ.
[����]				[˵��] 
----------------------------------
adStateClosed		ָʾ�����ǹرյ�. 
adStateOpen			ָʾ�����Ǵ򿪵�. 
adStateConnecting	ָʾ Recordset ������������. 
adStateExecuting	ָʾ Recordset ��������ִ������. 
adStateFetching		ָʾ Recordset ����������ڱ���ȡ. 
-----------------------------------------------------
Remarks:	 ������ʱʹ�� State ����ȷ��ָ������ĵ�ǰ״̬. ��������ֻ
����. Recordset ����� State ���Կ��������ֵ. ����: �������ִ�����,
�����Խ��� adStateOpen �� adStateExecuting �����ֵ.
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
		TRACE(_T("Warning: GetState ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return -1;
	} 
	return -1;
}


/*========================================================================
Name:		ָʾ�й������»��������������ĵ�ǰ��¼��״̬.
-----------------------------------------------------
returns:	��������һ������ RecordStatusEnum ֵ֮��.
[����]						[˵��]
-------------------------------------------------
adRecOK						�ɹ��ظ��¼�¼. 
adRecNew					��¼���½���. 
adRecModified				��¼���޸�. 
adRecDeleted				��¼��ɾ��. 
adRecUnmodified				��¼û���޸�. 
adRecInvalid				������ǩ��Ч, ��¼û�б���. 
adRecMultipleChanges		����Ӱ������¼, ��˼�¼δ������. 
adRecPendingChanges			���ڼ�¼���ù���Ĳ���, ���δ������. 
adRecCanceled				���ڲ�����ȡ��, δ�����¼. 
adRecCantRelease			�������м�¼����, û�б����¼�¼. 
adRecConcurrencyViolation	���ڿ���ʽ������ʹ����, ��¼δ������. 
adRecIntegrityViolation		�����û�Υ��������Լ��, ��¼δ������. 
adRecMaxChangesExceeded		���ڴ��ڹ���������, ��¼δ������. 
adRecObjectOpen				������򿪵Ĵ�������ͻ, ��¼δ������. 
adRecOutOfMemory			���ڼ�����ڴ治��, ��¼δ������. 
adRecPermissionDenied		�����û�û���㹻��Ȩ��, ��¼δ������. 
adRecSchemaViolation		���ڼ�¼Υ���������ݿ�Ľṹ, ���δ������. 
adRecDBDeleted				��¼�Ѿ�������Դ��ɾ��. 
-----------------------------------------------------
Remarks:	ʹ�� Status ���Բ鿴���������б��޸ĵļ�¼����Щ���ı�����.
Ҳ��ʹ�� Status ���Բ鿴��������ʱʧ�ܼ�¼��״̬. ����, ���� Recordset
����� Resync��UpdateBatch �� CancelBatch ����, �������� Recordset ����
�� Filter ����Ϊ��ǩ����. ʹ�ø�����, �ɼ��ָ����¼Ϊ��ʧ�ܲ��������
��.
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
		TRACE(_T("Warning: GetStatus ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return -1;
	} 
	return -1;
}

/*########################################################################
------------------------------------------------
��ȡ�ֶε�ֵ
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
			TRACE(_T("Warning: �޷���ȡ��Ӧ���ֶ�, �������Ͳ�ƥ��; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
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
			TRACE(_T("Warning: �޷���ȡ��Ӧ���ֶ�, �������Ͳ�ƥ��; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
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
			TRACE(_T("Warning: �޷���ȡ��Ӧ���ֶ�, �������Ͳ�ƥ��; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
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
			TRACE(_T("Warning: �޷���ȡ��Ӧ���ֶ�, �������Ͳ�ƥ��; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
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
			MessageBox(NULL, _T("���ݿ�����Ѿ��Ͽ�,\r\n���������ӡ�Ȼ������."), _T("��ʾ"), MB_ICONINFORMATION);
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
		TRACE(_T("Warning: �ֶη���ʧ��. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
MessageBox(NULL, _T("���ݿ�����Ѿ��Ͽ�,\r\n���������ӡ�Ȼ������."), _T("��ʾ"), MB_ICONINFORMATION);
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
TRACE(_T("Warning: �ֶη���ʧ��. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		AfxMessageBox("Parameter ���󴴽�ʧ��! ��ȷ���Ƿ��ʼ����Com����.");
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
		AfxMessageBox("Parameter ���󴴽�ʧ��! ��ȷ���Ƿ��ʼ����Com����.");
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
Name:		ָʾ�� Parameter ����������ֵ������ Field ����ľ��ȡ�
----------------------------------------------------------
Params:		���û򷵻� Byte ֵ��������ʾֵ�����λ������ֵ�� Parameter
������Ϊ��/д������ Field ������Ϊֻ����
----------------------------------------------------------
Remarks:	ʹ�� Precision ���Կ�ȷ����ʾ���� Parameter �� Field ����ֵ
�����λ��
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
		TRACE(_T("Warning: SetPrecision ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

/*========================================================================
Name:		ָ�� Parameter �� Field ����������ֵ�ķ�Χ��
----------------------------------------------------------
Params:		���û򷵻��ֽ�ֵ��ָʾ����ֵ����ȷ����С����λ����
----------------------------------------------------------
Remarks:	ʹ�� NumericScale ���Կ�ȷ�����ڱ��������� Parameter �� Field 
�����ֵ��С��λ����
���� Parameter ����NumericScale ����Ϊ��/д������ Field ����
NumericScale ����Ϊֻ����

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
		TRACE(_T("Warning: SetPrecision ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}	
}

/*========================================================================
Name:		ָʾ Parameter �������������.
----------------------------------------------------------
Params:		[DataType]: DataTypeEnum ����ֵ, ��ο� CRecordSet �����
����.
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
		TRACE(_T("Warning: SetType ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: SetDirection ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return adEmpty;
	}
}

/*========================================================================
Name:		��ʾ Parameter ���������С�����ֽڻ��ַ�����
----------------------------------------------------------
Params:		[size]: ��ʾ Parameter ���������С�����ֽڻ��ַ����ĳ�
����ֵ��
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
		TRACE(_T("Warning: SetSize ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: SetDirection ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return -1;
	}
}

/*========================================================================
Name:		ָʾ��������ơ�
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
		TRACE(_T("Warning: SetName ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: SetName ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return CString(_T(""));
	}
}

/*========================================================================
Name:		ָʾ Parameter �����������������������������Ǽ��������
�������������ò����Ƿ�Ϊ�洢���̷��ص�ֵ��
----------------------------------------------------------
Params:		[Direction]: ��������ĳ�� ParameterDirectionEnum ֵ��
[����]				[˵��] 
-------------------------------------------
AdParamUnknown		ָʾ��������δ֪�� 
AdParamInput		Ĭ��ֵ��ָʾ��������� 
AdParamOutput		ָʾ��������� 
AdParamInputOutput	ͬʱָʾ������������������ 
AdParamReturnValue	ָʾ����ֵ�� 
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
		TRACE(_T("Warning: SetDirection ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: SetDirection ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: GetValue ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: SetValue ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: SetValue ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: SetValue ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: SetValue ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: SetValue ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: SetValue ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: SetValue ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: SetValue ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: SetValue ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: GetValue ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: GetValue ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: GetValue ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: GetValue ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: GetValue ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: GetValue ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: GetValue ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: GetValue ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
	///���� Connection ����---------------------------
	m_pCommand.CreateInstance("ADODB.Command");
#ifdef _DEBUG
	if (m_pCommand == NULL)
	{
		AfxMessageBox("Command ���󴴽�ʧ��! ��ȷ���Ƿ��ʼ����Com����.");
	}
#endif
	ASSERT(m_pCommand != NULL);
}

CAdoCommand::CAdoCommand(CAdoConnection* pAdoConnection, CString strCommandText, CommandTypeEnum CommandType)
{
	///���� Connection ����---------------------------
	m_pCommand.CreateInstance("ADODB.Command");
#ifdef _DEBUG
	if (m_pCommand == NULL)
	{
		AfxMessageBox("Command ���󴴽�ʧ��! ��ȷ���Ƿ��ʼ����Com����.");
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
		TRACE(_T("Warning: Release���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: Execute ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: Cancel ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: CreateParameter ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: PutCommandText ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: SetConnection ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
}
/*========================================================================
Name:		ָʾ Command ��������͡�
----------------------------------------------------------
returns:	��������ĳ�� CommandTypeEnum ��ֵ.
[����]				 [˵��] 
----------------------------------
adCmdText			ָʾstrSQLΪ�����ı�, ����ͨ��SQL���. 
adCmdTable			ָʾADO����SQL��ѯ������ strSQL �����ı��е�
������. 
adCmdTableDirect	ָʾ�����ĸ�����strSQL�������ı��з���������. 
adCmdStoredProc		ָʾstrSQLΪ�洢����. 
adCmdUnknown		ָʾstrSQL�����е���������Ϊδ֪. 
adCmdFile			ָʾӦ����strSQL���������ļ��лָ�����(�����)
Recordset. 
adAsyncExecute		ָʾӦ�첽ִ��strSQL. 
adAsyncFetch		ָʾ����ȡ Initial Fetch Size ������ָ���ĳ�ʼ
������, Ӧ���첽��ȡ����ʣ�����. ������������δ
��ȡ, ��Ҫ���߳̽�������ֱ�������¿���. 
adAsyncFetchNonBlocking ָʾ��Ҫ�߳�����ȡ�ڼ��δ����. ���������
������δ��ȡ, ��ǰ���Զ��Ƶ��ļ�ĩβ. 
----------------------------------------------------------
Remarks: ʹ�� CommandType ���Կ��Ż� CommandText ���Եļ��㡣
��� CommandType ���Ե�ֵ���� adCmdUnknown(Ĭ��ֵ), ϵͳ�����ܽ���
����, ��Ϊ ADO ��������ṩ����ȷ�� CommandText ������ SQL ��䡢���Ǵ�
�����̻������ơ����֪������ʹ�õ����������, ��ͨ������ CommandType 
����ָ�� ADO ֱ��ת����ش��롣��� CommandType ������ CommandText ��
���е��������Ͳ�ƥ��, ���� Execute ����ʱ����������
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
		TRACE(_T("Warning: PutCommandType ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: GetState ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: SetCommandTimeOut ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: SetCommandTimeOut ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: Append ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: GetParamter ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: GetParamter ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: GetValue ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: GetValue ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: operator [] ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
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
		TRACE(_T("Warning: operator [] ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
	}
	return pParameter;
}

