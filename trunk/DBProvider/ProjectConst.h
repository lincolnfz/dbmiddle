#pragma once
#include "AdoProvider.h"

class CProjectConst
{
public:
	CProjectConst(void);
	~CProjectConst(void);

	static CAdoProvider* getAdoProvider(){ return CProjectConst::m_pAdoProvider; }
	static void setAdoProvider( CAdoProvider *p ){ CProjectConst::m_pAdoProvider = p; }

private:
	static CAdoProvider* m_pAdoProvider;
};

