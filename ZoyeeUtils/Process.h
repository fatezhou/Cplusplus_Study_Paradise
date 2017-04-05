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
		//�⺯��Ϊͬ������, ���ǳ�ʱ��Ŀ¼���̹ر�, ���᷵��

		static int GetProcessId(char* pProcessName);
		static char* GetProcessName(int nPid);
		static int CloseProcessById(int nPid);
		static int CloseProcessByProcessName(char* pProcessName, bool bAllSomeName = true);
	private:
		char* m_pProcessPath;
		char* m_pCmd;
		void* m_pPi;
	};
}

#endif