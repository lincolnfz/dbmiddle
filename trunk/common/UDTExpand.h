#pragma once
#include "../udt/udt.h"
#include <set>

class CUDTExpand
{
private:


public:
	CUDTExpand(void);
	~CUDTExpand(void);

	/************************************************************************/
	/* ����udt����                                                                     */
	/************************************************************************/
	void UDT_srv();

	void UDT_clean();

	virtual int procRecvData( char* pData , unsigned int ulen );

protected:

	UDTSOCKET m_UDTSock_srv;
};

