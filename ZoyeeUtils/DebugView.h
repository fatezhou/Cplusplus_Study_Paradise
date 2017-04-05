#ifndef __DEBUG_VIEW_H__
#define __DEBUG_VIEW_H__

#define MAX_SZ_BUFF_SIZE 4096

#define USE_AUTO_LOCK

#ifdef USE_AUTO_LOCK
#include "AutoLockClass.h"
#endif

#include <vector>
#include <map>
typedef void(*pPrintCallback)(const char* pProcessName, const int nPid, char* pBuff);

namespace ZoyeeUtils{
	class CDebugView
	{
	public:
		CDebugView(std::vector<std::string>& vecProcessName);
		CDebugView(std::vector<int>& vecPid);
		~CDebugView(void);

#pragma pack(push)
#pragma pack(1)
		class DebugViewInfo{
		public:
			int nPid;
			char szBuff[MAX_SZ_BUFF_SIZE - sizeof(int)];
		};		
#pragma pack(pop)
		void Start(bool bAsyc = false);
		void SetPrintFunction(pPrintCallback _callback);

	private:		
		void AddProcess(const char* pProcessName);
		void AddPid(const int nPid);
		void RemoveProcess(const char* pProcessName);
		void RemovePid(const int nPid);

		char* GetProcessName(int nPid);
		int GetProcessId(char* pProcessName);
		void SetMapProcess();
		static int Listen(void* pThis);

		static void DefaultPrint(const char* pProcessName, const int nPid, char* pBuff);
		void Init();

		std::map<int, std::string> m_mapProcess;
		std::map<int, std::string> m_scanProcess;

		bool m_bUsePid;
		pPrintCallback m_func;

#ifdef USE_AUTO_LOCK
		_AUTO_LOCK_CLASS;
#endif
	};
}

#endif