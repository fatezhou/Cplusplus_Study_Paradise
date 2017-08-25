#include "ReadWriteKey.h"

ZoyeeUtils::CReadWriteKey::CReadWriteKey() : m_nState(State_Read), m_nWriteCountRef(0)
{
	::InitializeCriticalSection(&m_csState);
	::InitializeCriticalSection(&m_csWrite);
	::InitializeCriticalSection(&m_csWriteCount);
	m_hWriteWaitEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
}

ZoyeeUtils::CReadWriteKey::~CReadWriteKey()
{
	::DeleteCriticalSection(&m_csState);
	::DeleteCriticalSection(&m_csWrite);
	::DeleteCriticalSection(&m_csWriteCount);
}

void ZoyeeUtils::CReadWriteKey::Lock(emLockType nType /*= normalLock*/)
{
	switch (nType)
	{
	case ZoyeeUtils::readLock:
		return ReadLock();
		break;
	case ZoyeeUtils::writeLock:
	case ZoyeeUtils::normalLock:
	default:
		return WriteLock();
		break;
	}
}

void ZoyeeUtils::CReadWriteKey::UnLock(emLockType nType /*= normalLock*/)
{
	switch (nType)
	{
	case ZoyeeUtils::readLock:
		return ReadUnLock();
		break;
	case ZoyeeUtils::writeLock:
	case ZoyeeUtils::normalLock:		
	default:
		return WriteUnLock();
		break;
	}
}

void ZoyeeUtils::CReadWriteKey::ReadLock()
{
	if (GetState() == State_Read){
		return;
	}else{
		WaitForSingleObject(m_hWriteWaitEvent, INFINITE);
	}
}

void ZoyeeUtils::CReadWriteKey::ReadUnLock()
{
	//donothing
}

void ZoyeeUtils::CReadWriteKey::WriteLock()
{
	AddWriteCount();
	::EnterCriticalSection(&m_csWrite);
}

void ZoyeeUtils::CReadWriteKey::WriteUnLock()
{
	::LeaveCriticalSection(&m_csWrite);
	RemoveWriteCount();
}

void ZoyeeUtils::CReadWriteKey::SetState(int nState)
{
	::EnterCriticalSection(&m_csState);
	m_nState = nState;
	::LeaveCriticalSection(&m_csState);
}

int ZoyeeUtils::CReadWriteKey::GetState()
{
	return m_nState;
}

void ZoyeeUtils::CReadWriteKey::AddWriteCount()
{
	SetEvent(m_hWriteWaitEvent);
	SetState(State_Write);
	::EnterCriticalSection(&m_csWriteCount);
	m_nWriteCountRef++;
	::LeaveCriticalSection(&m_csWriteCount);
}

void ZoyeeUtils::CReadWriteKey::RemoveWriteCount()
{
	::EnterCriticalSection(&m_csWriteCount);
	m_nWriteCountRef--;
	if (m_nWriteCountRef <= 0){
		SetState(State_Read);
		m_nWriteCountRef = 0;
		ResetEvent(m_hWriteWaitEvent);
	}
	::LeaveCriticalSection(&m_csWriteCount);
}
