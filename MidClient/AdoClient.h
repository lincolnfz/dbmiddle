#pragma once

#include <process.h>
#include "CommonHead.h"
#include "Ado.h"
#include "Mutex.h"
#include "AdoClientManager.h"

class CAdoClient;

typedef void (*FNSqlResultCallback)(CAdoClient*, EPACKATTRIB&);
typedef void (*FNBufferCallback)(char**, long&, int&);


class CAdoClient : public CAdoRecordSet 
{
public:
	CAdoClient(CAdoClientManager* pAdoManager);
	CAdoClient();
	~CAdoClient(void);
	//执行sql语句
	void execute(LPCTSTR sql);
	
	//提交数据集
	void update();

	inline DWORD getID(){return m_id;}

	//处理收的数据的线程
	static DWORD CALLBACK ThreadRecvProc(LPVOID lpParameter);

	void setRecvCallback(FNSqlResultCallback handleRecv);
	void setBufferCallback(FNBufferCallback handleBuf);
	void setAdoClientManager(CAdoClientManager* pAdoManager);
	inline BOOL DataSetAvailability(){return m_bDataSet;}
	//std::string serialJson();
	
	virtual void handleRecv(EPACKATTRIB& packattrib);



private:
	CAdoClientManager* m_pAdoClientManager;
	DWORD m_id;
	FNSqlResultCallback m_handleRecv;
	FNBufferCallback m_handleBuf;
	BOOL m_bDataSet;
};
