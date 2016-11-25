#ifndef __THREAD_EX_H__
#define __THREAD_EX_H__

#include <Windows.h>
#include <vector>
#include <queue>

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

	protected:
		CTask* GetTask();
		static DWORD WINAPI OnThread(void* pParam);
	private:
		void initParam();
		int m_nPoolSize;		
		std::queue<CTask*> m_queTasks;
		CRITICAL_SECTION cs;

		HANDLE hEvent;
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