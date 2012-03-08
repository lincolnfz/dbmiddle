#pragma once
#include "../common/udtunit.h"
#include "../common/CommonHead.h"
#include <map>
class CSrvUDPPeer :
	public CUDTUnit
{

public:
	CSrvUDPPeer(void);
	~CSrvUDPPeer(void);

	virtual PROCITEM* SubmitPack( char* data , long len );
};

