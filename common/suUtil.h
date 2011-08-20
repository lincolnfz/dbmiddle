#pragma once

#include "CommonHead.h"
class CsuUtil
{
public:
	CsuUtil(void);
	~CsuUtil(void);

	static void to64frombits(unsigned char *out, const unsigned char *in, int inlen);
	static char* paserAdopack(char* , unsigned long ,  PACK_ADO& );
	static int serialAdopack(char** ,PACK_ADO& );
	static CString CsuUtil::GetLastErrorMessageString(DWORD dwLastError );
	static CString getAppDir();
	static int getSockType( SOCKET s);
};
