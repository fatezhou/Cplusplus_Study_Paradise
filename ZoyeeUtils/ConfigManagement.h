#ifndef __CONFIG_MANAGEMENT_H__
#define __CONFIG_MANAGEMENT_H__

#include <Windows.h>
#include <list>
#include <string>

typedef void(*pConfigFileChanged)(const char* pFilePath);

namespace ZoyeeUtils{
	class CConfigManagement
	{
	public:
		~CConfigManagement(void);

		CConfigManagement(const char* pPath);
		void SetMonitorCallback(pConfigFileChanged _callback);
		static DWORD WINAPI MonitorThread(void* pParam);

		std::list<std::string> GetSection();
		std::list<std::string> GetKeys(const char* pSetion);
		void SetSection(char* pOld, char* pNew);

		void SetValue(const char* pSection, const char* pKey, const char* pValue);

	private:
		static void defaultCallbackFunction(const char* pFilePath);
		pConfigFileChanged m_callbackFunction;
		CConfigManagement(void);
		char* m_pFilePath;
	};
}
#endif