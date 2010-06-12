// MidClient.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include <afxdllx.h>
#include "AdoClientManager.h"
#include "AdoClient.h"
#include "MidClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define  DLLEXPORT

static AFX_EXTENSION_MODULE MidClientDLL = { NULL, NULL };

CAdoClientManager adoman;
CAdoClient* padoclient;
TCHAR szCommand[8192];

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// ���ʹ�� lpReserved���뽫���Ƴ�
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("MidClient.DLL ���ڳ�ʼ����\n");

		::CoInitialize(NULL);
		padoclient = new CAdoClient();
		padoclient->setAdoClientManager(&adoman);
		
		// ��չ DLL һ���Գ�ʼ��
		if (!AfxInitExtensionModule(MidClientDLL, hInstance))
			return 0;

		// ���� DLL ���뵽��Դ����
		// ע��: �������չ DLL ��
		//  MFC ���� DLL(�� ActiveX �ؼ�)��ʽ���ӵ�
		//  �������� MFC Ӧ�ó������ӵ�������Ҫ
		//  �����д� DllMain ���Ƴ������������һ��
		//  �Ӵ���չ DLL �����ĵ����ĺ����С�ʹ�ô���չ DLL ��
		//  ���� DLL Ȼ��Ӧ��ʽ
		//  ���øú����Գ�ʼ������չ DLL������
		//  CDynLinkLibrary ���󲻻ḽ�ӵ�
		//  ���� DLL ����Դ���������������ص�
		//  ���⡣

		new CDynLinkLibrary(MidClientDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("MidClient.DLL ������ֹ��\n");
		//delete padoclient;
		::CoUninitialize();

		// �ڵ�����������֮ǰ��ֹ�ÿ�
		AfxTermExtensionModule(MidClientDLL);
	}
	return 1;   // ȷ��
}

void __stdcall setProxy(TCHAR* proxyip, int port,int proxyType)
{
	adoman.SetProxy(proxyip, port, (PROXYTYPE)proxyType);
}

void __stdcall setServer(TCHAR* srvip,int port)
{
	adoman.SetDBProvider(srvip,port);
}

void __stdcall executeSql( TCHAR* sql, FNBufferCallback callback  /*void (*callback)(TCHAR**, long&, int&)*/ )
{
	padoclient->setBufferCallback(callback);
	_tcscpy(szCommand,sql);
	padoclient->execute(szCommand);
}
