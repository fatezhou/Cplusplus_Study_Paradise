#ifndef __FILE_MANAGE_H__
#define __FILE_MANAGE_H__

#include <string>
#include <list>
namespace ZoyeeUtils{
	enum emCopyFileRes{
		emContinue,
		emCancel,
		emStop,		
		emQuiet
	};
	typedef emCopyFileRes(*CopyFileCallbackFunction)(long lTotalFileSize, long lTotalBytesTransferred);

	class CFileManange
	{
	public:
		CFileManange(void);
		virtual ~CFileManange(void);

		static std::string LoadFile(const char* pFilePath);
		static int WriteFile(const char* pFilePath, std::string strData, bool bAppend = true);

		static int DelFile(const char* pFilePath);
		static int CpyFile(const char* pSrcFilePath, const char* pDescFilePath, bool bFailIfExist = true);
		static int CpyFileEx(const char* pSrcFilePath, const char* pDescFilePath, bool bFailIfExist, CopyFileCallbackFunction pFunc);

		static int CreateDir(const char* pDir);
		static int DeleteDir(const char* pDir);

		static std::list<std::pair<std::string, bool>> GetDirFiles(const char* pDir);
		static bool CheckFileExist(const char* pFile);		
		static long GetFileSize(const char* pFile);
	};
}

#endif