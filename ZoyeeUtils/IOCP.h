#ifndef __IOCP_H__
#define __IOCP_H__
#include "ISocket.h"
#include <WinSock2.h>
#include <Windows.h>
#include <vector>

#define MaxDataBuffSize 2048

namespace ZoyeeUtils{
	class IoOperationData{
	public:
		IoOperationData();
		void Reset();
		OVERLAPPED overlapped;
		WSABUF databuff;
		char szBuff[MaxDataBuffSize];
		int nLen;
		int nOperationType;
	};
	class CAutoLock{
	public:
		CAutoLock(CRITICAL_SECTION* pcs);
		~CAutoLock();
	private:
		CRITICAL_SECTION* m_pcs;
	};

	class IOCPServer : public ISocket
	{
	public:
		IOCPServer(pOnRecv pRecvCallback);
		~IOCPServer();
		virtual int Init(char* pSrcIP, int nPort);
		virtual int Send(const char* pszBuff, int nLen);
		virtual int Send(const char* pszBuff, int nLen, unsigned int nFailRetryTimes, bool bCallbackResults);
		virtual int Close();

	protected:
		static unsigned int OnAcceptThread(void* lpParam);
		static unsigned int OnRecvThread(void* lpParam);

		std::vector<ISocket*>& GetVecClient();
		HANDLE& GetIOCPHandle();
		void RemoveClient(ISocket* pSocket);
	private:
		HANDLE m_hIOCP;
		std::vector<ISocket*> m_vecClient;
		CRITICAL_SECTION m_key;
	};
}

#endif