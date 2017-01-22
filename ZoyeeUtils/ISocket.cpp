#include "ISocket.h"
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

ZoyeeUtils::ISocket::ISocket( pOnRecv pRecvCallback ) : m_pFunRecvCallback(pRecvCallback), m_nSocket(0)
{
	
}

int ZoyeeUtils::ISocket::Init( char* pSrcIP, int nPort )
{
	this->m_strIP = pSrcIP;
	this->m_nPort = nPort;
	WSAData wsaData;
	WSAStartup(0x0202, &wsaData);
	m_nSocket = socket(AF_INET, SOCK_STREAM, 0);
	//让socket 马上释放端口而不经过timeout_wait
	//BOOL bReuseaddr=TRUE;
	//setsockopt(m_nSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&bReuseaddr, sizeof(BOOL));
	//BOOL  bDontLinger = FALSE; 
	//setsockopt(m_nSocket, SOL_SOCKET, SO_DONTLINGER, (const char*)&bDontLinger, sizeof(BOOL));
	return 0;
}

void ZoyeeUtils::ISocket::Init( char* pSrcIP, int nPort, unsigned int nSocket )
{
	this->m_strIP = pSrcIP;
	this->m_nPort = nPort;
	m_nSocket = nSocket;
}
