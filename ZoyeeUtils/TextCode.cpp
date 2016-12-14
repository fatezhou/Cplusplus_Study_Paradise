#include "TextCode.h"
#include "3p/md5.h"

#include <Windows.h>
using namespace std;


bool ZoyeeUtils::CTextCode::ToWchar( char* pSrc, wchar_t* pDesc )
{
	int nLen = ::MultiByteToWideChar(CP_ACP, 0, (char*)pSrc, -1, NULL, NULL);
	return (MultiByteToWideChar(CP_ACP, 0, (char*)pSrc, -1, pDesc, nLen) == nLen);
}

bool ZoyeeUtils::CTextCode::ToChar( wchar_t* pSrc, char* pDesc )
{
	int nLen = ::WideCharToMultiByte(CP_ACP, 0, (wchar_t*)pSrc, -1, 0, 0, 0, 0);
	return (WideCharToMultiByte(CP_ACP, 0, (wchar_t*)pSrc, -1, pDesc, nLen, 0, 0) == nLen);
}

bool ZoyeeUtils::CTextCode::GBK2UTF8( char* pBuff )
{
	int nLen = ::MultiByteToWideChar(CP_ACP, 0, (char*)pBuff, -1, NULL, NULL);
	wstring wstrBuff;
	wstrBuff.resize(nLen + 2);
	MultiByteToWideChar(CP_ACP, 0, (char*)pBuff, -1, (wchar_t*)wstrBuff.data(), nLen);

	nLen = ::WideCharToMultiByte(CP_UTF8, 0, (wchar_t*)wstrBuff.data(), -1, NULL, 0, NULL, NULL); 	 
	return ::WideCharToMultiByte(CP_UTF8, 0, (wchar_t*)wstrBuff.data(), -1, (char *)pBuff, nLen, NULL, NULL) == nLen;
}

bool ZoyeeUtils::CTextCode::UTF82GBK( char* pBuff )
{
	int nLen = ::MultiByteToWideChar(CP_UTF8, 0, (char*)pBuff, -1, NULL, NULL);
	wstring wstrBuff;
	wstrBuff.resize(nLen + 2);
	MultiByteToWideChar(CP_UTF8, 0, (char*)pBuff, -1, (wchar_t*)wstrBuff.data(), nLen);

	nLen = ::WideCharToMultiByte(CP_ACP, 0, (wchar_t*)wstrBuff.data(), -1, NULL, 0, NULL, NULL); 	 
	return ::WideCharToMultiByte(CP_ACP, 0, (wchar_t*)wstrBuff.data(), -1, (char *)pBuff, nLen, NULL, NULL) == nLen;
}

std::string ZoyeeUtils::CTextCode::Base64Encode( const char* pSrc, int nLen )
{
	//编码表
	static const char EncodeTable[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	std::string strEncode;
	unsigned char Tmp[4] = {0};
	int LineLength = 0;
	for(int i = 0;i < (int)(nLen / 3); i++)
	{
		Tmp[1] = *pSrc++;
		Tmp[2] = *pSrc++;
		Tmp[3] = *pSrc++;
		strEncode += EncodeTable[Tmp[1] >> 2];
		strEncode += EncodeTable[((Tmp[1] << 4) | (Tmp[2] >> 4)) & 0x3F];
		strEncode += EncodeTable[((Tmp[2] << 2) | (Tmp[3] >> 6)) & 0x3F];
		strEncode += EncodeTable[Tmp[3] & 0x3F];
		if(LineLength += 4, LineLength == 76) {
			strEncode += "\r\n";
			LineLength = 0;
		}
	}
	//对剩余数据进行编码
	int Mod = nLen % 3;
	if(Mod == 1){
		Tmp[1] = *pSrc++;
		strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
		strEncode += EncodeTable[((Tmp[1] & 0x03) << 4)];
		strEncode += "==";
	}else if(Mod == 2){
		Tmp[1] = *pSrc++;
		Tmp[2] = *pSrc++;
		strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
		strEncode += EncodeTable[((Tmp[1] & 0x03) << 4) | ((Tmp[2] & 0xF0) >> 4)];
		strEncode += EncodeTable[((Tmp[2] & 0x0F) << 2)];
		strEncode += "=";
	}
	return strEncode;
}

std::string ZoyeeUtils::CTextCode::Base64Decode( const char* pSrc, int nLen )
{
	//解码表
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
		if (*pSrc != '\r' && *pSrc!='\n'){
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
		}else{// 回车换行,跳过
			pSrc++;
			i++;
		}
	}
	return strDecode;	
}

std::string ZoyeeUtils::CTextCode::Md5( const char* pSrc, int nLen )
{
	MD5_CTX mdt;
	string strOutput;
	unsigned char sz[16] = "";
	MD5Init(&mdt);
	MD5Update(&mdt, (unsigned char*)pSrc, nLen);
	MD5Final(&mdt, sz);
	char szTemp[4];
	for (int i = 0; i < 16; i++){
		sprintf(szTemp, "%02x", sz[i]);
		strOutput += szTemp;
	}
	return strOutput;
}
