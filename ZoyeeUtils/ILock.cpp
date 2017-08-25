#include "ILock.h"
#include "ReadWriteKey.h"
#include <windows.h>

namespace ZoyeeUtils{

	CLock::CLock(IKey* pKey, emLockType nType /*= normalLock*/) : m_pKey(pKey), m_nType(nType)
	{
		m_pKey->Lock(m_nType);
	}

	CLock::~CLock()
	{
		m_pKey->UnLock(m_nType);
	}

	class CNormalKey : public IKey{
	public:
		CNormalKey();
		~CNormalKey();
		void Lock(emLockType nType = normalLock);
		void UnLock(emLockType nType = normalLock);

	private:
		CRITICAL_SECTION cs;
	};

	CNormalKey::CNormalKey()
	{
		::InitializeCriticalSection(&cs);
	}

	CNormalKey::~CNormalKey()
	{
		::DeleteCriticalSection(&cs);
	}

	void CNormalKey::Lock(emLockType nType /*= normalLock*/)
	{
		::EnterCriticalSection(&cs);
	}

	void CNormalKey::UnLock(emLockType nType /*= normalLock*/)
	{
		::LeaveCriticalSection(&cs);
	}

	IKey::~IKey()
	{

	}

	IKey* KeyMaker::CreateKey(int nType /*= Mutex_Key*/)
	{
		switch (nType)
		{
		case Mutex_Key:
			return new CNormalKey;
		case ReadWrite_Key:
			return new CReadWriteKey;
		case FIFO_Key:
		case FILO_Key:
			break;
		default:
			break;
		}
		return nullptr;
	}
}