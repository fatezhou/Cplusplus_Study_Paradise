#ifndef __SOCKET_SERVER_H__
#define __SOCKET_SERVER_H__

#include "ISocket.h"

namespace ZoyeeUtils{
	class SocketServer : public ISocket
	{
	public:
		SocketServer(pOnRecv pRecvCallback);
		virtual int Init(char* pSrcIP, int nPort);
		virtual int Send(const char* pszBuff, int nLen);
		virtual int Send(const char* pszBuff, int nLen, unsigned int nFailRetryTimes, bool bCallbackResults);
		virtual int Close();

	protected:
		static unsigned int OnAcceptThread(void* lpParam);
		static unsigned int OnRecvThread(void* lpParam);
	};

}

#endif