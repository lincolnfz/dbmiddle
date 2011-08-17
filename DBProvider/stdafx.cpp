// stdafx.cpp : 只包括标准包含文件的源文件
// DBProvider.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"


// TODO: 在 STDAFX.H 中
// 引用任何所需的附加头文件，而不是在此文件中引用

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