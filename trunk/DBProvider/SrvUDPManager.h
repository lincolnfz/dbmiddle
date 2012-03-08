#pragma once
#include "../common/udtexpand.h"
class CSrvUDPManager :
	public CUDTExpand
{
public:
	CSrvUDPManager(void);
	~CSrvUDPManager(void);

	virtual CUDTUnit* GetNewUDTUnit();
};

