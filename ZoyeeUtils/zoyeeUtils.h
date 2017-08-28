#ifndef __ZOYEE_UTILS_H__
#define __ZOYEE_UTILS_H__

#include "TextCode.h"
#include "stdStringEx.h"
#include "Process.h"
#include "WMI.h"
#include "ProcessSnapshot.h"
#include "SingleProcess.hpp"
#include "ThreadEx.h"
#include "FileManange.h"
#include "SocketFactory.h"
#include "RegEdit.h"
#include "Regex.h"
#include "FileMemory.h"
#include "IPCInterface.h"
#include "SystemInfo.h"
#include "LiteLog.h"
#include "Encrypt.h"
#include "ILock.h"

typedef void(*p_inner_log_callback)(char* pModule, int nLogLevel, int nLine, char* pDescribe);
p_inner_log_callback pInnerCallback = nullptr;
void SetInnerLogCallback(p_inner_log_callback pCallback){
	pInnerCallback = pCallback;
};

void defaultInnerLog(char* pModule, int nLogLevel, int nLine, char* pDescribe){
	printf("[%s][%d][%d]:%s\n", pModule, nLogLevel, nLine, pDescribe);
}
p_inner_log_callback GetInnerLogCallback(){
	if (pInnerCallback){
		return pInnerCallback;
	}
	else{
		return defaultInnerLog;
	}
};

#endif