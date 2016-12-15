// ApiHook.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "ApiHook.h"
#include "mhook.h"


typedef HANDLE (WINAPI* pfn_CreateFileA)(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
typedef HANDLE (WINAPI* pfn_CreateFileW)(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);

typedef int (WINAPI* pfn_bind)(SOCKET s, const struct sockaddr FAR *addr, int namelen);
typedef int (WINAPI* pfn_connect)(SOCKET s, const struct sockaddr FAR *name, int namelen);
typedef int (WINAPI* pfn_recv)(SOCKET s, char* buf, int len, int flags);
typedef int (WINAPI* pfn_send)(SOCKET s, char* buf, int len, int flags);

typedef LSTATUS (WINAPI* pfn_RegOpenKeyExA)(HKEY hKey, LPCSTR lpSubKey, DWORD dwOptions, REGSAM samDesired, PHKEY phkResult);
typedef LSTATUS (WINAPI* pfn_RegOpenKeyExW)(HKEY hKey, LPCWSTR lpSubKey, DWORD dwOptions, REGSAM samDesired, PHKEY phkResult);
typedef LSTATUS (WINAPI* pfn_RegSetValueExA)(HKEY hKey, LPCSTR lpValueName, DWORD dwReserved, DWORD dwType, CONST BYTE* lpData, DWORD dwData);
typedef LSTATUS (WINAPI* pfn_RegSetValueExW)(HKEY hKey, LPCWSTR lpValueName, DWORD dwReserved, DWORD dwType, CONST BYTE* lpData, DWORD dwData);

//HMODULE hDll = LoadLibraryA("Advapi32.dll");
//pfnRegOpenKeyExA pFun = (pfnRegOpenKeyExA)GetProcAddress(hDll, "RegOpenKeyExA");

extern "C" APIHOOK_API void SetAPIHook( void* pTrueFunc, void* pFakeFunc )
{
	Mhook_SetHook((PVOID*)&pTrueFunc, (void*)pFakeFunc);
}

extern "C" APIHOOK_API void SetAPIUnHook( void* pTrueFunc)
{
	Mhook_Unhook((PVOID*)&pTrueFunc);
}


//LSTATUS WINAPI FakeRegOpenKeyExA(HKEY hKey, LPCSTR lpSubKey, DWORD dwOptions, REGSAM samDesired, PHKEY phkResult){	
//	//return pFun(hKey, lpSubKey, dwOptions, samDesired, phkResult);
//}

extern "C" APIHOOK_API void Test()
{
	//Mhook_SetHook((PVOID*)&pFun, FakeRegOpenKeyExA);
}

#define DefineValue(str) pfn_##str _true_##str;
#define GetProcValue(str, dll) _true_##str = (pfn_##str)GetProcAddress(dll, #str);

class CHookGroup{
public:
	CHookGroup();
	HMODULE hKernel32;
	HMODULE hAdvapi32;
	HMODULE hWs2_32;

	DefineValue(CreateFileA);
	DefineValue(CreateFileW);
	DefineValue(connect);
	DefineValue(recv);
	DefineValue(send);
	DefineValue(bind);
	DefineValue(RegOpenKeyExA);
	DefineValue(RegSetValueExA);
	DefineValue(RegOpenKeyExW);
	DefineValue(RegSetValueExW)	
};

CHookGroup::CHookGroup()
{
	hKernel32 = LoadLibraryA("kernel32.dll");
	hAdvapi32 = LoadLibraryA("Advapi32.dll");
	hWs2_32 = LoadLibraryA("ws2_32.dll");

	GetProcValue(CreateFileA, hKernel32);
	GetProcValue(CreateFileW, hKernel32);

	GetProcValue(bind, hWs2_32);
	GetProcValue(send, hWs2_32);
	GetProcValue(connect, hWs2_32);
	GetProcValue(recv, hWs2_32);

	GetProcValue(RegOpenKeyExA, hAdvapi32);
	GetProcValue(RegOpenKeyExW, hAdvapi32);
	GetProcValue(RegSetValueExA, hAdvapi32);
	GetProcValue(RegSetValueExW, hAdvapi32);
}

CHookGroup g_group;

HANDLE WINAPI Fake_CreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	g_group._true_CreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	return 0;
}

HANDLE WINAPI Fake_CreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	g_group._true_CreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	return 0;
}

extern "C" APIHOOK_API void AddHook( char* pFunctionName )
{
	if (strcmpi("CreateFileA", pFunctionName) == 0){
		Mhook_SetHook((PVOID*)&g_group._true_CreateFileA, Fake_CreateFileA);
	}else if (strcmpi("CreateFileW", pFunctionName) == 0){
		Mhook_SetHook((PVOID*)&g_group._true_CreateFileW, Fake_CreateFileW);
	}
}

extern "C" APIHOOK_API void RemoveHook( char* pFunctionName )
{

}
