#include "StdAfx.h"
#include "SrvUDPPeer.h"
#include "ProjectConst.h"
#include "../common/suUtil.h"

CSrvUDPPeer::CSrvUDPPeer(void)
{
}


CSrvUDPPeer::~CSrvUDPPeer(void)
{
}

PROCITEM* CSrvUDPPeer::SubmitPack( char* data , long len )
{
	PROCITEM* p = CUDTUnit::SubmitPack( data , len );
	CProjectConst::getAdoProvider()->NotifyRequest( p );

	return 0;
}
