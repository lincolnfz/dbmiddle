
//软件环境:本软件是基于COM的需要MSADO15.DLL文件
//
//
/*本软件主要封装了CADODatabase和CADORecordset两大类,其中CADODatabase主要是针对数据源的操作.
而CADORecordse主要是针对数据集的操作

在使用本类之前必须要初始化 COM 环境, 可以调用 CoInitialize(NULL)  
来初始化, 用 CoUninitialize() 释放;
*/
/*
具体使用举例 
1.定义一个CADODatabase实例CADODatabase A;
2.建立连接 
A.open(lpstrConnection);
//便可打开数据源,其中lpstrConnection是连接字符串
3.打开记录集  
CADORecordset B(&A);
CString strSQL;
strSQL="select * from Table_Name"; 	
k.open(strSQL);
4.获得一个字段名为ID的值 
k.GetValueStr(m_strID,"ID");
5.给一个字段名为NAME的赋值
k.SetFieldValue(m_strNAME,"NAME");
6.添家加记录
K.AddNew();
...;//所要赋给的对应字段名的值
*/
//

#ifndef _ADO_H_
#define _ADO_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//#include "stdafx.h"

//将动态连接库引入工程
/*#pragma warning(disable:4146)
#import "C:\Program Files\Common Files\System\ADO\msado15.dll" rename_namespace("ADOCG") rename("EOF", "EndOfFile")
using namespace ADOCG;*/
#pragma warning(disable:4146)
#import "C:\Program Files\Common Files\System\ADO\msado15.dll" named_guids rename("EOF","adoEOF"), rename("BOF","adoBOF")
#pragma warning(default:4146)
using namespace ADODB; 

/*======为MSXML增加的=====2005.8===*/
#import "c:\windows\system32\msxml.dll"
using namespace MSXML;
/*======END=======*/

#include <string>
#include <icrsint.h>
#include <comutil.h>

struct CADOFieldInfo
{
	TCHAR m_strName[30]; 
	short m_nType;
	long m_lSize; 
	long m_lDefinedSize;
	long m_lAttributes;
	short m_nOrdinalPosition;
	BOOL m_bRequired;   
	BOOL m_bAllowZeroLength; 
	long m_lCollatingOrder;  
};

CString IntToStr(int nVal);

CString LongToStr(long lVal);

// 数值类型转换 -----------------------------------
COleDateTime vartodate(const _variant_t& var);
COleCurrency vartocy(const _variant_t& var);
bool vartobool(const _variant_t& var);
BYTE vartoby(const _variant_t& var);
short vartoi(const _variant_t& var);
long vartol(const _variant_t& var);
double vartof(const _variant_t& var);
CString vartostr(const _variant_t& var);

/*########################################################################
------------------------------------------------
CAdoConnection class
------------------------------------------------
########################################################################*/
class CAdoConnection
{
	// 构建/析构 ------------------------------------------
public:
	CAdoConnection();
	virtual ~CAdoConnection();

protected:
	void Release();

	// 属性 -----------------------------------------------
public:
	// 连接对象 ----------------------------------
	_ConnectionPtr& GetConnection() {return m_pConnection;};

	// 异常信息 ----------------------------------
	CString GetLastErrorText();
	ErrorsPtr GetErrors();
	ErrorPtr  GetError(long index);

	// 连接字串 ----------------------------------
	CString GetConnectionText() {return m_strConnect;}

	// 连接信息 ----------------------------------
	CString GetProviderName();
	CString GetVersion();
	CString GetDefaultDatabase();

	// 连接状态 ----------------------------------
	BOOL IsOpen();
	long GetState();

	// 连接模式 ----------------------------------
	ConnectModeEnum GetMode();
	BOOL SetMode(ConnectModeEnum mode);

	// 连接时间 ----------------------------------
	long GetConnectTimeOut();
	BOOL SetConnectTimeOut(long lTime = 5);

	// 数据源信息 -------------------------------
	_RecordsetPtr OpenSchema(SchemaEnum QueryType);

