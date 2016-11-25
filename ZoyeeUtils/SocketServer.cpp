#include "SocketServer.h"
#include "SocketFactory.h"
#include <WinSock2.h>

class CClientParam{
public:
	CClientParam(ZoyeeUtils::ISocket* pServer, ZoyeeUtils::ISocket* pClient){
		m_pServer = pServer;
		m_pClient = pClient;
	}

	ZoyeeUtils::ISocket* GetServer(){return m_pServer;};
	ZoyeeUtils::ISocket* GetClient(){return m_pClient;};

private:
	ZoyeeUtils::ISocket* m_pServer;
	ZoyeeUtils::ISocket* m_pClient;
};

ZoyeeUtils::SocketServer::SocketServer( pOnRecv pRecvCallback ) : ISocket(pRecvCallback)
{

}

int ZoyeeUtils::SocketServer::Init( char* pSrcIP, int nPort )
{
	ISocket::Init(pSrcIP, nPort);
	sockaddr_in sock;
	sock.sin_family = AF_INET;
	sock.sin_addr.S_un.S_addr = inet_addr(pSrcIP);
	sock.sin_port = htons(nPort);
	if(bind(GetSocket(), (const sockaddr*)&sock, sizeof(sock)) != 0){
		GetRecvCallback()("bind error", 0, ZoyeeUtils::em_on_error, this);
		return -1;
	}

	listen(GetSocket(), 5);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)OnAcceptThread, this, 0, 0);
	return 0;
}

unsigned int ZoyeeUtils::SocketServer::OnAcceptThread( void* lpParam )
{
	SocketServer* pServer = (SocketServer*)lpParam;
	char szMsg[1024] = "";
	sockaddr_in clientSockAddr = {0};
	int nLen = sizeof(clientSockAddr);

	while(1){
		SOCKET sClient = accept(pServer->GetSocket(), (sockaddr*)&clientSockAddr, &nLen);
		sprintf(szMsg, "Accept: %s:%d", inet_ntoa(clientSockAddr.sin_addr), ntohs(clientSockAddr.sin_port));
		ISocket* pClient = ZoyeeUtils::SocketFactory::MakeSocket(em_client, (void*)pServer->GetRecvCallback());
		pClient->Init(inet_ntoa(clientSockAddr.sin_addr), ntohs(clientSockAddr.sin_port), sClient);

		CClientParam* pClientParam = new CClientParam(pServer, pClient);
		pServer->GetRecvCallback()(szMsg, 0, ZoyeeUtils::em_on_connect, pClient);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)OnRecvThread, pClientParam, 0, 0);
	}
	return 0;
}

int ZoyeeUtils::SocketServer::Send( const char* pszBuff, int nLen )
{
	return 0;
}

int ZoyeeUtils::SocketServer::Send( const char* pszBuff, int nLen, unsigned int nFailRetryTimes, bool bCallbackResults )
{
	return 0;
}

int ZoyeeUtils::SocketServer::Close()
{
	return 0;
}

unsigned int ZoyeeUtils::SocketServer::OnRecvThread( void* lpParam )
{
#define MAX_BUFF_LEN 1024
	char szBuff[MAX_BUFF_LEN] = {0};
	int nLen = 0;
	CClientParam* pParam = (CClientParam*)lpParam;
	ZoyeeUtils::ISocket* pServer = pParam->GetServer();
	ZoyeeUtils::ISocket* pClient = pParam->GetClient();
	bool bReturn = false;
	while(1){
		nLen = recv(pClient->GetSocket(), szBuff, MAX_BUFF_LEN, 0);
		switch(nLen){
		case -1://error
			pClient->GetRecvCallback()("error", 0, ZoyeeUtils::em_on_error, pClient);
			bReturn = true;
			break;
		case 0://disconnect
			pClient->GetRecvCallback()("disconnect!", 0, ZoyeeUtils::em_on_disconnect, pClient);
			bReturn = true;
			break;
		default://recv
			pClient->GetRecvCallback()(szBuff, nLen, ZoyeeUtils::em_on_recv, pClient);
			break;
		}
		if (bReturn){
			break;
		}
	}
	delete pClient;
	delete pParam;
	return 0;
}
