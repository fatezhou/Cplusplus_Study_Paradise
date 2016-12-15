#ifndef _TOOLS_H_
#define _TOOLS_H_

#include "stdafx.h"



static bool IsXP()
{
	OSVERSIONINFO   osver;    
	osver.dwOSVersionInfoSize   =   sizeof(OSVERSIONINFO);    
	GetVersionEx(&osver);    
	if(osver.dwPlatformId == 2) 
	{
		if(osver.dwMajorVersion < 6) 
		{ 
			return true; 
		} 
	}
	return false;
}

static bool IsPortAvail( int nPort )
{
	int nOk = 0;

	SOCKET s_localhost = socket(AF_INET, SOCK_STREAM, 0);
	SOCKET s_any = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addrLocal, addrAny;
	addrLocal.sin_addr.S_un.S_addr =  htonl((u_long)0x7f000001);
	addrLocal.sin_family = AF_INET;   
	addrLocal.sin_port = htons(nPort);

	addrAny.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrAny.sin_family = AF_INET;   
	addrAny.sin_port = htons(nPort);

	if (bind(s_any, (SOCKADDR*)&addrAny, sizeof(SOCKADDR)) == 0)
	{
		nOk++;

		if (IsXP())
		{
			nOk++;
		}
		else
		{
			if (bind(s_localhost, (SOCKADDR*)&addrLocal, sizeof(SOCKADDR)) == 0)
			{
				nOk++;
			}	
		}	
	}
	BOOL bReuseaddr = TRUE;
	setsockopt (s_localhost, SOL_SOCKET , SO_REUSEADDR, (const char*)&bReuseaddr, sizeof(BOOL));
	setsockopt (s_any, SOL_SOCKET , SO_REUSEADDR, (const char*)&bReuseaddr, sizeof(BOOL));
	closesocket(s_any);
	closesocket(s_localhost);
	if (nOk == 2)
	{
		return true;
	}
	return false;
}

static int GenerateAvailPort(int nDefPort)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);

	int nPort = nDefPort;
	while(!IsPortAvail(nPort))
	{
		nPort++;
	}

	return nPort;
}




















#endif