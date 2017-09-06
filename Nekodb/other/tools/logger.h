#ifndef __LOGGER_H__
#define __LOGGER_H__

#include "../include/global.h"
#include "Lock.h"

class CLogger{
public:
	static CLogger* GetInstance();
	void LogSwitch(bool bEnable = true);
	tofstream& Start();
	tofstream& operator<<(const tstring& strText);
	tofstream& operator<<(int nText);

protected:
	tstring MakeLoggerFile();
	void End();
	void Flush();

private:
	CLogger();
	tofstream m_oFileStream;
	bool m_bEnable;
	std::mutex m_lock;
};

#endif