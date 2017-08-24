#ifndef __SHARE_MEMORY_H__
#define __SHARE_MEMORY_H__

#include "IPCInterface.h"
#include <list>

namespace ZoyeeUtils{
	class CShareMemoryClient : public IPC_Object{
	public:
		virtual int Init(const char* pConnectString);
		virtual int Send(const char* pBuff, const int nLen);
		virtual int Connect(const char* pConnectString);

	protected:
		static int OnRecvThread(void* pParam);

	protected:
		HANDLE hConnectionPoolEvent;
		HANDLE hLocalMap;
		HANDLE hLocalMapEvent;
		char* pLocalMapBuff;
	};
	class CShareMemoryServer : public IPC_Object{
	public:
		virtual int Init(const char* pConnectString);
		virtual int Send(const char* pBuff, const int nLen);
		virtual int Connect(const char* pConnectString);

	protected:
		static int OnRecvThread(void* pParam);

	protected:
		HANDLE hConnectionPoolEvent;
		HANDLE hLocalMap;
		HANDLE hLocalMapEvent;
		char* pLocalMapBuff;
	};
	class CShareMemoryServerManage : public IPC_Object{
	public:
		virtual int Init(const char* pConnectString);
		virtual int Send(const char* pBuff, const int nLen);
		virtual int Connect(const char* pConnectString);

	protected:
		static int OnRecvThread(void* pParam);
		HANDLE hWaitMap;
		HANDLE hWaitEvent;
		HANDLE hWaitWriteLock;
		char* pWaitPoolBuff;
		std::list<CShareMemoryServer*> listShareMemoryUnit;
		bool bIdPools[256];
	};
}

#endif