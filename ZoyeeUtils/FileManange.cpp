#include "FileManange.h"
#include <Windows.h>
#include <direct.h>
#include <io.h>
#include <string>
#include <sys/stat.h>

using namespace ZoyeeUtils;
using namespace std;

CFileManange::CFileManange(void)
{
}


CFileManange::~CFileManange(void)
{
}

std::string ZoyeeUtils::CFileManange::LoadFile( const char* pFilePath )
{
	FILE* pFile = fopen(pFilePath, "rb");
	std::string strData;
	if (pFile){
		int nReadSize = 1024;
		char szBuff[1024];
		int nLen = 0;
		while(1){
			nLen = fread(szBuff, 1, 1024, pFile);
			if (nLen <= 0){
				break;
			}
			strData += std::string(szBuff, nLen);
		}
	}
	fclose(pFile);
	return strData;
}

int ZoyeeUtils::CFileManange::WriteFile( const char* pFilePath, std::string strData, bool bAppend )
{
	FILE* pFile = fopen(pFilePath, bAppend ? "a" : "w");
	if (pFile){
		fwrite(strData.data(), 1, strData.length(), pFile);
		fclose(pFile);
		return 0;
	}	
	return GetLastError();
}

int ZoyeeUtils::CFileManange::DelFile( const char* pFilePath )
{
	return remove(pFilePath);
}

int ZoyeeUtils::CFileManange::CpyFile( const char* pSrcFilePath, const char* pDescFilePath, bool bFailIfExist )
{
	return CopyFileA(pSrcFilePath, pDescFilePath, bFailIfExist);
}

DWORD CALLBACK CopyProgressRoutine(
	LARGE_INTEGER TotalFileSize,              // 文件总字节数
	LARGE_INTEGER TotalBytesTransferred,      // 已复制的字节数
	LARGE_INTEGER StreamSize,                 // 当前流的字节数
	LARGE_INTEGER StreamBytesTransferred,     // 当前流已拷贝的字节数
	DWORD dwStreamNumber,                     // 当前流序号
	DWORD dwCallbackReason,                   // 回调函数的状态; 下面有补充... 
	HANDLE hSourceFile,                       // 源文件句柄
	HANDLE hDestinationFile,                  // 目标文件句柄
	LPVOID lpData                             // CopyFileEx 传递的参数指针
	){
		//return PROGRESS_CANCEL;
		CopyFileCallbackFunction pFun = (CopyFileCallbackFunction)lpData;
		return pFun(TotalFileSize.QuadPart, TotalBytesTransferred.QuadPart);
};

int ZoyeeUtils::CFileManange::CpyFileEx( const char* pSrcFilePath, const char* pDescFilePath, bool bFailIfExist, CopyFileCallbackFunction pFunc )
{
	if (pFunc == nullptr){
		return -1;
	}
	return CopyFileExA(pSrcFilePath, pDescFilePath, CopyProgressRoutine, pFunc, 0, 0);
}

int ZoyeeUtils::CFileManange::CreateDir( const char* pDir )
{
	int nLen = strlen(pDir);
	std::string strPath;

	for (int i = 0; i < nLen; i++){
		if (pDir[i] == '\\' || pDir[i] == '/'){
			strPath = std::string(pDir, &pDir[i] - pDir);			
			if (_access(strPath.c_str(), 0 != 0)){
				_mkdir(strPath.c_str());
			}
		}
	}
	if (pDir[nLen -1] != '\\' && pDir[nLen -1] != '/'){
		strPath = pDir;
		if (_access(strPath.c_str(), 0 )!= 0){
			_mkdir(strPath.c_str());
		}	
	}		
	return 0;
}

std::list<std::pair<std::string, bool>> ZoyeeUtils::CFileManange::GetDirFiles( const char* pDir )
{
	list<pair<string, bool>> lstFileInfo;
	WIN32_FIND_DATAA fd;
	HANDLE lFile;
	char szDir[1024] = "";
	int nLen = strlen(pDir);
	sprintf(szDir, "%s%s*.*", pDir, (pDir[nLen - 1] == '\\' || pDir[nLen - 1] == '/') ? "" : "\\");

	lFile = FindFirstFileA(szDir, &fd);
	char szSub[1024];
	while(lFile != INVALID_HANDLE_VALUE)
	{
		if(FindNextFileA(lFile, &fd) == false){
			break;
		}
		if ((fd.dwFileAttributes & 0x10) == 0x10){						
			sprintf(szSub, "%s%s%s\\*.*", pDir, (pDir[nLen - 1] == '\\' || pDir[nLen - 1] == '/') ? "" : "\\", fd.cFileName);
			if (string(fd.cFileName) != "." && string(fd.cFileName) != ".."){
				char szFileName[1024];
				sprintf(szFileName, "%s%s%s", pDir, (pDir[nLen - 1] == '\\' || pDir[nLen - 1] == '/') ? "" : "\\", fd.cFileName);
				lstFileInfo.push_back(make_pair(szFileName, true));
				list<pair<string, bool>> listTemp = GetDirFiles(szFileName);
				for (list<pair<string, bool>>::iterator iter = listTemp.begin(); iter != listTemp.end(); iter++){
					lstFileInfo.push_back(*iter);
				}
			}			
		}else{
			char szFileName[1024];
			sprintf(szFileName, "%s%s%s", pDir, (pDir[nLen - 1] == '\\' || pDir[nLen - 1] == '/') ? "" : "\\", fd.cFileName);
			lstFileInfo.push_back(make_pair(szFileName, false));
		}		
	}
	FindClose(lFile);
	return lstFileInfo;
}

bool ZoyeeUtils::CFileManange::CheckFileExist( const char* pFile )
{
	if (_access(pFile, 0) != 0){
		return false;
	}
	return true;
}

int ZoyeeUtils::CFileManange::DeleteDir( const char* pDir )
{
	char szPath[1024] = "";
	int nLen = strlen(pDir);
	sprintf(szPath, "%s%s*.*", pDir, (pDir[nLen - 1] == '\\' || pDir[nLen - 1] == '/') ? "" : "\\");
	WIN32_FIND_DATAA fd;
	HANDLE lFile;
	lFile = FindFirstFileA(szPath, &fd);

	while(lFile != INVALID_HANDLE_VALUE){
		if (FindNextFileA(lFile, &fd) == false){
			break;
		}

		if (strcmp(fd.cFileName, "..") == 0 || strcmp(fd.cFileName, ".") == 0){
			continue;
		}

		if ((fd.dwFileAttributes & 0x10) == 0x10){
			char szSubDir[1024];
			sprintf(szSubDir, "%s%s%s", pDir, (pDir[nLen - 1] == '\\' || pDir[nLen - 1] == '/') ? "" : "\\", fd.cFileName);
			DeleteDir(szSubDir);
		}else{
			char szFile[1024];
			sprintf(szFile, "%s%s%s", pDir, (pDir[nLen - 1] == '\\' || pDir[nLen - 1] == '/') ? "" : "\\", fd.cFileName);			
			DelFile(szFile);
		}
	}	
	FindClose(lFile);
	return _rmdir(pDir);
}

long ZoyeeUtils::CFileManange::GetFileSize( const char* pFile )
{
	struct _stat info;
	_stat((char*)pFile, &info);
	return info.st_size;
}

