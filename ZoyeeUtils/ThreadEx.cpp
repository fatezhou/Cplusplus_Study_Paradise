#include "ThreadEx.h"
#define RETRUNIFFALSE(h) if(h == nullptr)return;
//#define LOCK() ZoyeeUtils::CThreadPool::CLocker::CLocker locker(&cs);

ZoyeeUtils::CTask::CTask( void* pParam /*= nullptr*/ ) : m_pParam(pParam), hHandle(0), m_nThreadId(0)
{
}

ZoyeeUtils::CTask::~CTask( void )
{

}

void ZoyeeUtils::CTask::Start(void* pParam)
{
	if (pParam != nullptr){
		m_pParam = pParam;
	}
	hHandle = CreateThread(0, 0, ThreadFunc, this, 0, (DWORD*)&m_nThreadId);
}

void ZoyeeUtils::CTask::Pause()
{
	RETRUNIFFALSE(hHandle);
	SuspendThread(hHandle);
}

void ZoyeeUtils::CTask::Resume()
{
	RETRUNIFFALSE(hHandle);
	ResumeThread(hHandle);
}

void ZoyeeUtils::CTask::Stop()
{
	RETRUNIFFALSE(hHandle);
	TerminateThread(hHandle, 1);
	hHandle = 0;
	m_pParam = 0;
	m_nThreadId = 0;
}

int ZoyeeUtils::CTask::GetThreadId()
{
	return m_nThreadId == 0 ? GetCurrentThreadId() : m_nThreadId;
}

DWORD WINAPI ZoyeeUtils::CTask::ThreadFunc( void* pParam )
{
	ZoyeeUtils::CTask* pThis = (ZoyeeUtils::CTask*)pParam;
	pThis->Run();
	return 0;
}

HANDLE ZoyeeUtils::CTask::GetHandle()
{
	return hHandle;
}

void* ZoyeeUtils::CTask::GetParam()
{
	return m_pParam;
}

void ZoyeeUtils::CTask::SetParam( void* pParam )
{
	this->m_pParam = pParam;
}

ZoyeeUtils::CThreadPool::CLocker::CLocker(CRITICAL_SECTION* pcs)
{
	this->pcs = pcs;
	::EnterCriticalSection(pcs);
}

ZoyeeUtils::CThreadPool::CLocker::~CLocker()
{
	::LeaveCriticalSection(this->pcs);
}

ZoyeeUtils::CThreadPool::CThreadPool(int nSize)
{
	hEvent = CreateEvent(NULL, false, false, NULL);
	initParam();
	m_nPoolSize = nSize;
	for (int i = 0; i < nSize; i++){
		CreateThread(0, 0, OnThread, this, 0, 0);
	}
}

ZoyeeUtils::CThreadPool::~CThreadPool()
{
	::DeleteCriticalSection(&cs);
	for (auto iter = m_ListThreads.begin(); iter != m_ListThreads.end(); iter++){
		CloseHandle(*iter);
	}
}

void ZoyeeUtils::CThreadPool::AddTask( CTask* pTask )
{	
	CLocker locker(&cs);
	m_queTasks.push(pTask);	
	if (m_queTasks.size() == 1){
		SetEvent(hEvent);
	}
}

ZoyeeUtils::CTask* ZoyeeUtils::CThreadPool::GetTask()
{
	CLocker locker(&cs);
	if (m_queTasks.empty() == false){
		CTask* pTask = m_queTasks.front();
		m_queTasks.pop();
		printf("[%d]size:%d\n", GetCurrentThreadId(), m_queTasks.size());
		return pTask;
	}
	return nullptr;
}

void ZoyeeUtils::CThreadPool::initParam()
{
	::InitializeCriticalSection(&cs);
	m_nPoolSize = 0;
}

DWORD WINAPI ZoyeeUtils::CThreadPool::OnThread( void* pParam )
{
	CThreadPool* pThis = (CThreadPool*)pParam;
	while(1){
		CTask* pTask = pThis->GetTask();
		if (pTask != nullptr){
			pTask->Run();	
		}else{
			printf("WaitForSingleObject\n");
			WaitForSingleObject(pThis->hEvent, INFINITE);						
		}
	}
	return 0;
}

void ZoyeeUtils::CThreadPool::StopAllThread()
{
	for (auto iter = m_ListThreads.begin(); iter != m_ListThreads.end(); iter++){
		TerminateThread(*iter, -1);
	}
}
