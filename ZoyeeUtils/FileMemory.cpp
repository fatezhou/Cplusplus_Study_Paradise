#include "FileMemory.h"
#include <iostream>

extern std::string MakeGuid();

ZoyeeUtils::CFileMemory::CFileInfo::CFileInfo( const char* pFilePath )
{
	if (pFilePath){
		int nLen = strlen(pFilePath);
		this->pFilePath = new char[nLen + 1];
		sprintf(this->pFilePath, "%s", pFilePath);
	}
	pFileCtrlPtr = nullptr;
}

ZoyeeUtils::CFileMemory::CFileInfo::~CFileInfo()
{
	if (pFilePath){
		delete pFilePath;
		pFilePath = nullptr;
	}
}

ZoyeeUtils::CFileMemory::CFileInfo* ZoyeeUtils::CFileMemory::FileToMemory( const char* pFilePath, std::string& strError )
{
	do{
		HANDLE hFile = CreateFileA(pFilePath, 
			GENERIC_READ|GENERIC_WRITE, //对文件进行读写操作
			FILE_SHARE_READ|FILE_SHARE_WRITE,
			NULL,     
			OPEN_EXISTING,  //打开已存在文件
			FILE_ATTRIBUTE_NORMAL,   
			0);

		if (hFile == INVALID_HANDLE_VALUE){
			strError.resize(128);
			sprintf((char*)strError.data(), "CreateFileA Fail, ErrorCode:%d", GetLastError());
			break;
		}

		DWORD dwHighSize, dwLowSize;//分别表示文件大小的高32位/低32位
		dwLowSize = GetFileSize(hFile, &dwHighSize);

		//创建映射文件
		HANDLE hMapFile = CreateFileMappingA(hFile, 
			NULL,
			PAGE_READWRITE,  //对映射文件进行读写
			dwHighSize,
			dwLowSize,
			NULL
			);

		if (hMapFile == INVALID_HANDLE_VALUE){
			strError.resize(128);
			sprintf((char*)strError.data(), "CreateFileMappingA Fail, ErrorCode:%d", GetLastError());
			CloseHandle(hFile);
			break;
		}

		void* pFilePtr = MapViewOfFile(
			hMapFile, 
			FILE_MAP_READ|FILE_MAP_WRITE, 
			0,
			0,
			0);  

		CFileInfo* pFileInfo = new CFileInfo(pFilePath);
		pFileInfo->pFileCtrlPtr = (unsigned char*)pFilePtr;
		pFileInfo->dwHighSize = dwHighSize;
		pFileInfo->dwLowSize = dwLowSize;
		pFileInfo->hFile = hFile;
		pFileInfo->hMapFile = hMapFile;
		return pFileInfo;
	} while (false);	
	return nullptr;
}

ZoyeeUtils::CFileMemory::CFileInfo* ZoyeeUtils::CFileMemory::FileToMemory( const char* pFilePath, DWORD dwFileSize, std::string& strError )
{
	do{
		const std::string strShareMemeryName = MakeGuid();
		HANDLE hFile = CreateFileA(
			pFilePath,
			GENERIC_READ|GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, 
			OPEN_ALWAYS,
			FILE_FLAG_SEQUENTIAL_SCAN,
			NULL
			);

		if (hFile == INVALID_HANDLE_VALUE){
			strError.resize(128);
			sprintf((char*)strError.data(), "CreateFileA Fail, ErrorCode:%d", GetLastError());
			break;
		}

		DWORD dwLow, dwHigh;
		dwLow = GetFileSize(hFile, &dwHigh);
		int nErrorCode = GetLastError();
		if (dwLow == 0xFFFFFFFF && nErrorCode != 0){
			strError.resize(128);
			sprintf((char*)strError.data(), "GetFileSize fail, ErrorCode:[%d]", nErrorCode);
			break;
		}

		HANDLE hMap = CreateFileMappingA(hFile, NULL,
			PAGE_READWRITE,
			dwHigh,
			dwFileSize,
			strShareMemeryName.c_str());

		nErrorCode = GetLastError();
		if (hMap == INVALID_HANDLE_VALUE || nErrorCode != 0){
			strError.resize(128);
			sprintf((char*)strError.data(), "CreateFileMappingA Fail, ErrorCode[%d]", nErrorCode);
			CloseHandle(hFile);
			break;
		}

		char* pFileCtrlPtr = (char*)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, dwFileSize);

		CFileInfo* pFileInfo = new CFileInfo(pFilePath);
		pFileInfo->pFileCtrlPtr = (unsigned char*)pFileCtrlPtr;
		pFileInfo->dwHighSize = dwHigh;
		pFileInfo->dwLowSize = dwLow;
		pFileInfo->hFile = hFile;
		pFileInfo->hMapFile = hMap;
		return pFileInfo;		
	} while (false);	
	return nullptr;
}

void ZoyeeUtils::CFileMemory::ReleaseFile( CFileInfo* pFileInfo )
{
	CloseHandle(pFileInfo->hFile);
	CloseHandle(pFileInfo->hMapFile);
	UnmapViewOfFile(pFileInfo->pFileCtrlPtr);
	delete pFileInfo;
	pFileInfo = nullptr;
}
