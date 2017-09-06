#ifndef __FILE_STREAM_H__
#define __FILE_STREAM_H__

#include "../include/global.h"

class CFileStream{
public:
	enum{
		fileRead,
		fileWriteReplace,
		fileWriteAppend
	};
	CFileStream(const tstring& strFileName, int nMode);
	~CFileStream();

	tstring ReadOneData();
	void WriteOneData(const tstring& strText);

	tstring Read(int nLen);
	void Write(const tstring& strText, int nLen);
	void EnableEnCrypt(bool bEncrypt);//no support encrypt now, cause of the speed
private:
	int m_nMode;
	tstring m_strFileName;
	tfstream m_file;
};
#endif