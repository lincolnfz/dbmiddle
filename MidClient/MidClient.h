#ifndef _MIDCLIENT_H
#define _MIDCLIENT_H

#ifdef DLLEXPORT
#define _LIBAPI extern "C" _declspec(dllexport)
#else
#define _LIBAPI extern "C" _declspec(dllimport)
#endif

_LIBAPI void __stdcall setProxy(TCHAR* proxyip, int port,int proxyType);
_LIBAPI void __stdcall setServer(TCHAR* srvip,int port);
_LIBAPI void __stdcall executeSql(TCHAR* sql , void (*callback)(TCHAR**, long&, int&) , unsigned int Flag);

#endif