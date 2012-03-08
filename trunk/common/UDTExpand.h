#pragma once
#include "../udt/udt.h"
#include "../common/TaskPool.h"
#include "../common/Mutex.h"
#include "../common/CommonHead.h"
#include <set>
#include <map>

class CUDTUnit;

class CUDTExpand
{
	typedef std::map< UDTSOCKET , CUDTUnit* > REMOTE_CLIENT_MAP;

	typedef struct _PEERPARM
	{
		UDTSOCKET sock;
		char* buf;
		int buf_len;
	}PEERPARM;

	friend unsigned long __stdcall procDataThread( /*in*/void* lpParam , /*out*/void* funContext );
	friend unsigned int __stdcall ListenUDTData(LPVOID lpParameter);

private:
	REMOTE_CLIENT_MAP m_remote_peers;

public:
	CUDTExpand(void);
	~CUDTExpand(void);

	/************************************************************************/
	/* 创建udt服务                                                                     */
	/************************************************************************/
	void UDT_srv();

	void UDT_clean();

	void AddNewContent( UDTSOCKET& remoteSock , sockaddr_storage& remoteAddr );

	void udpHole( const char*& remoteIP , const int& port );

	void RemoveContent( int& eid , UDTSOCKET& remoteSock );

	/************************************************************************/
	/* 可重定义CUDTUnit的类                                                  */
	/************************************************************************/
	virtual CUDTUnit* GetNewUDTUnit();
	

protected:

	//提交到池程池处理
	int SubmitTask( const UDTSOCKET& sock , const char*& buf , const int& len );

	//
	int procRecvData( const UDTSOCKET& sock , const char*& buf , const int& len );

	int bindUDPSock( const int& udpsock , const sockaddr_in& addr );


protected:
	Mutex m_mutex;
	UDTSOCKET m_UDTSock_srv;

	CTaskPool m_TaskPool;
};

