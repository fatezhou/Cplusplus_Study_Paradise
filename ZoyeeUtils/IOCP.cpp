#include "IOCP.h"
#include "SocketFactory.h"

#define _LOCK_IN_THREAD CAutoLock lock(&pThis->m_key);
#define _LOCK CAutoLock lock(&m_key);


ZoyeeUtils::IOCPServer::IOCPServer( pOnRecv pRecvCallback ) : ISocket(pRecvCallback)
{
	InitializeCriticalSection(&m_key);
	m_hIOCP = INVALID_HANDLE_VALUE;
}

int ZoyeeUtils::IOCPServer::Init( char* pSrcIP, int nPort )
{
	ISocket::Init(pSrcIP, nPort);
	GetIOCPHandle() = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	SOCKADDR_IN sockAddr;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.S_un.S_addr = inet_addr(pSrcIP);
	sockAddr.sin_port = htons(nPort);
	if (bind(GetSocket(), (sockaddr*)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR){
		return GetLastError();
	}

	if (listen(GetSocket(), 10) == SOCKET_ERROR){
		return GetLastError();
	}

	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)OnAcceptThread, this, 0, 0);
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	for (int i = 0; i < 2; i++){
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)OnRecvThread, this, 0, 0);
	}

	return 0;
}

int ZoyeeUtils::IOCPServer::Send( const char* pszBuff, int nLen )
{
	return 0;
}

int ZoyeeUtils::IOCPServer::Send( const char* pszBuff, int nLen, unsigned int nFailRetryTimes, bool bCallbackResults )
{
	return 0;
}

int ZoyeeUtils::IOCPServer::Close()
{
	return 0;
}

unsigned int ZoyeeUtils::IOCPServer::OnAcceptThread( void* lpParam )
{
	IOCPServer* pThis = (IOCPServer*)lpParam;
	while(1){		
		SOCKADDR_IN remoteSock;
		int nRemoteLen;
		SOCKET sockClient;

		nRemoteLen = sizeof(remoteSock);
		sockClient = accept(pThis->GetSocket(), (SOCKADDR*)&remoteSock, &nRemoteLen);

		ISocket* pClient = ZoyeeUtils::SocketFactory::MakeSocket(em_client, (void*)pThis->GetRecvCallback());
		pClient->Init(inet_ntoa(remoteSock.sin_addr), ntohs(remoteSock.sin_port), sockClient);		
		_LOCK_IN_THREAD;
		pThis->GetVecClient().push_back(pClient);
		//把socket与iocp关联
		CreateIoCompletionPort((HANDLE)pClient->GetSocket(), pThis->GetIOCPHandle(), (DWORD)pClient, 0);
		pClient->GetRecvCallback()("Accept", 0, ZoyeeUtils::em_on_connect, pClient);
		IoOperationData* pIoData = nullptr;
		pIoData = new IoOperationData;
		DWORD dwRecvBytes = 0, dwFlags = 0;
		WSARecv(pClient->GetSocket(), &pIoData->databuff, 1, &dwRecvBytes, &dwFlags, &pIoData->overlapped, NULL);
	}
	return 0;
}

unsigned int ZoyeeUtils::IOCPServer::OnRecvThread( void* lpParam )
{
	IOCPServer* pThis = (IOCPServer*)lpParam;
	DWORD dwReadBytes = 0, dwFlags = 0;
	IoOperationData* lpIoOperationData = nullptr;
	ISocket* pClient = nullptr;
	LPOVERLAPPED lpOverlapped;
	
	while(1){
		if (GetQueuedCompletionStatus(pThis->GetIOCPHandle(), &dwReadBytes, (PULONG_PTR)&pClient, (LPOVERLAPPED*)&lpOverlapped, INFINITE) == 0){
			//error socket disconnect not standardly
			dwReadBytes = -1;			
		}
		lpIoOperationData = (IoOperationData*)CONTAINING_RECORD(lpOverlapped, IoOperationData, overlapped);
		if (dwReadBytes <= 0){//socket disconnect and error
			pClient->GetRecvCallback()("disconnect", 0, dwReadBytes == 0 ? ZoyeeUtils::em_on_disconnect : ZoyeeUtils::em_on_error, pClient);
			closesocket(pClient->GetSocket());
			pThis->RemoveClient(pClient);
			delete pClient;
			delete lpIoOperationData;
			continue;
		}
		lpIoOperationData->nLen = dwReadBytes;
		lpIoOperationData->szBuff[dwReadBytes] = 0;
		pClient->GetRecvCallback()(lpIoOperationData->szBuff, dwReadBytes, ZoyeeUtils::em_on_recv, pClient);
		lpIoOperationData->Reset();
		WSARecv(pClient->GetSocket(), &lpIoOperationData->databuff, 1, &dwReadBytes, &dwFlags, &lpIoOperationData->overlapped, NULL);
	}
	return 0;
}

std::vector<ZoyeeUtils::ISocket*>& ZoyeeUtils::IOCPServer::GetVecClient()
{
	return m_vecClient;
}

HANDLE& ZoyeeUtils::IOCPServer::GetIOCPHandle()
{
	return m_hIOCP;
}

ZoyeeUtils::IOCPServer::~IOCPServer()
{
	::DeleteCriticalSection(&m_key);
}

void ZoyeeUtils::IOCPServer::RemoveClient( ISocket* pSocket )
{
	_LOCK;
	std::vector<ISocket*>::iterator iter = m_vecClient.begin();
	for (; iter != m_vecClient.end(); iter++){
		if (*iter == pSocket){
			m_vecClient.erase(iter);
			return;
		}
	}
	return;
}

ZoyeeUtils::IoOperationData::IoOperationData()
{
	memset(&overlapped, 0, sizeof(overlapped));
	this->databuff.buf = this->szBuff;
	this->databuff.len = 1024;
	this->nOperationType = 0;
}

void ZoyeeUtils::IoOperationData::Reset()
{
	memset(&overlapped, 0, sizeof(OVERLAPPED));
	databuff.len = 1024;
	databuff.buf = szBuff;
	nOperationType = 0;
}

ZoyeeUtils::CAutoLock::CAutoLock( CRITICAL_SECTION* pcs ) : m_pcs(pcs)
{
	::EnterCriticalSection(pcs);
}

ZoyeeUtils::CAutoLock::~CAutoLock()
{
	::LeaveCriticalSection(m_pcs);
}
