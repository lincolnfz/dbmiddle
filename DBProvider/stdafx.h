// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <afxwin.h>         // MFC ���ĺͱ�׼���
//#include <afxext.h>         // MFC ��չ
//#include <afxdisp.h>        // MFC �Զ�����
//
//#include <Windows.h>
#include <io.h>


// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include "xmlParser.h"

#include "../third_party/tcmalloc.h"

#ifdef HAS_TCMALLOC
void* operator new ( unsigned int size );

void* operator new[] ( unsigned int size );

void operator delete( void* p );

void operator delete[]( void *p );
#endif