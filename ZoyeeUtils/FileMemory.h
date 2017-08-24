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
			unsigned char* pFileCtrlPtr;//ֱ��������ַ�����һ��, �������ָ��ͺ���
			char* pFilePath;
			DWORD dwHighSize;
			DWORD dwLowSize;
			HANDLE hFile;
			HANDLE hMapFile;
		};
		
		static CFileInfo* FileToMemory(const char* pFilePath, std::string& strError);//�������ļ��Ķ�д(�ļ��Ѵ���)
		static CFileInfo* FileToMemory(const char* pFilePath, DWORD dwFileSize, std::string& strError);//�����ļ��Ķ�д(�ļ����ܲ�����)
		static void ReleaseFile(CFileInfo* pFileInfo);
	};
}

#endif
