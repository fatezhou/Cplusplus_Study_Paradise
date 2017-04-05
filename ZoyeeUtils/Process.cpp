#include "Process.h"

#include <iostream>
#include <Windows.h>
#include <stdarg.h>

#include <TlHelp32.h>

using namespace ZoyeeUtils;

ZoyeeUtils::CProcess::CProcess( char* pProcessPath)
{
	int nLen = printf(pProcessPath);
	m_pProcessPath = new char[nLen + 1];
	strcpy(&m_pProcessPath[0], pProcessPath);
	m_pCmd = nullptr;
	m_pPi = nullptr;
}

ZoyeeUtils::CProcess::~CProcess()
{
	if (m_pCmd){
		delete[] m_pCmd;
		m_pCmd = nullptr;
	}

	if (m_pProcessPath){
		delete[] m_pProcessPath;
		m_pProcessPath = nullptr;
	}

	if (m_pPi){
		PROCESS_INFORMATION* pPi = (PROCESS_INFORMATION*)m_pPi;
		delete pPi;
		m_pPi = nullptr;
	}
}

int ZoyeeUtils::CProcess::Start( bool bShowWindow /*= false*/, bool bWaitForClose /*= true*/, int nTimeOutMillisecond /*= -1*/ )
{
	m_pPi = new PROCESS_INFORMATION;
	memset(m_pPi, 0, sizeof(PROCESS_INFORMATION));
	STARTUPINFOA si = {0};
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;

	if (bShowWindow){
		si.wShowWindow = SW_SHOW;
	}else{
		si.wShowWindow = SW_HIDE;
	}

	if(CreateProcessA(m_pProcessPath, m_pCmd, NULL, NULL, TRUE, NULL, NULL, NULL, &si, (PROCESS_INFORMATION*)m_pPi) == false){
		return GetLastError();
	}else{
		if (bWaitForClose){
			if(WaitForSingleObject(((PROCESS_INFORMATION*)m_pPi)->hProcess, nTimeOutMillisecond == -1 ? INFINITE : nTimeOutMillisecond) == WAIT_TIMEOUT){
				return -1;//timeout
			}
			return 0;
		}else{
			return 0;
		}		
	}
	return 0;
}

void* ZoyeeUtils::CProcess::GetProcessHandle()
{
	if (m_pPi){
		PROCESS_INFORMATION* pPi = (PROCESS_INFORMATION*)m_pPi;
		return pPi->hProcess;
	}
	return nullptr;	
}

int ZoyeeUtils::CProcess::GetProcessProcessId()
{
	if (m_pPi){
		PROCESS_INFORMATION* pPi = (PROCESS_INFORMATION*)m_pPi;
		return pPi->dwProcessId;
	}
	return -1;	
}

