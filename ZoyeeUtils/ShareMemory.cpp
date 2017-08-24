#include "ShareMemory.h"
#include <string>

#define MaxLenSize 1024

#define GLOBAL "Global\\"

int ZoyeeUtils::CShareMemoryClient::Init( const char* pConnectString )
{
	SetRecvFunctionPtr(DefaultRecv);
	this->strConnectString = pConnectString;
	return Connect(pConnectString);
}

int ZoyeeUtils::CShareMemoryClient::OnRecvThread( void* pParam )
{
	CShareMemoryClient* pThis = (CShareMemoryClient*)pParam;
	
	char szId[16];
	sprintf(szId, "%d", pThis->GetId());
	std::string strEvent = GLOBAL + pThis->strConnectString + "_c_" + szId;
	for (;;){
		if(WaitForSingleObject(pThis->hLocalMapEvent, INFINITE) == WAIT_OBJECT_0){
			int nLen = 0;
			memcpy(&nLen, pThis->pLocalMapBuff, 2);
			pThis->m_pfnRecv(&pThis->pLocalMapBuff[2], nLen, pThis);
		}else{
			return 0;
		}		
	}
	return 0;
}

int ZoyeeUtils::CShareMemoryClient::Send( const char* pBuff, const int nLen )
{
	char szId[16];
	sprintf(szId, "%d", GetId());
	std::string strEventName = GLOBAL + strConnectString + "_s_" + szId;
	std::string strRemoteMapName = strConnectString + "_s_" + szId;

	for (int i = 0; i < 5; i++){
		HANDLE hRemoteEvent = OpenEventA(EVENT_ALL_ACCESS, false, strEventName.c_str());
		HANDLE hRemoteMap = OpenFileMappingA(FILE_MAP_ALL_ACCESS, 0, strRemoteMapName.c_str());

		if (hRemoteEvent && hRemoteMap){
			ResetEvent(hRemoteEvent);
			char* pMapBuff = (char*)MapViewOfFile(hRemoteMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
			memset(pMapBuff, 0, nLen + 3);
			memcpy(pMapBuff, &nLen, 2);
			memcpy(&pMapBuff[2], pBuff, nLen);
			SetEvent(hRemoteEvent);
			return nLen;
		}else{
			Sleep(100);
		}		
	}
	
	return -1;
}

int ZoyeeUtils::CShareMemoryClient::Connect( const char* pConnectString )
{
	//connect the connection pool
	std::string strConnectionPoolEvent = GLOBAL + this->strConnectString + "_wait";
	hConnectionPoolEvent = OpenEventA(EVENT_ALL_ACCESS, false, strConnectionPoolEvent.c_str());
	WaitForSingleObject(hConnectionPoolEvent, INFINITE);
	//enter pool, then get id
	std::string strMapName = strConnectString + "_wait";
	HANDLE hConnectionPoolMap = OpenFileMappingA(FILE_MAP_ALL_ACCESS, 0, strMapName.c_str());
	if (hConnectionPoolMap != INVALID_HANDLE_VALUE){
		std::string strConnectionPoolWriteLock = GLOBAL + this->strConnectString + "_waitLock";
		HANDLE hConnectionPoolWriteLock = OpenEventA(EVENT_ALL_ACCESS, false, strConnectionPoolWriteLock.c_str());
		ResetEvent(hConnectionPoolWriteLock);
		char* pConnectionPoolMapBuff = (char*)MapViewOfFile(hConnectionPoolMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		std::string strConnectionId = &pConnectionPoolMapBuff[1];
		SetId(atoi(strConnectionId.c_str()));
		pConnectionPoolMapBuff[0] = '1';
		SetEvent(hConnectionPoolWriteLock);
		//get id over

		std::string strLocalMap = this->strConnectString + "_c_" + strConnectionId;
		this->hLocalMap = CreateFileMappingA(INVALID_HANDLE_VALUE,
			NULL,
			PAGE_READWRITE,
			0,
			MaxLenSize,
			strLocalMap.c_str());
		std::string strEventName = GLOBAL + this->strConnectString + "_c_" + strConnectionId;
		this->hLocalMapEvent = CreateEventA(NULL, false, false, strEventName.c_str());
		//make localmap to write
		pLocalMapBuff = (char*)MapViewOfFile(this->hLocalMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)OnRecvThread, this, 0, 0));
		return 0;
	}
	return -1;
}

int ZoyeeUtils::CShareMemoryServer::Init( const char* pConnectString )
{
	SetRecvFunctionPtr(DefaultRecv);
	char szId[4];
	this->strConnectString = pConnectString;
	sprintf(szId, "%d", GetId());
	std::string strEventName = GLOBAL + strConnectString + "_s_" + szId;
	std::string strMapName = strConnectString + "_s_" + szId;
	this->hLocalMap = CreateFileMappingA(INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		MaxLenSize,
		strMapName.c_str());

	this->hLocalMapEvent = CreateEventA(NULL, false, false, strEventName.c_str());
	this->pLocalMapBuff = (char*)MapViewOfFile(this->hLocalMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)OnRecvThread, this, 0, 0));
	return 0;
}

