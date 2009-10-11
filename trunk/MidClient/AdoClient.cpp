#include "StdAfx.h"
#include ".\adoclient.h"
#include "Util.h"
//#include "jsonlib/value.h"
//#include "jsonlib/writer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CAdoClient::CAdoClient():CAdoRecordSet()
{
	m_id = reinterpret_cast<DWORD>(this);
	m_handleRecv = NULL;
	m_bDataSet = FALSE;
	m_handleBuf = NULL;
}

CAdoClient::CAdoClient(CAdoClientManager* pAdoManager):CAdoRecordSet()
{
	m_id = reinterpret_cast<DWORD>(this);
	m_pAdoClientManager = pAdoManager;
	m_handleRecv = NULL;
	m_bDataSet = FALSE;
	m_handleBuf = NULL;
}

CAdoClient::~CAdoClient(void)
{
}

void CAdoClient::setAdoClientManager(CAdoClientManager* pAdoManager)
{
	m_pAdoClientManager = pAdoManager;
}

void CAdoClient::execute(LPCTSTR sql)
{
	m_bDataSet = FALSE;
	PACK_ADO packado;
	packado.adotype = PACK_ADO.ADO_EXECUTE;
	packado.result = REQUEST_ASK;
	packado.datalen = strlen(sql);
	packado.data = const_cast<char*>(sql);
	m_pAdoClientManager->sendAdoData(this,packado);
}

void CAdoClient::update()
{
	PACK_ADO packado;
	LONG lsize;
	::CoInitialize(NULL);
	if(SaveToBuffer(packado.data,lsize))
	{
		packado.datalen = lsize;
		packado.adotype = PACK_ADO.ADO_UPDATE;
		packado.result = REQUEST_ASK;
		m_pAdoClientManager->sendAdoData(this,packado);
		delete[] packado.data;
	}
	::CoUninitialize();
}

DWORD CAdoClient::ThreadRecvProc(LPVOID lpParameter)
{
	RECVADOBUF* pRecvAdo = (RECVADOBUF*)lpParameter;
	CAdoClient* pAdoClient = pRecvAdo->pAdoClient;
	PACK_ADO packado;
	CUtil::paserAdopack(pRecvAdo->pbuf,packado);
	
	if(packado.adotype == PACK_ADO.ADO_EXECUTE && packado.result == RESULT_SUCESS)
	{
		::CoInitialize(NULL);
		pAdoClient->m_bDataSet = pAdoClient->LoadBuffer(packado.data, packado.datalen);
		::CoUninitialize();
	}

	//执行内存序列回调
	if(pAdoClient->m_handleBuf)
	{
		(*(pAdoClient->m_handleBuf))(&(packado.data), packado.datalen, (int&)packado.result);
	}

	//释放provider分配的空间
	pAdoClient->m_pAdoClientManager->releaseBuf(pRecvAdo->pbuf);
	delete pRecvAdo;

	//执行回调
	if(pAdoClient->m_handleRecv)
	{
		(*(pAdoClient->m_handleRecv))(pAdoClient,packado.result);
	}

	//pAdoClient->serialJson();

	
	return 0;
}

void CAdoClient::setRecvCallback(FNSqlResultCallback handleRecv)
{
	m_handleRecv = handleRecv;
}

void CAdoClient::setBufferCallback(FNBufferCallback handleBuf)
{
	m_handleBuf = handleBuf;
}

/*std::string CAdoClient::serialJson()
{
	Json::Value root;
	Json::StyledWriter writer;

	int i = 0,j = 0;
	DWORD nCount = GetRecordCount();
	long nField = GetFieldsCount();
	DataTypeEnum fieldtype;
	for(i = 0; i < nCount; i++)
	{
		for (j = 0; j < nField; j++)
		{
			fieldtype = GetFieldType(j);
			switch (fieldtype)
			{
			case adTinyInt:
			case adInteger:
			case adBigInt:
			case adUnsignedTinyInt:
			case adUnsignedSmallInt:
			case adUnsignedInt:
			case adUnsignedBigInt:
			case adSingle:
			case adCurrency:
			case adDecimal:
			case adNumeric:
				{
					root[GetFieldName(j).GetBuffer()] = GetIntFieldValue(j);					
				}
				break;

			case adDouble:
				{
					root[GetFieldName(j).GetBuffer()] = GetDoubleFieldValue(j);
				}
				break;

			case adBoolean:
				{
					root[GetFieldName(j).GetBuffer()] = GetBoolFieldValue(j);
				}
				break;

			case adDate:
			case adDBDate:
			case adDBTime:
			case adDBTimeStamp:
				{
					//root[GetFieldName(j).GetBuffer()] = GetDateTimeFieldValue(j);
				}
				break;

			case adChar:
				{
					root[GetFieldName(j).GetBuffer()] = GetStringFieldValue(j).GetBuffer();
				}
				break;

			default:
				break;
			}
			//fech next field
		}	
		//fech next record
	}

	std::string output = writer.write(root);

	return output;
}*/

void CAdoClient::handleRecv(EPACKATTRIB& packattrib)
{

}