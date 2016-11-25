#include "SocketClient.h"
#include <WinSock2.h>

using namespace std;
using namespace ZoyeeUtils;

SocketClient::SocketClient(pOnRecv pRecvCallback) : ISocket(pRecvCallback)
{
}


SocketClient::~SocketClient(void)
{
}

int ZoyeeUtils::SocketClient::Init( char* pSrcIP, int nPort )
{
	ISocket::Init(pSrcIP, nPort);
	sockaddr_in sock;
	sock.sin_family = AF_INET;
	sock.sin_addr.S_un.S_addr = inet_addr(pSrcIP);
	sock.sin_port = htons(nPort);
	if(connect(GetSocket(), (const sockaddr*)&sock, sizeof(sock)) != 0){
		GetRecvCallback()("connect fail!", 0, ZoyeeUtils::em_on_error, this);
		return -1;
	}else{
		GetRecvCallback()("connect ok!", 0, ZoyeeUtils::em_on_connect, this);
	}
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)OnRecvThread, this, 0, 0);
	return 0;
}

int ZoyeeUtils::SocketClient::Send( const char* pszBuff, int nLen )
{
	return send(GetSocket(), pszBuff, nLen, 0);
}

int ZoyeeUtils::SocketClient::Send( const char* pszBuff, int nLen, unsigned int nFailRetryTimes, bool bCallbackResults )
{
	for (unsigned int i = 0; i < nFailRetryTimes; i++){
		if (send(GetSocket(), pszBuff, nLen, 0) != nLen){
			if (bCallbackResults){
				GetRecvCallback()((char*)pszBuff, nLen, em_on_send_fail, this);
			}			
			continue;
		}
		if (bCallbackResults){
			GetRecvCallback()((char*)pszBuff, nLen, em_on_send_ok, this);
		}		
		break;
	}
	return nLen;
}

int ZoyeeUtils::SocketClient::Close()
{
	closesocket(GetSocket());
	return 0;
}

unsigned int ZoyeeUtils::SocketClient::OnRecvThread( void* lpParam )
{
#define MAX_BUFF_LEN 1024
	SocketClient* pClient = (SocketClient*)lpParam;
	char szBuff[MAX_BUFF_LEN] = {0};
	int nLen = 0;

	while(1){
		nLen = recv(pClient->GetSocket(), szBuff, MAX_BUFF_LEN, 0);
		if (nLen > 0){
			pClient->GetRecvCallback()(szBuff, nLen, ZoyeeUtils::em_on_recv, pClient);
			continue;
		}else if(nLen == 0){
			pClient->GetRecvCallback()("disconnect", 0, ZoyeeUtils::em_on_disconnect, pClient);
			break;
		}else{
			pClient->GetRecvCallback()("error", 0, ZoyeeUtils::em_on_error, pClient);
			break;
		}
	}
	return 0;
}
