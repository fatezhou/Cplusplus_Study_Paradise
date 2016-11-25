#ifndef __I_SOCKET_H__
#define __I_SOCKET_H__

#include <string>
namespace ZoyeeUtils{
	enum RecvType{
		em_on_connect,
		em_on_disconnect,
		em_on_send_ok,
		em_on_send_fail,
		em_on_recv,
		em_on_error
	};
	class ISocket;	
	typedef void (*pOnRecv)(char* pBuff, int nLen, RecvType type, ISocket* pSocket);
	class ISocket{	
	public:
		ISocket(pOnRecv pRecvCallback);
		virtual int Init(char* pSrcIP, int nPort);
		void Init(char* pSrcIP, int nPort, unsigned int nSocket);
		virtual int Send(const char* pszBuff, int nLen) = 0;
		virtual int Send(const char* pszBuff, int nLen, unsigned int nFailRetryTimes, bool bCallbackResults) = 0;
		virtual int Close() = 0;

		inline std::string GetIP(){return m_strIP;};
		inline int GetPort(){return m_nPort;};
		inline pOnRecv GetRecvCallback(){return m_pFunRecvCallback;};
		inline unsigned int& GetSocket(){return m_nSocket;};

	private:
		pOnRecv m_pFunRecvCallback;
		unsigned int m_nSocket;
		std::string m_strIP;
		int m_nPort;
	};
}

#endif