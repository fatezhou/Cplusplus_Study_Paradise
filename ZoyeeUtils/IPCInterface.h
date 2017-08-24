#ifndef __IPC_INTERFACE_H__
#define __IPC_INTERFACE_H__
#include <string>
#include <Windows.h>

namespace ZoyeeUtils{
	class IPC_Object;
	typedef void(*pfnIPCRecv)(const char* pbuff, const int nLen, IPC_Object* pIpcObj);
	class IPC_Object{
	public:
		virtual int Init(const char* pConnectString) = 0;
		virtual int Send(const char* pBuff, const int nLen) = 0;
		virtual int Connect(const char* pConnectString) = 0;
		int Close();
		
		void SetRecvFunctionPtr(pfnIPCRecv pfn);
		int GetId();
		void SetId(int nId);
	protected:
		HANDLE hRead;
		HANDLE hWrite;
		static void DefaultRecv(const char* pbuff, const int nLen, IPC_Object* pIpcObj);
		pfnIPCRecv m_pfnRecv;
		std::string strConnectString;
		int nId;
	};

	enum IPC_Type{
		Share_Memory_Client,
		Share_Memory_Server
	};

	class IPC_Interface{
	public:
		static IPC_Object* CreateIPCObject(IPC_Type type);
		static void DestroyIPCObject(IPC_Object* ptr);
	};
}

#endif