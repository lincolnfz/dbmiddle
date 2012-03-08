#include "StdAfx.h"
#include "SrvUDPManager.h"
#include "SrvUDPPeer.h"


CSrvUDPManager::CSrvUDPManager(void)
{
}


CSrvUDPManager::~CSrvUDPManager(void)
{
}

CUDTUnit* CSrvUDPManager::GetNewUDTUnit()
{
	return new CSrvUDPPeer();
}
