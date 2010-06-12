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
	//ִ��sql���
	void execute(LPCTSTR sql , unsigned int Flag = 0 );
	
	//�ύ���ݼ�
	void update( unsigned int Flag = 0);

	inline DWORD getID(){return m_id;}

	//�����յ����ݵ��߳�
	static DWORD CALLBACK ThreadRecvProc(LPVOID lpParameter);

	//�������ݼ����ؽ���Ļص�
	void setRecvCallback(FNSqlResultCallback handleRecv);
	
	//����sql����Ƿ�ִ�гɹ��Ļص�
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
