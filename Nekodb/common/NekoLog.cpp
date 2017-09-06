#include "NekoLog.h"

#define LOG_LOCK() std::lock_guard<std::mutex> lock(this->m_mutex)

Neko::CLog* Neko::CLog::GetInstance()
{
	static CLog logObject;
	return &logObject;
}

void Neko::CLog::SetWriteFunction(std::function<void(const std::string& strText)> func)
{
	this->m_funcWrite = func;
}

std::function<void(const std::string& strText)> Neko::CLog::GetWriteFunction()
{
	return this->m_funcWrite;
}

void Neko::CLog::Debug(const std::string& strModule, const int nLine, char* pFmt, ...)
{
	if (m_nLogLevel > 0){
		return;
	}
	va_list ap;
	va_start(ap, pFmt);
	std::string strText = Format("DEBUG", getLocalTime(), strModule, nLine, pFmt, ap);	
	va_end(ap);
	input(strText);
}

void Neko::CLog::Info(const std::string& strModule, const int nLine, char* pFmt, ...)
{
	if (m_nLogLevel > 1){
		return;
	}
	va_list ap;
	va_start(ap, pFmt);
	std::string strText = Format("INFO ", getLocalTime(), strModule, nLine, pFmt, ap);
	va_end(ap);
	input(strText);
}

void Neko::CLog::Error(const std::string& strModule, const int nLine, char* pFmt, ...)
{
	if (m_nLogLevel > 2){
		return;
	}
	va_list ap;
	va_start(ap, pFmt);
	std::string strText = Format("ERROR", getLocalTime(), strModule, nLine, pFmt, ap);
	va_end(ap);
	input(strText);
}

void Neko::CLog::LoadConfig()
{
#ifdef WIN32
	char szRes[1280];
	char szDir[1024] = { 0 };
	char szConfigPath[1024] = { 0 };
	GetModuleFileNameA(NULL, szDir, 1024);
	char* p = strrchr(szDir, '\\');
	*p = 0;
	sprintf(szConfigPath, "%s\\config.cfg", szDir);

	GetPrivateProfileStringA("set", "log_level", "0", szRes, 1280, szConfigPath);
	ConfigMap["log_level"] = szRes;
	m_nLogLevel = atoi(ConfigMap["log_level"].c_str());

	char szLogFile[1024] = { 0 };
	sprintf(szLogFile, "%s\\log\\%s.log", szDir, getLocalTime().c_str());
	GetPrivateProfileStringA("set", "log_path", szLogFile, szRes, 1280, szConfigPath);
	ConfigMap["log_path"] = szRes;

	m_fstream.open(szRes, std::ios::app);

#else
#endif
}

void Neko::CLog::SetAsync(bool bAsync)
{
	if (bAsync == false){
		if (GetRunThreadState()){//async thread is running, stop the thread
			SetRunThreadState(false);

			std::string strText = Get();
			while ( ! strText.empty()){
				GetWriteFunction()(strText);
				strText = Get();
			}
		}
	}
	else{
		SetRunThreadState(true);
		std::thread(asyncFunction).detach();
	}
	this->m_bAsync = bAsync;
}

bool Neko::CLog::IsAsync()
{
	return this->m_bAsync;
}

std::string Neko::CLog::Format(const std::string& strType, const std::string& strTime, const std::string& strModule, const int nLine, char* pFmt, va_list ap)
{
	char szHeader[512] = { 0 };
	sprintf(szHeader, "[%s][%s][%s][%d]:", strTime.c_str(), strType.c_str(), strModule.c_str(), nLine);
	int nLen = vprintf(pFmt, ap);
	std::string strBuff(nLen + 1, 0);
	vsprintf((char*)strBuff.data(), pFmt, ap);
	std::string strText = szHeader;
	strText += strBuff.c_str();
	return strText;
}

Neko::CLog::CLog()
{
	LoadConfig();
#ifdef WIN32
	hEvent = CreateEvent(NULL, true, true, NULL);
#endif
	SetAsync(false);
	SetWriteFunction(defaultWrite);
}

void Neko::CLog::input(const std::string& strText)
{
	if (CLog::GetInstance()->IsAsync()){
		CLog::GetInstance()->Push(strText);
	}
	else{
		CLog::GetInstance()->GetWriteFunction()(strText);
	}
}

void Neko::CLog::asyncFunction()
{
	std::string strText;
	while (CLog::GetInstance()->GetRunThreadState()){
		strText = CLog::GetInstance()->Get();
		if (strText.empty() == false){
			CLog::GetInstance()->GetWriteFunction()(strText);
		}
		else{
#ifdef WIN32
			ResetEvent(CLog::GetInstance()->hEvent);
#else
			std::this_thread::sleep_for(std::chrono::seconds(1));
#endif
		}
	}
}

void Neko::CLog::defaultWrite(const std::string& strText)
{
	CLog::GetInstance()->Write(strText);
}

std::string Neko::CLog::getLocalTime()
{
	char szTime[60] = { 0 };
#ifndef WIN32
	auto toTimeT = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	struct tm* ptm = localtime(&toTimeT);
	sprintf(szTime, ("%02d%02d%02d%02d%02d%02d"), (int)ptm->tm_year + 1900, (int)ptm->tm_mon + 1, (int)ptm->tm_mday,
		(int)ptm->tm_hour, (int)ptm->tm_min, (int)ptm->tm_sec);
#else
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	sprintf(szTime, "%02d%02d%02d%02d%02d%02d%04d", sysTime.wYear, sysTime.wMonth,
		sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);
#endif
	return szTime;
}

void Neko::CLog::Write(const std::string& strText)
{
	LOG_LOCK();
	m_fstream << strText << std::endl;
	m_fstream.flush();
	printf("[write][%s]\n", strText.c_str());
	return;
}

void Neko::CLog::Push(const std::string& strText)
{
	LOG_LOCK();
	this->m_queue.push(strText);
	SetEvent(hEvent);
}

std::string Neko::CLog::Get()
{
#ifdef WIN32
	WaitForSingleObject(hEvent, INFINITE);
#endif

	LOG_LOCK();
	if (this->m_queue.empty()){
		return "";
	}

	std::string strText = this->m_queue.front();
	this->m_queue.pop();
	return strText;
}

void Neko::CLog::SetRunThreadState(bool bRun)
{
	this->m_bRunThreadState = bRun;
}

bool Neko::CLog::GetRunThreadState()
{
	return this->m_bRunThreadState;
}
