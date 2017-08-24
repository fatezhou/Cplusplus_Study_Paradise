#ifndef __THREAD_EX_H__
#define __THREAD_EX_H__

#include <Windows.h>
#include <vector>
#include <queue>
#include <list>

namespace ZoyeeUtils{
	class CTask
	{
	public:
		CTask(void* pParam = nullptr);
		~CTask(void);
		virtual void Run() = 0;

		void Start(void* pParam = nullptr);
		void Pause();
		void Resume();
		void Stop();

		int GetThreadId();
		HANDLE GetHandle();
		void* GetParam();
		void SetParam(void* pParam);

		void SetThreadId(int nId);
		void SetThreadHandle(HANDLE hThread);

	protected:
		static DWORD WINAPI ThreadFunc(void* pParam);
	private:
		HANDLE hHandle;
		void* m_pParam;
		int m_nThreadId;
	};

	class CThreadPool{
	public:
		CThreadPool(int nSize = 5);
		~CThreadPool();

		void AddTask(CTask* pTask);		
		void StopAllThread();

	protected:
		CTask* GetTask();
		static DWORD WINAPI OnThread(void* pParam);
	private:
		void initParam();

		int m_nPoolSize;		
		std::queue<CTask*> m_queTasks;
		CRITICAL_SECTION cs;
		HANDLE hEvent;
		std::list<HANDLE> m_ListThreads;

		class CLocker{
		public:
			CLocker(CRITICAL_SECTION* pcs);
			~CLocker();
		private:
			CRITICAL_SECTION* pcs;
		};
	};
}

#endif