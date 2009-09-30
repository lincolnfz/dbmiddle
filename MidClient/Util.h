#pragma once

#include "CommonHead.h"
class CUtil
{
public:
	CUtil(void);
	~CUtil(void);

	static void to64frombits(unsigned char *out, const unsigned char *in, int inlen);
	static void paserAdopack(char* data, PACK_ADO& packado);
	static int serialAdopack(char** data,PACK_ADO& packado);
	static CString CUtil::GetLastErrorMessageString(DWORD dwLastError );
	static CString getAppDir();
};