	// 操作 -----------------------------------------------
public:
	// 数据库连接 --------------------------------
	BOOL Open(LPCTSTR lpszConnect =_T(""), long lOptions = adConnectUnspecified);
	BOOL ConnectSQLServer(CString dbsrc, CString dbname, CString user, CString pass, long lOptions = adConnectUnspecified);
	BOOL ConnectAccess(CString dbpath, CString pass = _T(""), long lOptions = adConnectUnspecified);
	BOOL OpenUDLFile(LPCTSTR strFileName, long lOptions = adConnectUnspecified);
	BOOL ConnectMysql(CString dbsrc, CString sqlport, CString dbname, CString user, CString pass, long lOptions = adConnectUnspecified);
	void Close();
	BOOL ReConnect();

	// 处理 -----------------------------------------------
public:
	// 事务处理 ----------------------------------
	long BeginTrans();
	BOOL RollbackTrans();
	BOOL CommitTrans();

	// 执行 SQL 语句 ------------------------------
	_RecordsetPtr Execute(LPCTSTR strSQL,VARIANT* pRecordAfter=NULL, long lOptions = adCmdText);
	BOOL Cancel();

	// 数据 -----------------------------------------------
protected:
	CString			m_strConnect;
	_ConnectionPtr	m_pConnection;
	long			m_lOptions;
};


/*########################################################################
------------------------------------------------
CAdoDatabase class
------------------------------------------------
########################################################################*/
class CAdoDatabase
{
protected:
	_ConnectionPtr m_pConnection;
	CString m_strConnection;
	CString m_strLastError;
public:
	BOOL Execute(LPCTSTR lpstrExec);

	CAdoDatabase()
	{
		//初始化COM
		//::CoInitialize(NULL);

		m_pConnection = NULL;
		m_strConnection = _T("");
		//创建m_pConnection实例
		m_pConnection.CreateInstance(__uuidof(Connection));
	}

	~CAdoDatabase()
	{
		Close();
		m_pConnection.Release();
		m_pConnection = NULL;
		//::CoUninitialize();
	}

	//打开连接数据库
	BOOL Open(LPCTSTR lpstrConnection = _T(""));
	_ConnectionPtr GetActiveConnection() {return m_pConnection;};
	DWORD GetRecordCount(_RecordsetPtr m_pRs);
	long BeginTransaction() 
	{return m_pConnection->BeginTrans();};
	long CommitTransaction() 
	{return m_pConnection->CommitTrans();};
	long RollbackTransaction() 
	{return m_pConnection->RollbackTrans();};
	BOOL IsOpen();
	void Close();
	//设置连接字符串
	void SetConnectionString(LPCTSTR lpstrConnection)
	{m_strConnection = lpstrConnection;};
	//获得连接字符串
	CString GetConnectionString()
	{return m_strConnection;};
	CString GetLastError() 
	{return m_strLastError;};
protected:
	void dump_com_error(_com_error &e);

};

/*########################################################################
------------------------------------------------
CADORecordset class
------------------------------------------------
########################################################################*/
class CAdoRecordSet
{ 
protected:
	CAdoConnection* m_pConnection;
	LPCTSTR m_strConnect;

	//int m_nSearchDirection;
	SearchDirectionEnum m_SearchDirection;
	IADORecordBinding	*m_pAdoRecordBinding;

	CString m_strFind;
	CString m_strSQL;

	_variant_t m_varBookFind;

	int m_nEditStatus;

	CString m_strLastError;
	void dump_com_error(_com_error &e);

	_RecordsetPtr m_pRecordset;

	CString m_strQuery;
	_StreamPtr	m_pXmlStream;	/*这是为XML增加的数据流======2005.8======*/
public:

	_variant_t m_varBookmark;
	enum cadoOpenEnum
	{
		openUnknown = 0,
		openQuery = 1,
		openTable = 2,
		openStoredProc = 3
	};

	enum cadoEditEnum
	{
		dbEditNone = 0,
		dbEditNew = 1,
		dbEdit = 2
	};

	enum cadoPositionEnum
	{
		positionUnknown = -1,
		positionBOF = -2,
		positionEOF = -3
	};

	enum cadoSearchEnum
	{	
		searchForward = 1,
		searchBackward = -1
	};

	/////////////////////////////////////////////////////////////

	//获得记录指针
	_RecordsetPtr GetRecordsetPtr();

