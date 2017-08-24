#ifndef __FILE_MEMORY_H__
#define __FILE_MEMORY_H__

#include <Windows.h>
#include <string>

namespace ZoyeeUtils{
	class CFileMemory
	{
	public:
		class CFileInfo{
		public:
			CFileInfo(const char* pFilePath);
			~CFileInfo();
			unsigned char* pFileCtrlPtr;//直接像访问字符数组一样, 访问这个指针就好了
			char* pFilePath;
			DWORD dwHighSize;
			DWORD dwLowSize;
			HANDLE hFile;
			HANDLE hMapFile;
		};
		
		static CFileInfo* FileToMemory(const char* pFilePath, std::string& strError);//不增大文件的读写(文件已存在)
		static CFileInfo* FileToMemory(const char* pFilePath, DWORD dwFileSize, std::string& strError);//增大文件的读写(文件可能不存在)
		static void ReleaseFile(CFileInfo* pFileInfo);
	};
}

#endif
