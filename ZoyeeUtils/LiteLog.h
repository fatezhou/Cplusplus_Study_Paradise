#ifndef __LITE_LOG__
#define __LITE_LOG__

#include <Windows.h>
#include <string>
#include <time.h>
#include <stdarg.h>

#define LOG_DEBUG 0
#define LOG_INFO 1
#define LOG_ERROR 2
#define LOG_NONE 3

namespace ZoyeeUtils{	
	static char* ARRAY_TYPE[] = {
		" INFO", "DEBUG", "ERROR"
	};
	class CLiteLog{
	public:
		static CLiteLog* GetLog();
		void Config(const char* pConfigFile);
		void SetLogLevel(int nLogLevel);		

		void Log(int nLogLevel, char* pModule, char* pCppFile, int nLine, char* pFmt, ...);		
		void Debug(char* pModule, char* pCppFile, int nLine, char* pFmt, ...);
		void Info(char* pModule, char* pCppFile, int nLine, char* pFmt, ...);
		void Error(char* pModule, char* pCppFile, int nLine, char* pFmt, ...);
		void VLog(int nLogLevel, char* pModule, char* pCppFile, int nLine, char* pFmt, va_list& ap);
	private:
		CLiteLog();
		~CLiteLog();	

		CRITICAL_SECTION cs;
		SYSTEMTIME sysTime;

		inline void Lock(){
			::EnterCriticalSection(&cs);
		};

		inline void UnLock(){
			::LeaveCriticalSection(&cs);
		};	

		inline std::string GetTime(){
			static char szBuff[128];
			GetLocalTime(&sysTime);
			sprintf(szBuff, "%04d-%02d-%02d %02d:%02d:%02d.%04d", sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);
			return szBuff;
		}
		inline char* GetType(int nType){
			if (nType > LOG_NONE){
				return "";
			}
			return ARRAY_TYPE[nType];
		}
		int m_nLogLevel;

		FILE* pLogFile;
	};
}
#define LLOG(nLogLv, pFmt, ...) ZoyeeUtils::CLiteLog::GetLog()->Log(nLogLv, __FUNCTION__, (__FILE__), __LINE__, pFmt, __VA_ARGS__);
#define LINFO(pFmt, ...) ZoyeeUtils::CLiteLog::GetLog()->Info(__FUNCTION__, (__FILE__), __LINE__, pFmt, __VA_ARGS__);
#define LDEBUG(pFmt, ...) ZoyeeUtils::CLiteLog::GetLog()->Debug(__FUNCTION__, (__FILE__), __LINE__, pFmt, __VA_ARGS__);
#define LERROR(pFmt, ...) ZoyeeUtils::CLiteLog::GetLog()->Error(__FUNCTION__, (__FILE__), __LINE__, pFmt, __VA_ARGS__);
#endif
