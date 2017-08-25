#pragma once
#include "ILock.h"
#include <windows.h>

namespace ZoyeeUtils{
	class CReadWriteKey : public IKey
	{
	public:
		CReadWriteKey();
		virtual ~CReadWriteKey();
		virtual void Lock(emLockType nType = normalLock);
		virtual void UnLock(emLockType nType = normalLock);

	protected:
		enum{
			State_Read,
			State_Write
		};

		void ReadLock();
		void ReadUnLock();
		void WriteLock();
		void WriteUnLock();
		void SetState(int nState);
		int GetState();
		void AddWriteCount();
		void RemoveWriteCount();

	private:
		int m_nState;
		CRITICAL_SECTION m_csState;
		CRITICAL_SECTION m_csWrite;
		CRITICAL_SECTION m_csWriteCount;
		HANDLE m_hWriteWaitEvent;
		int m_nWriteCountRef;
	};
}


