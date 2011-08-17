#pragma once
#include "../udt/udt.h"
#include <set>
#include <map>

class CUDTUnit;

class CUDTExpand
{
	typedef std::map< UDTSOCKET , CUDTUnit* > REMOTE_CLIENT_MAP;
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

	virtual int procRecvData( char* pData , unsigned int ulen );


protected:

	UDTSOCKET m_UDTSock_srv;
};