int ZoyeeUtils::CProcess::StartEx(pEndCallback pCallback, std::string& strOutput, void* hInteraptEvent, int nTimeOutMillisecond /*= -1*/ )
{
	strOutput.clear();
	HANDLE hRead;
	HANDLE hWrite;
	STARTUPINFOA si;
	DWORD dwExitCode = 0;
	SECURITY_ATTRIBUTES sa;
	PROCESS_INFORMATION pi;

	memset(&si, 0, sizeof(si));
	memset(&pi, 0, sizeof(pi));
	memset(&sa, 0, sizeof(sa));

	sa.nLength = sizeof(sa);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;

	if ( ! CreatePipe(&hRead, &hWrite, &sa, 0)){
		strOutput = "CreatePipeFail";
		if (pCallback){
			pCallback(-1, -1, strOutput);
		}		
		return -1;
	}

	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdOutput = si.hStdError = hWrite;
	si.hStdInput = stdin;

	if ( ! CreateProcessA(m_pProcessPath, m_pCmd, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
		CloseHandle(hRead);
		CloseHandle(hWrite);
		char szError[128] = "";
		sprintf(szError, "CreateProcess Fail!, ErrorCode:[%d]", GetLastError());
		strOutput = szError;
		if (pCallback){
			pCallback(-1, -1, strOutput);
		}
		return -2;
	}

	DWORD dwStartTime = GetTickCount();
	DWORD dwTotal = 0;
	while(1){
		if (nTimeOutMillisecond != -1){
			if (GetTickCount() - dwStartTime > nTimeOutMillisecond){//timeout
				::TerminateThread(pi.hThread, -3);
				::TerminateProcess(pi.hProcess, -3);
				break;
			}
		}

		if(hInteraptEvent != nullptr && WaitForSingleObject(hInteraptEvent, 0) != WAIT_TIMEOUT){
			strOutput = "Interrapt process";
			::TerminateThread(pi.hThread, -4);
			::TerminateProcess(pi.hProcess, -4);
			GetExitCodeProcess(pi.hProcess, &dwExitCode);
			CloseHandle(hWrite);
			CloseHandle(hRead);
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
			if (pCallback){
				pCallback(pi.dwProcessId, dwExitCode, strOutput);
			}
			return -1;
		}

		if ( ! PeekNamedPipe(hRead, NULL, 0, NULL, &dwTotal, NULL)){
			break;
		}

		if (dwTotal == 0){
			DWORD dwState = WaitForSingleObject(pi.hProcess, 100);
			if (dwState == WAIT_TIMEOUT){
				continue;
			}else if(dwState == WAIT_OBJECT_0){
				if ( ! PeekNamedPipe(hRead, NULL, 0, NULL, &dwTotal, NULL))
					break;				
				if (dwTotal == 0)
					break;
				else
					continue;
			}
		}
		std::string str;
		str.resize(dwTotal);
		DWORD read = 0;
		if ( ! ReadFile(hRead, (void*)str.data(), dwTotal, &read, NULL))
			break;
		strOutput += str.c_str();
	}

	GetExitCodeProcess(pi.hProcess, &dwExitCode);
	CloseHandle(hWrite);
	CloseHandle(hRead);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	if (pCallback){
		pCallback(pi.dwProcessId, dwExitCode, strOutput);
	}

	if (dwExitCode != 0){
		return -3;
	}
	return 0;
}

std::string ToChar( wchar_t* pSrc )
{
	std::string str;
	int nLen = ::WideCharToMultiByte(CP_ACP, 0, (wchar_t*)pSrc, -1, 0, 0, 0, 0);
	if (nLen <= 0){
		return "";
	}
	str.resize(nLen);
	WideCharToMultiByte(CP_ACP, 0, (wchar_t*)pSrc, -1, (char*)str.data(), nLen, 0, 0);
	return str;
}

int ZoyeeUtils::CProcess::GetProcessId( char* pProcessName )
{
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);

	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return -1;
	}
	int nCount = 0;
	BOOL bMore = ::Process32First(hProcessSnap, &pe32);  
	while(bMore){
		std::string strProcessName = ToChar(pe32.szExeFile);
		if (stricmp(strProcessName.c_str(), pProcessName) == 0)
		{
			return pe32.th32ProcessID;
		}
		bMore = ::Process32Next(hProcessSnap, &pe32);
	}
	return -1;
}

char* ZoyeeUtils::CProcess::GetProcessName( int nPid )
{
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);

	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return "";
	}
	int nCount = 0;
	BOOL bMore = ::Process32First(hProcessSnap, &pe32);  
	std::string strProcessName;
	while(bMore){
		if (pe32.th32ProcessID == nPid){
			return (char*)ToChar(pe32.szExeFile).c_str();
		}
		bMore = ::Process32Next(hProcessSnap, &pe32);
	}
	return "";
}

CProcess::CProcess(char* pProcessPath, char* pFmt, ...){
	int nLen = printf(pProcessPath);
	m_pProcessPath = new char[nLen + 1];
	strcpy(&m_pProcessPath[0], pProcessPath);

	va_list ap;
	va_start(ap, pFmt);
	nLen = vprintf(pFmt, ap);
	if (nLen > 0){
		m_pCmd = new char[nLen + 1];
		vsprintf(m_pCmd, pFmt, ap);
	}else{
		m_pCmd = nullptr;
	}
	va_end(ap);
	m_pPi = nullptr;
}