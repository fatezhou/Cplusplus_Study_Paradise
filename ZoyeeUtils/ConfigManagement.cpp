#include "ConfigManagement.h"
#include <tchar.h>
#include <iostream>

#define CheckPath(type) if(m_pFilePath == 0 || m_pFilePath[0] == 0)return type;

DWORD WINAPI ZoyeeUtils::CConfigManagement::MonitorThread( void* pParam )
{
	ZoyeeUtils::CConfigManagement* pThis = (ZoyeeUtils::CConfigManagement*)pParam;

	if (pThis->m_pFilePath){
		char szDirPath[1024];
		strcpy(&szDirPath[0], pThis->m_pFilePath);
		char* ptr = strrchr(szDirPath, '\\');
		if (ptr){
			*ptr = 0;
		}
		HANDLE hDir = CreateFileA(szDirPath, FILE_LIST_DIRECTORY, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING,   
			FILE_FLAG_BACKUP_SEMANTICS, NULL);
		if (hDir == INVALID_HANDLE_VALUE){
			CloseHandle(hDir);
			return GetLastError();
		}

		const int nBuffSize = 1024;
		TCHAR tszBuff[nBuffSize];
		DWORD dwBufWrittenSize;

		while(1){
			if (ReadDirectoryChangesW(hDir, &tszBuff, nBuffSize, FALSE, FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_ATTRIBUTES 
				, &dwBufWrittenSize, NULL, NULL)){
					FILE_NOTIFY_INFORMATION * pfiNotifyInfo = (FILE_NOTIFY_INFORMATION*)tszBuff;
					DWORD dwNextEntryOffset;

					do{
						dwNextEntryOffset = pfiNotifyInfo->NextEntryOffset;  
						DWORD dwAction = pfiNotifyInfo->Action;   
						DWORD dwFileNameLength = pfiNotifyInfo->FileNameLength; 

						if(dwNextEntryOffset != 0)  
						{  
							pfiNotifyInfo= (FILE_NOTIFY_INFORMATION*)((BYTE*)pfiNotifyInfo + dwNextEntryOffset);  
						}  
						if(pfiNotifyInfo->FileNameLength > 0){
							char szFileName[1024] = {0};
							TCHAR tszFileName[1024] = {0};							
							memcpy(tszFileName, pfiNotifyInfo->FileName, pfiNotifyInfo->FileNameLength);
							[&](){
								int nLen = ::WideCharToMultiByte(CP_ACP, 0, (wchar_t*)tszFileName, -1, 0, 0, 0, 0);
								if (nLen <= 0){
									return;
								}
								WideCharToMultiByte(CP_ACP, 0, (wchar_t*)tszFileName, -1, szFileName, nLen, 0, 0);
							}();
							pThis->m_callbackFunction(szFileName);
						}
					}while (dwNextEntryOffset != 0);
			}
		}
	}
	return 0;
}

ZoyeeUtils::CConfigManagement::CConfigManagement( const char* pPath ) : m_pFilePath(nullptr)
{
	SetMonitorCallback(nullptr);
	if (pPath){
		m_pFilePath = new char[strlen(pPath) + 1];
		strcpy(&m_pFilePath[0], pPath);
	}
}

void ZoyeeUtils::CConfigManagement::defaultCallbackFunction( const char* pFilePath )
{
	printf("file [%s] is changed\n", pFilePath);
}

void ZoyeeUtils::CConfigManagement::SetMonitorCallback( pConfigFileChanged _callback )
{
	if (_callback == nullptr){
		this->m_callbackFunction = defaultCallbackFunction;
	}else{
		this->m_callbackFunction = _callback;
	}
}

ZoyeeUtils::CConfigManagement::CConfigManagement(){
	SetMonitorCallback(nullptr);
}

std::list<std::string> ZoyeeUtils::CConfigManagement::GetSection()
{
	std::list<std::string> listSetion;
	char szSection[10240];
	CheckPath(listSetion);
	int nLens = GetPrivateProfileSectionNamesA(szSection, 10240, m_pFilePath);
	char* pSection = szSection;
	for (int i = 0; i < nLens; i++){		
		if (szSection[i] == 0){
			listSetion.push_back(pSection);
			pSection = &szSection[i + 1];
		}
	}
	return listSetion;
}

std::list<std::string> ZoyeeUtils::CConfigManagement::GetKeys( const char* pSection )
{
	std::list<std::string> listKeys;
	char szKeys[10240];
	CheckPath(listKeys);
	int nLen = GetPrivateProfileSectionA(pSection, szKeys, 10240, m_pFilePath);
	char* pKey = szKeys;
	for (int i = 0; i < nLen; i++){
		if (szKeys[i] == 0){
			std::string strKey(pKey);
			int nFindEqualPostion = strKey.find("=");
			if (nFindEqualPostion != -1){
				listKeys.push_back(strKey.substr(0, nFindEqualPostion));
			}
			pKey = &szKeys[i + 1];
		}
	}
	return listKeys;
}

void ZoyeeUtils::CConfigManagement::SetValue( const char* pSection, const char* pKey, const char* pValue )
{
	CheckPath(;);
	WritePrivateProfileStringA(pSection, pKey, pValue, m_pFilePath);
}

void ZoyeeUtils::CConfigManagement::SetSection( char* pOld, char* pNew )
{
	CheckPath(;);
	WriteProfileSectionA(pOld, pNew);
}
