#ifndef __SOCKET_FACTORY_H__
#define __SOCKET_FACTORY_H__

#include "ISocket.h"

namespace ZoyeeUtils{
	enum SocketType{
		em_IOCP,
		em_PCPT,//Per connection per thread
		em_Select,
		em_client
	};
	class SocketFactory{
	public:
		static SocketFactory* GetInstance();
		static ISocket* MakeSocket(SocketType nType, void* pRecvFunction);
	private:
		SocketFactory();
	};
}

#endif