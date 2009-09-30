#ifndef _MIDCLIENT_H
#define _MIDCLIENT_H

#ifdef DLLEXPORT
#define _LIBAPI extern "C" _declspec(dllexport)
#else
#define _LIBAPI extern "C" _declspec(dllimport)
#endif

_LIBAPI void __stdcall setProxy(char* proxyip, int port,int proxyType);
_LIBAPI void __stdcall setServer(char* srvip,int port);
_LIBAPI void __stdcall executeSql(char* sql,void (*callback)(char**, long&, int&));

#endif