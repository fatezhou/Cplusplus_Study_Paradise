#pragma once
#include <windows.h>

namespace ZoyeeUtils{
	class CEncrypt
	{
	public:
		enum BlowfishType{
			ECB = 0,
			CBC = 1,
			CFB = 2
		};

		static std::string BlowFishEncrypt(const std::string& strSrc, const std::string& strPassword, BlowfishType nEncryptType = ECB);
		static std::string BlowFishDecrypt(const std::string& strSrc, const std::string& strPassword, BlowfishType nEncryptType = ECB);

	private:
		static std::string byteHexToString(char* pSrc, int nLen);
		static std::string hexStringToByte(char* pSrc, int nLen);
		static inline BYTE toHex(const BYTE &x)
		{
			return x > 9 ? x - 10 + 'A' : x + '0';
		}

		static inline BYTE toByte(const BYTE &x)
		{
			return (x - '0' <= 9) ? (x - '0') : (x - 'A' + 10);
		}
	};
}


