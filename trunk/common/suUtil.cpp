#include "StdAfx.h"
#include "suUtil.h"
#include "../zlib/zlib.h"

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

char* CsuUtil::paserAdopack(char* pComprData, unsigned long ulong , PACK_ADO& packado)
{
	unsigned long uncomprlen = 0;
	memcpy( &uncomprlen , pComprData , sizeof(unsigned long) );
	char *data = new char[uncomprlen + 100];
	memset( data , 0 , uncomprlen + 100);
	char *pComprStart = pComprData + sizeof(unsigned long);
	uncompress( (byte*)data , &uncomprlen , (byte*)pComprStart , ulong - sizeof(unsigned long) );

	char* pvist = data;
	memcpy(&packado.adotype, pvist, sizeof(packado.adotype));
	pvist += sizeof(packado.adotype);

	memcpy(&packado.result, pvist, sizeof(packado.result));
	pvist += sizeof(packado.result);

	memcpy(&packado.datalen, pvist, sizeof(packado.datalen)); //设定数据包长度
	pvist += sizeof(packado.datalen);
	packado.data = pvist;

	return data;
}

//序列化adopack
int CsuUtil::serialAdopack(char** pComprData , PACK_ADO& packado)
{
	int ilen = 0;
	ilen = packado.datalen + sizeof(PACK_ADO) - sizeof(char*) + 1;
	char *pdata = new char[ilen + 1];
	memset(pdata,0,ilen+1);

	char* pvist = pdata;
	memcpy(pvist, &packado.adotype, sizeof(packado.adotype));
	pvist += sizeof(packado.adotype);

	memcpy(pvist, &packado.result, sizeof(packado.result));
	pvist += sizeof(packado.result);

	memcpy(pvist, &packado.datalen, sizeof(packado.datalen));
	pvist += sizeof(packado.datalen);

	if(packado.datalen > 0)
		memcpy(pvist, packado.data, packado.datalen);

	//压缩数据
	unsigned long complen = sizeof(unsigned long) + ( unsigned long )ilen;
	*pComprData = new char[ complen + 100 ];
	memset(*pComprData , 0 , complen + 100);
	char* pStartCompr = *pComprData;
	pStartCompr += sizeof(unsigned long);
	compress( (byte*)pStartCompr , &complen, (const Bytef*)pdata, ilen); //压缩数据
	delete []pdata;
	memcpy( *pComprData , &ilen , sizeof(unsigned long) ); //填充原始数据大小
	ilen = sizeof(unsigned long) + complen; //计算压缩后的数据大小

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