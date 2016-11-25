#include "SocketFactory.h"
#include "SocketServer.h"
#include "SocketClient.h"

using namespace ZoyeeUtils;

SocketFactory* ZoyeeUtils::SocketFactory::GetInstance()
{
	static SocketFactory sockFactory;
	return &sockFactory;
}

ZoyeeUtils::SocketFactory::SocketFactory()
{

}

ISocket* ZoyeeUtils::SocketFactory::MakeSocket( SocketType nType, void* pData )
{
	switch (nType)
	{
	case em_client:
		return new SocketClient((pOnRecv)pData);
		break;
	case em_server:
		return new SocketServer((pOnRecv)pData);
		break;
	default:
		break;
	}
	return nullptr;
}
