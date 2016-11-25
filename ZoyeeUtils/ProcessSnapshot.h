#ifndef __PROCESS_SNAPSHOT_H__
#define __PROCESS_SNAPSHOT_H__

#include <list>

namespace ZoyeeUtils{
	class ProcessSnapshot
	{
	public:
		struct ProcessInfo{
			void* hHandle;
			int nPid;
			int nThreadCount;
			int nHandleCount;
			long lWorkSetMemory;
			char szName[128];
		};
		ProcessSnapshot(void);
		~ProcessSnapshot(void);
		static std::list<ProcessInfo> GetProcessInfo(const char* pProcessName = nullptr);
	};
}
#endif