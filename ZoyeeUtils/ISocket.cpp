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
	return 0;
}

void ZoyeeUtils::ISocket::Init( char* pSrcIP, int nPort, unsigned int nSocket )
{
	this->m_strIP = pSrcIP;
	this->m_nPort = nPort;
	m_nSocket = nSocket;
}
