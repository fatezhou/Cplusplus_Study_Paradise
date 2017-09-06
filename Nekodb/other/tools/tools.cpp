#include "tools.h"
#ifdef _WIN32
#include <io.h>
#include <direct.h>
#include <windows.h>
#endif

tstring CTools::GetLocalTime()
{
	auto toTimeT = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	struct tm* ptm = localtime(&toTimeT);
	tchar szTime[60] = { 0 };
	_sntprintf(szTime, 60, _W("%02d%02d%02d%02d%02d%02d"), (int)ptm->tm_year + 1900, (int)ptm->tm_mon + 1, (int)ptm->tm_mday,
		(int)ptm->tm_hour, (int)ptm->tm_min, (int)ptm->tm_sec);
	return szTime;
}

tstring CTools::GetProcessDir()
{
	tchar tszProcess[1024] = { 0 };
	GetModuleFileName(NULL, tszProcess, 1024);
	tstring strProcess(tszProcess);	
	int nFind = strProcess.find_last_of(_W("\\"));
	if (nFind != -1){
		strProcess.erase(nFind, strProcess.npos);
		return strProcess;
	}
	throw _W("GetProcessDir exception");
	return _W("");
}

tstring CTools::ToTString(std::string str)
{
#ifdef UNICODE
	std::wstring wstr;
	int nLen = ::MultiByteToWideChar(CP_ACP, 0, (char*)str.c_str(), -1, NULL, NULL);
	if (nLen <= 0){
		return _W("");
	}
	wstr.resize(nLen);
	MultiByteToWideChar(CP_ACP, 0, (char*)str.c_str(), -1, (wchar_t*)wstr.data(), nLen);
	return wstr.c_str();
#else
	return str;
#endif
}

std::string CTools::ToString(tstring str)
{
	std::string strRes;
	int nLen = ::WideCharToMultiByte(CP_ACP, 0, (wchar_t*)str.c_str(), -1, 0, 0, 0, 0);
	if (nLen <= 0){
		return "";
	}
	str.resize(nLen);
	WideCharToMultiByte(CP_ACP, 0, (wchar_t*)str.c_str(), -1, (char*)strRes.data(), nLen, 0, 0);
	return strRes;
}

tstring CTools::GuidMaker()
{
	char szGUID[64]; 
#ifdef WIN32
	GUID uuid;
	CoCreateGuid(&uuid);
#else
	TmUUID_t    uuid;
	TmMsg_CreateUUID(&uuid);
#endif
	sprintf(szGUID, "%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X",
		uuid.Data1, uuid.Data2, uuid.Data3, uuid.Data4[0], uuid.Data4[1], uuid.Data4[2],
		uuid.Data4[3], uuid.Data4[4], uuid.Data4[5], uuid.Data4[6], uuid.Data4[7]);

	return ToTString(szGUID);
}

tstring CTools::ToBase64(const tstring& strSrc)
{
	//编码表
	std::string str = CTools::ToString(strSrc);
	const char* pSrc = str.c_str();
	int nLen = str.length();
	static const char EncodeTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	std::string strEncode;
	unsigned char Tmp[4] = { 0 };
	int LineLength = 0;
	for (int i = 0; i < (int)(nLen / 3); i++)
	{
		Tmp[1] = *pSrc++;
		Tmp[2] = *pSrc++;
		Tmp[3] = *pSrc++;
		strEncode += EncodeTable[Tmp[1] >> 2];
		strEncode += EncodeTable[((Tmp[1] << 4) | (Tmp[2] >> 4)) & 0x3F];
		strEncode += EncodeTable[((Tmp[2] << 2) | (Tmp[3] >> 6)) & 0x3F];
		strEncode += EncodeTable[Tmp[3] & 0x3F];
		if (LineLength += 4, LineLength == 76) {
			strEncode += "\r\n";
			LineLength = 0;
		}
	}
	//对剩余数据进行编码
	int Mod = nLen % 3;
	if (Mod == 1){
		Tmp[1] = *pSrc++;
		strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
		strEncode += EncodeTable[((Tmp[1] & 0x03) << 4)];
		strEncode += "==";
	}
	else if (Mod == 2){
		Tmp[1] = *pSrc++;
		Tmp[2] = *pSrc++;
		strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
		strEncode += EncodeTable[((Tmp[1] & 0x03) << 4) | ((Tmp[2] & 0xF0) >> 4)];
		strEncode += EncodeTable[((Tmp[2] & 0x0F) << 2)];
		strEncode += "=";
	}
	return ToTString(strEncode);
}

tstring CTools::FromBase64(const tstring& strSrc)
{
	std::string str = CTools::ToString(strSrc);
	const char* pSrc = str.c_str();
	int nLen = str.length();
	static const char DecodeTable[] =
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		62, // '+'
		0, 0, 0,
		63, // '/'
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
		0, 0, 0, 0, 0, 0, 0,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
		0, 0, 0, 0, 0, 0,
		26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
		39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
	};
	//返回值
	std::string strDecode;
	int nValue;
	int OutByte = 0;
	int i = 0;
	while (i < nLen)
	{
		if (*pSrc != '\r' && *pSrc != '\n'){
			nValue = DecodeTable[*pSrc++] << 18;
			nValue += DecodeTable[*pSrc++] << 12;
			strDecode += (nValue & 0x00FF0000) >> 16;
			OutByte++;
			if (*pSrc != '=')	{
				nValue += DecodeTable[*pSrc++] << 6;
				strDecode += (nValue & 0x0000FF00) >> 8;
				OutByte++;
				if (*pSrc != '=')	{
					nValue += DecodeTable[*pSrc++];
					strDecode += nValue & 0x000000FF;
					OutByte++;
				}
			}
			i += 4;
		}
		else{// 回车换行,跳过
			pSrc++;
			i++;
		}
	}
	return ToTString(strDecode);
}

int CTools::CreateDir(tstring& strDirPath)
{
	const char* pDir = ToString(strDirPath).c_str();
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
	if (pDir[nLen - 1] != '\\' && pDir[nLen - 1] != '/'){
		strPath = pDir;
		if (_access(strPath.c_str(), 0) != 0){
			_mkdir(strPath.c_str());
		}
	}
	return 0;
}

tstring CTools::LoadFile(const tstring& strPath, int nLen /*= -1*/)
{
	tifstream file(strPath);

}
