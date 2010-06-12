#pragma once

#include <process.h>
#include "../common/CommonHead.h"
#include "../common/Ado.h"
#include "../common/Mutex.h"
#include "AdoClientManager.h"

class CAdoClient;

typedef void (*FNSqlResultCallback)(CAdoClient*, EPACKATTRIB&);
typedef void ( _stdcall *FNBufferCallback)(char**, unsigned long&, int&);


class CAdoClient : public CAdoRecordSet 
{
public:
	CAdoClient(CAdoClientManager* pAdoManager);
	CAdoClient();
	~CAdoClient(void);
	//执行sql语句
	void execute(LPCTSTR sql , unsigned int Flag = 0 );
	
	//提交数据集
	void update( unsigned int Flag = 0);

	inline DWORD getID(){return m_id;}

	//处理收的数据的线程
	static DWORD CALLBACK ThreadRecvProc(LPVOID lpParameter);

	//设置数据集返回结果的回调
	void setRecvCallback(FNSqlResultCallback handleRecv);
	
	//设置sql语句是否执行成功的回调
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