	//给对应的列字段赋值
	BOOL SetFieldValue(int nIndex, CString strValue);
	BOOL SetFieldValue(LPCTSTR lpFieldName, CString strValue);
	BOOL SetFieldValue(int nIndex, int nValue);
	BOOL SetFieldValue(LPCTSTR lpFieldName, int nValue);
	BOOL SetFieldValue(int nIndex, long lValue);
	BOOL SetFieldValue(LPCTSTR lpFieldName, long lValue);
	BOOL SetFieldValue(int nIndex, double dblValue);
	BOOL SetFieldValue(LPCTSTR lpFieldName, double dblValue);
	BOOL SetFieldValue(int nIndex, COleDateTime time);
	BOOL SetFieldValue(LPCTSTR lpFieldName, COleDateTime time);

	//对Bool变量赋值--------------2005.11新增
	BOOL SetFieldValue(int nIndex, bool value);
	BOOL SetFieldValue(LPCTSTR lpFieldName, bool value);

	// BLOB 数据存取 ---------------------------2005.8新增
	BOOL AppendChunk(FieldPtr pField, LPVOID lpData, UINT nBytes);
	BOOL AppendChunk(int index, LPVOID lpData, UINT nBytes);
	BOOL AppendChunk(LPCTSTR strFieldName, LPVOID lpData, UINT nBytes);
	BOOL AppendChunk(int index, LPCTSTR lpszFileName);
	BOOL AppendChunk(LPCTSTR strFieldName, LPCTSTR lpszFileName);

	BOOL GetChunk(FieldPtr pField, LPVOID lpData);
	BOOL GetChunk(int index, LPVOID lpData);
	BOOL GetChunk(LPCTSTR strFieldName, LPVOID lpData);
	BOOL GetChunk(int index, CBitmap &bitmap);
	BOOL GetChunk(LPCTSTR strFieldName, CBitmap &bitmap);
	BOOL GetChunkFile(LPCTSTR strFieldName, LPCTSTR strFileName);
	// BLOB 数据存取定义结束-----------------------------------

	// 字段对象 -----------------------------2005.8新增
	FieldPtr  GetField(long lIndex);
	FieldPtr  GetField(LPCTSTR lpszFieldName);
	long GetFieldsCount();

	// 字段名 -------------------------------
	CString GetFieldName(long lIndex);

	// 字段数据类型 -------------------------
	ADODB::DataTypeEnum GetFieldType(long lIndex);
	ADODB::DataTypeEnum GetFieldType(LPCTSTR lpszFieldName);

	// 字段属性 -----------------------------
	long  GetFieldAttributes(long lIndex);
	long  GetFieldAttributes(LPCTSTR lpszFieldName);

	// 字段定义长度 -------------------------
	long  GetFieldDefineSize(long lIndex);
	long  GetFieldDefineSize(LPCTSTR lpszFieldName);

	// 字段实际长度 -------------------------
	long  GetFieldActualSize(long lIndex);
	long  GetFieldActualSize(LPCTSTR lpszFieldName);
	//字段对像定义结束
	// 字段集 -------------------------------2005.8新增


	FieldsPtr GetFields();

	//取消操作
	BOOL CancelUpdate();
	BOOL Update();
	void Edit();
	BOOL AddNew();
	//查找
	BOOL Find(LPCTSTR lpszFind, SearchDirectionEnum SearchDirection = adSearchForward);
	BOOL FindFirst(LPCTSTR lpFind);
	BOOL FindNext();

	// 刷新记录集中的数据 ------------------
	BOOL Requery(long Options = adConnectUnspecified);
	BOOL Resync(AffectEnum AffectRecords = adAffectAll, ResyncEnum ResyncValues = adResyncAllValues);   

	BOOL AddNew(CADORecordBinding &pAdoRecordBinding);

	//记录移动
	//	BOOL MoveFirst();

	CAdoRecordSet();
	CAdoRecordSet(CAdoConnection *pConnection);
	~CAdoRecordSet();

	//获得查询表
	CString GetQuery() 
	{return m_strQuery;};
	void SetQuery(LPCTSTR strQuery) 
	{m_strQuery = strQuery;};

