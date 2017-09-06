#ifndef __TEXT_CODE_H__
#define __TEXT_CODE_H__

#include <string>

namespace ZoyeeUtils{
	class CTextCode{
	public:		
		static std::wstring ToWchar(char* pSrc);
		static std::string ToChar(wchar_t* pSrc);
		static std::string ToUTF8(char* pSrc);
		static std::string ToGBK(char* pSrc);
		static std::string UTF2GBK(char* pBuff);
		static std::string Base64Encode(const char* pSrc, int nLen);
		static std::string Base64Decode(const char* pSrc, int nLen);
		static std::string Md5(const char* pSrc, int nLen);		
	};
}

#endif