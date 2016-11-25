#ifndef __TEXT_CODE_H__
#define __TEXT_CODE_H__

#include <string>

namespace ZoyeeUtils{
	class CTextCode{
	public:
		static bool ToWchar(char* pSrc, wchar_t* pDesc);
		static bool ToChar(wchar_t* pSrc, char* pDesc);
		static bool GBK2UTF8(char* pBuff);
		static bool UTF82GBK(char* pBuff);
		static std::string Base64Encode(const char* pSrc, int nLen);
		static std::string Base64Decode(const char* pSrc, int nLen);
		static std::string Md5(const char* pSrc, int nLen);
	};
}

#endif