	// 连接对象 ----------------------------
	CAdoConnection* GetConnection() {return m_pConnection;}
	void SetAdoConnection(CAdoConnection *pConnection);

	//记录绑定数据
	BOOL RecordBinding(CADORecordBinding &pAdoRecordBinding);
	DWORD GetRecordCount();
	BOOL IsOpen();
	void Close();

	//打开所连接的对应表
	BOOL Open(LPCTSTR strSQL, long lOption = adCmdText, CursorTypeEnum CursorType = adOpenStatic, ADODB::LockTypeEnum LockType = adLockOptimistic);
	BOOL SetRecordset(_RecordsetPtr RecordsetPtr);

	//获得对应字段的值
	double GetDoubleFieldValue(LPCTSTR lpFieldName);
	double GetDoubleFieldValue(int nIndex);

	long GetLongFieldValue(LPCTSTR lpFieldName);
	long GetLongFieldValue(int nIndex);

	BOOL GetBOOLFieldValue(LPCTSTR lpFieldName);
	int GetIntFieldValue(LPCTSTR lpFieldName);
	int GetIntFieldValue(int nIndex);

	CString GetStringFieldValue(LPCTSTR lpFieldName);
	CString GetStringFieldValue(int nIndex);
	COleDateTime GetDateTimeFieldValue(LPCTSTR lpFieldName);
	COleDateTime GetDateTimeFieldValue(int nIndex);

	BOOL GetBoolFieldValue(long index);
	BOOL GetBoolFieldValue(LPCTSTR lpFieldName);

	BOOL IsFieldNull(LPCTSTR lpFieldName);
	BOOL IsFieldNull(int nIndex);
	BOOL IsFieldEmpty(LPCTSTR lpFieldName);
	BOOL IsFieldEmpty(int nIndex);	
	BOOL IsEOF();
	BOOL IsBOF();
	//记录的搜索
	BOOL MoveFirst();
	BOOL MoveNext();
	BOOL MovePrevious();
	BOOL MoveLast();
	BOOL Move(long lRecords, _variant_t Start = _variant_t((long)adBookmarkFirst));
	BOOL SetSort(LPCTSTR lpszCriteria);

	long GetAbsolutePage()
	{return m_pRecordset->GetAbsolutePage();};
	void SetAbsolutePage(int nPage)
	{m_pRecordset->PutAbsolutePage((enum PositionEnum)nPage);};
	long GetPageCount()
	{return m_pRecordset->GetPageCount();};
	long GetPageSize()
	{return m_pRecordset->GetPageSize();};
	void SetPageSize(int nSize)
	{m_pRecordset->PutPageSize(nSize);};
	long GetAbsolutePosition()
	{return m_pRecordset->GetAbsolutePosition();};
	void SetAbsolutePosition(int nPosition)
	{m_pRecordset->PutAbsolutePosition((enum PositionEnum)nPosition);};
	BOOL GetFieldInfo(LPCTSTR lpFieldName, CADOFieldInfo* fldInfo);
	BOOL GetFieldInfo(int nIndex, CADOFieldInfo* fldInfo);
	BOOL GetChunk(LPCTSTR lpFieldName, CString& strValue);
	CString GetString(LPCTSTR lpCols, LPCTSTR lpRows, LPCTSTR lpNull, long numRows = 0);
	CString GetLastError() {return m_strLastError;};
	_variant_t GetBookmark();
	BOOL SetBookmark(_variant_t varBookMark = _variant_t((long)adBookmarkFirst));
	BOOL Delete();
	BOOL Delete(AffectEnum AffectRecords);
	BOOL SaveToXMLStream(_StreamPtr &pstream);

	/*========================================================================
	name:	创建与现有 Recordset 对象相同的复制 Recordset 对象。可选择指定
	该副本为只读。
	==========================================================================*/
	BOOL Clone(CAdoRecordSet &pRecordSet);
	//	_RecordsetPtr operator = (_RecordsetPtr &pRecordSet);



