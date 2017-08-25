#pragma once

namespace ZoyeeUtils{
	enum emLockType{
		readLock,
		writeLock,
		normalLock
	};

	class IKey{
	public:
		virtual ~IKey();
		virtual void Lock(emLockType nType = normalLock) = 0;
		virtual void UnLock(emLockType nType = normalLock) = 0;
		int LockEx(emLockType nType = normalLock, int nTimeoutMilliSeconds = -1);//ªπŒ¥ µœ÷
	};

	class CLock
	{
	public:
		CLock(IKey* pKey, emLockType nType = normalLock);
		virtual ~CLock();
	private:
		IKey* m_pKey;
		emLockType m_nType;
	};

	class KeyMaker{
		enum
		{
			Mutex_Key,
			ReadWrite_Key,
			FIFO_Key,
			FILO_Key
		};
		static IKey* CreateKey(int nType = Mutex_Key);
	};
}

