#include "logger.h"
#include "tools.h"


CLogger* CLogger::GetInstance()
{
	static CLogger logger;
	return &logger;
}

tstring CLogger::MakeLoggerFile()
{
	tstring strDir = CTools::GetProcessDir();
	tstring strLogFileName = CTools::GetLocalTime();

	strDir += _W("\\log\\") + strLogFileName + _W(".log");
	return strDir;
}

void CLogger::LogSwitch(bool bEnable /*= true*/)
{
	this->m_bEnable = bEnable;
}

tofstream& CLogger::Start()
{
	if (m_bEnable == false){
		return m_oFileStream;
	}
	AutoLock lock(m_lock);
	End();
	m_oFileStream << _W("[") << CTools::GetLocalTime() << _W("]");
	return m_oFileStream;
}

void CLogger::End()
{
	Flush();
}

void CLogger::Flush()
{
	m_oFileStream << _W("\n");
	m_oFileStream.flush();
}

tofstream& CLogger::operator<<(const tstring& strText)
{
	if (m_bEnable == false){
		return m_oFileStream;
	}
	AutoLock lock(m_lock);
	m_oFileStream << strText;
	return m_oFileStream;
}

tofstream& CLogger::operator<<(int nText)
{
	if (m_bEnable == false){
		return m_oFileStream;
	}
	AutoLock lock(m_lock);
	m_oFileStream << nText;
	return m_oFileStream;
}

CLogger::CLogger()
{
	tstring strFileName = MakeLoggerFile();
	m_oFileStream.open(strFileName);	
	m_oFileStream.imbue(std::locale("chs"));
	m_bEnable = true;
}