	/*存为文件,默认为ADTG格式*/
	BOOL	SaveTOFile(LPCTSTR FileName,PersistFormatEnum PersistFormat = adPersistADTG);
	/*导入ADtg文件*/
	BOOL	LoadFile(LPCTSTR FileName,ADODB::LockTypeEnum LockMode = adLockBatchOptimistic);
	BOOL	SaveToXML();
	BOOL	SaveToBinary();
	BOOL	LoadXMLOrBuffer(_StreamPtr XmlTmp,ADODB::LockTypeEnum LockMode = adLockBatchOptimistic);
	_StreamPtr GetXMLStream();
	/*=============END====================*/

	/*=========过滤ADO数据集========*/
	BOOL SetFilter(LPCTSTR lpszFilter);

	/*=============获取数据集状态=========*/
	long GetDataSetState();
	ADODB::EditModeEnum GetEditMode();
	long GetState();
	long GetStatus();

	/*=======获取联接状态=======*/
	long GetConnetionState();

	//更新数据集
	BOOL	UpdateBatch(AffectEnum = adAffectAll);

	//保存数据集到缓冲池
	BOOL	SaveToBuffer(TCHAR* &buffer, unsigned long &size);

	//从缓立冲池读数据
	BOOL	LoadBuffer(TCHAR* &buffer,LONG size);

	/////////////////////////////////////////////////////////////////
	//新增另一种方法存储、读取字段值2007-08-23
	BOOL PutCollect(long index, const _variant_t &value);
	BOOL PutCollect(long index, const CString &value);
	BOOL PutCollect(long index, const double &value);
	BOOL PutCollect(long index, const float  &value);
	BOOL PutCollect(long index, const long   &value);
	BOOL PutCollect(long index, const DWORD  &value);
	BOOL PutCollect(long index, const int    &value);
	BOOL PutCollect(long index, const short  &value);
	BOOL PutCollect(long index, const BYTE   &value);
	BOOL PutCollect(long index, const bool   &value);
	BOOL PutCollect(long index, const COleDateTime &value);
	BOOL PutCollect(long index, const COleCurrency &value);

	BOOL PutCollect(LPCTSTR strFieldName, const _variant_t &value);
	BOOL PutCollect(LPCTSTR strFieldName, const CString &value);
	BOOL PutCollect(LPCTSTR strFieldName, const double &value);
	BOOL PutCollect(LPCTSTR strFieldName, const float  &value);
	BOOL PutCollect(LPCTSTR strFieldName, const long   &value);
	BOOL PutCollect(LPCTSTR strFieldName, const DWORD  &value);
	BOOL PutCollect(LPCTSTR strFieldName, const int    &value);
	BOOL PutCollect(LPCTSTR strFieldName, const short  &value);
	BOOL PutCollect(LPCTSTR strFieldName, const BYTE   &value);
	BOOL PutCollect(LPCTSTR strFieldName, const bool   &value);
	BOOL PutCollect(LPCTSTR strFieldName, const COleDateTime &value);
	BOOL PutCollect(LPCTSTR strFieldName, const COleCurrency &value);

	// ---------------------------------------------------------

	BOOL GetCollect(long index, CString &value);
	BOOL GetCollect(long index, double  &value);
	BOOL GetCollect(long index, float   &value);
	BOOL GetCollect(long index, long    &value);
	BOOL GetCollect(long index, DWORD   &value);
	BOOL GetCollect(long index, int     &value);
	BOOL GetCollect(long index, short   &value);
	BOOL GetCollect(long index, BYTE    &value);
	BOOL GetCollect(long index, bool   &value);
	BOOL GetCollect(long index, COleDateTime &value);
	BOOL GetCollect(long index, COleCurrency &value);

	BOOL GetCollect(LPCTSTR strFieldName, CString &strValue);
	BOOL GetCollect(LPCTSTR strFieldName, double &value);
	BOOL GetCollect(LPCTSTR strFieldName, float  &value);
	BOOL GetCollect(LPCTSTR strFieldName, long   &value);
	BOOL GetCollect(LPCTSTR strFieldName, DWORD  &value);
	BOOL GetCollect(LPCTSTR strFieldName, int    &value);
	BOOL GetCollect(LPCTSTR strFieldName, short  &value);
	BOOL GetCollect(LPCTSTR strFieldName, BYTE   &value);
	BOOL GetCollect(LPCTSTR strFieldName, bool   &value);
	BOOL GetCollect(LPCTSTR strFieldName, COleDateTime &value);
	BOOL GetCollect(LPCTSTR strFieldName, COleCurrency &value);

};

