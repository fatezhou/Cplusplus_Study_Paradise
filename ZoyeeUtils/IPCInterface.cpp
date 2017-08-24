#include "IPCInterface.h"
#include "ShareMemory.h"
#include <iostream>

using namespace ZoyeeUtils;

void ZoyeeUtils::IPC_Object::SetRecvFunctionPtr( pfnIPCRecv pfn )
{
	if (pfn){
		m_pfnRecv = pfn;
	}else{
		m_pfnRecv = DefaultRecv;
	}
}

void ZoyeeUtils::IPC_Object::DefaultRecv( const char* pbuff, const int nLen, IPC_Object* pIpcObj )
{
	printf("[%p]:[%d][%s]\n", pIpcObj, nLen, pbuff);
}

int ZoyeeUtils::IPC_Object::GetId()
{
	return nId;
}

void ZoyeeUtils::IPC_Object::SetId( int nId )
{
	this->nId = nId;
}

int ZoyeeUtils::IPC_Object::Close()
{
	return this->Send("Close", 5);
}

IPC_Object* ZoyeeUtils::IPC_Interface::CreateIPCObject( IPC_Type type )
{
	switch(type){
	case Share_Memory_Client:
		return new CShareMemoryClient;
		break;
	case Share_Memory_Server:
		return new CShareMemoryServerManage;
		break;
	default:
		break;
	}
	return nullptr;
}

void ZoyeeUtils::IPC_Interface::DestroyIPCObject( IPC_Object* ptr )
{
	delete ptr;
	ptr = nullptr;
}
