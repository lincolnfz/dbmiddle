#pragma once

#include "CommonHead.h"
class CsuUtil
{
public:
	CsuUtil(void);
	~CsuUtil(void);

	static void to64frombits(unsigned char *out, const unsigned char *in, int inlen);
	static void paserAdopack(char* data, PACK_ADO& packado);
	static int serialAdopack(char** data,PACK_ADO& packado);
	static CString CsuUtil::GetLastErrorMessageString(DWORD dwLastError );
	static CString getAppDir();
};