/*########################################################################
------------------------------------------------
CAdoParameter class
------------------------------------------------
########################################################################*/
class CAdoParameter
{
	// 构建/折构 -------------------------------------------
public:
	CAdoParameter();
	CAdoParameter(ADODB::DataTypeEnum DataType, long lSize, ADODB::ParameterDirectionEnum Direction, CString strName);
	virtual ~CAdoParameter();
	// 属性 ------------------------------------------------
public:
	_ParameterPtr GetParameter() {return m_pParameter;}

	// 参数精度 ---------------------------
	BOOL SetPrecision(TCHAR nPrecision);

	// 参数小数位数 -----------------------
	BOOL SetNumericScale(int nScale);

	// 参数类型 ---------------------------
	ADODB::ParameterDirectionEnum GetDirection();
	BOOL SetDirection(ADODB::ParameterDirectionEnum Direction);

	// 参数名称 ---------------------------
	CString GetName();
	BOOL SetName(CString strName);

	// 参数长度 ---------------------------
	int GetSize();
	BOOL SetSize(int size);

	// 参数据类型 -------------------------
	ADODB::DataTypeEnum GetType();
	BOOL SetType(ADODB::DataTypeEnum DataType);

	// 方法 ------------------------------------------------
public:	
	BOOL GetValue(COleDateTime &value);
	BOOL GetValue(CString &value);
	BOOL GetValue(double &value);
	BOOL GetValue(long &value);
	BOOL GetValue(int &value);
	BOOL GetValue(short &value);
	BOOL GetValue(BYTE &value);
	BOOL GetValue(bool &value);

	BOOL SetValue(const float &value);
	BOOL SetValue(const short &value);
	BOOL SetValue(const BYTE &value);
	BOOL SetValue(const COleDateTime &value);
	BOOL SetValue(const CString &value);
	BOOL SetValue(const double &value);
	BOOL SetValue(const long &value);
	BOOL SetValue(const int &value);
	BOOL SetValue(const bool &value);
	BOOL SetValue(const _variant_t &value);

	//其他方法 ------------------------------
public:
	_ParameterPtr& operator =(_ParameterPtr& pParameter);

	// 数据 ------------------------------------------------
protected:
	_ParameterPtr m_pParameter;
	CString m_strName;
	ADODB::DataTypeEnum m_nType;
};


/*########################################################################
------------------------------------------------
CAdoCommand class
------------------------------------------------
########################################################################*/

class CAdoCommand
{
	// 构建/折构 -------------------------------------------
public:
	CAdoCommand();
	CAdoCommand(CAdoConnection* pAdoConnection, CString strCommandText = _T(""), CommandTypeEnum CommandType = adCmdStoredProc);
	virtual ~CAdoCommand();

	// 属性 ------------------------------------------------
public:
	_variant_t GetValue(LPCTSTR lpstrName);
	_variant_t GetValue(long index);

	_ParameterPtr GetParameter(long index);
	_ParameterPtr GetParamter(LPCTSTR lpstrName);

	BOOL Append(_ParameterPtr param);

	ParametersPtr GetParameters();

	BOOL SetCommandTimeOut(long lTime);

	long GetState();

	BOOL SetCommandType(CommandTypeEnum CommandType);
	BOOL SetCommandText(LPCTSTR lpstrCommand);

	CAdoParameter operator [](int index);
	CAdoParameter operator [](LPCTSTR lpszParamName);

	// 实现 ------------------------------------------------
public:
	_ParameterPtr CreateParameter(LPCTSTR lpstrName, ADODB::DataTypeEnum Type, 
		ADODB::ParameterDirectionEnum Direction, 
		long Size, _variant_t Value);
	_RecordsetPtr CAdoCommand::Execute(long Options = adCmdStoredProc);
	BOOL Cancel();

	// 其他方法 --------------------------------------------
public:
	_CommandPtr& GetCommand();
	BOOL SetConnection(CAdoConnection *pConnect);

	// 数据 ------------------------------------------------
protected:
	void Release();
	_CommandPtr		m_pCommand;
	CString			m_strSQL;	//
};

#endif

