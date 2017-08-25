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

		/*class DataBlock{
		public:
			DataBlock(int nValue);
			DataBlock(std::string& strValue);
			DataBlock(bool bValue);
			DataBlock(double dValue);
			DataBlock();

			void Init();
			bool IsVaild();
			bool ToBoolen();
			double ToDouble();
			std::string ToString();
			int ToInt();

			std::string strData;			
			bool bIsVaild;
		};*/

		//char* GetValue(const char* pKey, const char* pDefaultValue);
		//int GetValue(const char* pKey, const int nDefaultValue);
		//bool GetValue(const char* pKey, const bool bDefaultValue);
		//void SetValue(const char* pKey, const char* pValue);
		//void SetValue(const char* pKey, const int nValue);
		//void SetValue(const char* pKey, const bool bValue);

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