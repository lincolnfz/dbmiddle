// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// DBProvider.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"


// TODO: �� STDAFX.H ��
// �����κ�����ĸ���ͷ�ļ����������ڴ��ļ�������

#ifdef HAS_TCMALLOC

#ifdef _DEBUG
#pragma comment(lib, "../third_lib/libtcmalloc_minimal-debug.lib")
#else
#pragma comment(lib, "../third_lib/libtcmalloc_minimal.lib") 
#endif

void* operator new ( unsigned int size )
{
	return tc_malloc(size);
}

void* operator new[] ( unsigned int size )
{
	return tc_malloc( size );
}

void operator delete( void* p )
{
	tc_free(p);
}

void operator delete[]( void *p )
{
	tc_free(p);
}

#endif