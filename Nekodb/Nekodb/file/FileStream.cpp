#include "FileStream.h"
#include "../tools/tools.h"

CFileStream::CFileStream(const tstring& strFileName, int nMode) : m_strFileName(strFileName),
m_nMode(nMode)
{
	int nOpenType;
	switch (nMode)
	{
	case fileRead:
		nOpenType = std::ios::in;
		break;
	case fileWriteReplace:
		nOpenType = std::ios::out;
		break;
	case fileWriteAppend:
		nOpenType = std::ios::app;
		break;
	default:
		break;
	}
	m_file.open(CTools::ToString(m_strFileName).c_str(), nOpenType);
	m_file.imbue(std::locale("chs"));
}

tstring CFileStream::ReadOneData()
{
	if (m_nMode != fileRead || m_file.is_open() == false){
		return _W("");
	}

	std::string strData;
	std::fstream file(CTools::ToString(m_strFileName).c_str(), std::ios::in);
	std::getline(file, strData);
	//guid:base64data\n

	return CTools::ToTString(strData);
}

void CFileStream::WriteOneData(const tstring& strText)
{
	if (m_nMode == fileRead || m_file.is_open() == false){
		return;
	}

	m_file << strText << _W("\n");
	m_file.flush();
}
