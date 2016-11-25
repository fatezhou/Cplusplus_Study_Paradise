#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <string>

namespace ZoyeeUtils{	
	typedef void(*pEndCallback)(int nPid, int nExitCode, std::string& strOutput);
	class CProcess{
	public:
		CProcess(char* pProcessPath);
		CProcess(char* pProcessPath, char* pFmt, ...);

		~CProcess();

		int Start(bool bShowWindow = false, bool bWaitForClose = true, int nTimeOutMillisecond = -1);// -1 means INFINITE
		void* GetProcessHandle();
		int GetProcessProcessId();

		int StartEx(pEndCallback pCallback, std::string& strOutput, void* hInteraptEvent = nullptr, int nTimeOutMillisecond = -1);// -1 means INFINITE
		//这函数为同步函数, 若非超时或目录进程关闭, 不会返回
		
	private:
		char* m_pProcessPath;
		char* m_pCmd;
		void* m_pPi;
	};
}

#endif