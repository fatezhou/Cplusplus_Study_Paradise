// CrashDebuger.cpp : 定义 DLL 应用程序的导出函数。
//
#include <Windows.h>
#include "CrashDebuger.h"
extern LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException);  
extern void SetCallbackUserDataFunction(pCallbackUserData p);
using namespace ZoyeeUtils;
void InitDebuger(pCallbackUserData pFunction)
{
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);
	SetCallbackUserDataFunction(pFunction);
}
