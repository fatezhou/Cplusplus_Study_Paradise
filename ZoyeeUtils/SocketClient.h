#ifndef __SOCKET_CLIENT_H__
#define __SOCKET_CLIENT_H__
#include "ISocket.h"

namespace ZoyeeUtils{
	class SocketClient : public ISocket
	{
	public:
		SocketClient(pOnRecv pRecvCallback);
		~SocketClient(void);
		virtual int Init(char* pSrcIP, int nPort);
		virtual int Send(const char* pszBuff, int nLen);
		virtual int Send(const char* pszBuff, int nLen, unsigned int nFailRetryTimes, bool bCallbackResults);
		virtual int Close();
	protected:
		static unsigned int OnRecvThread(void* lpParam);
	};
}



#endif