int ZoyeeUtils::CShareMemoryServer::Send( const char* pBuff, const int nLen )
{
	char szId[4];
	sprintf(szId, "%d", GetId());
	std::string strMap = GLOBAL + strConnectString + "_c_" + szId;
	HANDLE hRemoteMap, hRemoteEvent;

	for (int i = 0; i < 5; i++){
		hRemoteMap = OpenFileMappingA(FILE_MAP_ALL_ACCESS, false, strMap.c_str());
		hRemoteEvent = OpenEventA(EVENT_ALL_ACCESS, false, strMap.c_str());

		if (hRemoteMap && hRemoteEvent){		
			ResetEvent(hRemoteEvent);
			char* pRemoteBuff = (char*)MapViewOfFile(hRemoteMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
			memset(pRemoteBuff, 0, MaxLenSize);
			memcpy(pRemoteBuff, &nLen, 2);
			memcpy(&pRemoteBuff[2], pBuff, nLen);
			SetEvent(hRemoteEvent);
			return nLen;
		}else{
			Sleep(100);
		}
	}	
	return -1;
}

int ZoyeeUtils::CShareMemoryServer::Connect( const char* pConnectString )
{
	return 0;
}

int ZoyeeUtils::CShareMemoryServer::OnRecvThread( void* pParam )
{
	CShareMemoryServer* pThis = (CShareMemoryServer*)pParam;
	char szId[4];
	sprintf(szId, "%d", pThis->GetId());
	std::string strLocal = GLOBAL + pThis->strConnectString + "_s_" + szId;
	int nLen = 0;

	for (;;){
		if(WAIT_OBJECT_0 == WaitForSingleObject(pThis->hLocalMapEvent, INFINITE)){
			nLen = 0;
			memcpy(&nLen, pThis->pLocalMapBuff, 2);
			pThis->m_pfnRecv(&pThis->pLocalMapBuff[2], nLen, pThis);
		}else{
			return 1;
		}	
	}
	return 0;
}

int ZoyeeUtils::CShareMemoryServerManage::Init( const char* pConnectString )
{
	SetRecvFunctionPtr(DefaultRecv);
	for (int i = 0; i < 256; i++){
		bIdPools[i] = true;
	}

	//make connection pool
	strConnectString = pConnectString;
	std::string strWait = GLOBAL + strConnectString + "_wait";
	std::string strWaitLock = GLOBAL + strConnectString + "_waitLock";
	hWaitEvent = CreateEventA(NULL, false, false, strWait.c_str());
	hWaitWriteLock = CreateEventA(NULL, false, false, strWaitLock.c_str());
	std::string strWaitMapName = strConnectString + "_wait";
	hWaitMap = CreateFileMappingA(INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		MaxLenSize,
		strWaitMapName.c_str());

	pWaitPoolBuff = (char*)MapViewOfFile(hWaitMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	ResetEvent(hWaitWriteLock);
	ResetEvent(hWaitEvent);
	pWaitPoolBuff[0] = '0';
	bIdPools[0] = false;
	sprintf(&pWaitPoolBuff[1], "%d", 0);
	SetEvent(hWaitEvent);
	SetEvent(hWaitWriteLock);
	CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)OnRecvThread, this, 0, 0));
	return 0;
}

int ZoyeeUtils::CShareMemoryServerManage::Send( const char* pBuff, const int nLen )
{
	return 0;
}

int ZoyeeUtils::CShareMemoryServerManage::Connect( const char* pConnectString )
{
	return 0;
}

int ZoyeeUtils::CShareMemoryServerManage::OnRecvThread( void* pParam )
{
	CShareMemoryServerManage* pThis = (CShareMemoryServerManage*)pParam;
	for (;;){
		DWORD dwRes = WaitForSingleObject(pThis->hWaitEvent, 1000);
		if (dwRes == WAIT_OBJECT_0){
			//client enter
			ResetEvent(pThis->hWaitWriteLock);
			if (pThis->pWaitPoolBuff[0] == '1'){
				int nLastId = atoi(&pThis->pWaitPoolBuff[1]);
				CShareMemoryServer* pServerUnit = new CShareMemoryServer;
				pServerUnit->SetId(nLastId);
				pServerUnit->Init(pThis->strConnectString.c_str());
				pThis->listShareMemoryUnit.push_back(pServerUnit);

				pThis->bIdPools[nLastId] = false;
				for (int i = 0; i < 256; i++){
					if (pThis->bIdPools[i]){
						nLastId = i;
						break;
					}
				}
				sprintf(&pThis->pWaitPoolBuff[1], "%d", nLastId);
				pThis->pWaitPoolBuff[0] = '0';
				SetEvent(pThis->hWaitWriteLock);
				//SetEvent(pThis->hWaitEvent);
			}
		}else if (dwRes == WAIT_TIMEOUT){
			SetEvent(pThis->hWaitEvent);
		}
	}
	return 0;
}
