#include "StdAfx.h"
#include "suUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CsuUtil::CsuUtil(void)
{
}

CsuUtil::~CsuUtil(void)
{
}

void CsuUtil::to64frombits(unsigned char *out, const unsigned char *in, int inlen)
{
	const char base64digits[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	for (; inlen >= 3; inlen -= 3)
	{
		*out++ = base64digits[in[0] >> 2];
		*out++ = base64digits[((in[0] << 4) & 0x30) | (in[1] >> 4)];
		*out++ = base64digits[((in[1] << 2) & 0x3c) | (in[2] >> 6)];
		*out++ = base64digits[in[2] & 0x3f];
		in += 3;
	}
	if (inlen > 0)
	{
		unsigned char fragment;

		*out++ = base64digits[in[0] >> 2];
		fragment = (in[0] << 4) & 0x30;
		if (inlen > 1)
			fragment |= in[1] >> 4;
		*out++ = base64digits[fragment];
		*out++ = (inlen < 2) ? '=' : base64digits[(in[1] << 2) & 0x3c];
		*out++ = '=';
	}
	*out = '\0';
}

void CsuUtil::paserAdopack(char* data, PACK_ADO& packado)
{
	char* pvist = data;
	memcpy(&packado.adotype, pvist, sizeof(packado.adotype));
	pvist += sizeof(packado.adotype);

	memcpy(&packado.result, pvist, sizeof(packado.result));
	pvist += sizeof(packado.result);

	memcpy(&packado.datalen, pvist, sizeof(packado.datalen)); //设定数据包长度
	pvist += sizeof(packado.datalen);
	packado.data = pvist;
}

//序列化adopack
int CsuUtil::serialAdopack(char** data,PACK_ADO& packado)
{
	int ilen = 0;
	ilen = packado.datalen + sizeof(PACK_ADO) - sizeof(char*) + 1;
	*data = new char[ilen];
	memset(*data,0,ilen);

	char* pvist = *data;
	memcpy(pvist, &packado.adotype, sizeof(packado.adotype));
	pvist += sizeof(packado.adotype);

	memcpy(pvist, &packado.result, sizeof(packado.result));
	pvist += sizeof(packado.result);

	memcpy(pvist, &packado.datalen, sizeof(packado.datalen));
	pvist += sizeof(packado.datalen);

	if(packado.datalen > 0)
		memcpy(pvist, packado.data, packado.datalen);

	return ilen;
}

//helpers
CString CsuUtil::GetLastErrorMessageString(DWORD dwLastError )
{
	LPVOID lpMsgBuf = NULL;
	FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
		dwLastError, 
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf, 0, NULL );

	CString str = (LPCTSTR)lpMsgBuf;

	if( lpMsgBuf != NULL )
		LocalFree(lpMsgBuf);

	return str;
}

CString CsuUtil::getAppDir()
{
	TCHAR szAppDir[MAX_PATH];
	GetModuleFileName(NULL,szAppDir,MAX_PATH);	//获取当前目录
	TCHAR *p = _tcsrchr(szAppDir, '\\');
	if (p)
		*(p + 1) = '\0';
	
	CString path(szAppDir);
	return path;
}