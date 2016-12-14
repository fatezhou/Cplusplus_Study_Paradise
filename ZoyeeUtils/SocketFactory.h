#ifndef __SOCKET_FACTORY_H__
#define __SOCKET_FACTORY_H__

#include "ISocket.h"

namespace ZoyeeUtils{
	enum SocketType{
		em_IOCP,
		em_server,
		em_client
	};
	class SocketFactory{
	public:
		static SocketFactory* GetInstance();
		static ISocket* MakeSocket(SocketType nType, void* pData);
	private:
		SocketFactory();
	};
}

#endif