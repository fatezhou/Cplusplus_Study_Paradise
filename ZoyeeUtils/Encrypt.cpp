#include "3p/Blowfish.h"
#include "Encrypt.h"

using namespace ZoyeeUtils;
union UNION_INT{
	int ival;
	char b[4];
};

std::string CEncrypt::BlowFishEncrypt(const std::string& strSrc, const std::string& strPassword, BlowfishType nEncryptType /*= 0*/)
{
	if (strSrc.empty() || strPassword.empty()){
		return "";
	}
	size_t nSrcLen = strSrc.length();
	size_t nPasswordLen = strPassword.length();

	unsigned int n = nSrcLen + 4;
	n = (n + 7) / 8 * 8;
	int nDescLen = n + 1;

#if (_MSC_VER >= 1700)
	unsigned char* pBuffer = new unsigned char[nDescLen]{0};
#else
	unsigned char* pBuffer = new unsigned char[nDescLen];
	memset(pBuffer, 0, nDescLen);
#endif

	UNION_INT _xInt;
	_xInt.ival = nSrcLen;
	pBuffer[0] = _xInt.b[0];
	pBuffer[1] = _xInt.b[1];
	pBuffer[2] = _xInt.b[2];
	pBuffer[3] = _xInt.b[3];

	memcpy(pBuffer + 4, strSrc.c_str(), nSrcLen);
	CBlowFish blowfish((unsigned char *)strPassword.c_str(), nPasswordLen, CBlowFish::SBlock(1UL, 0UL));
	blowfish.Encrypt((unsigned char *)pBuffer, n, nEncryptType);
	std::string strHex = byteHexToString((char*)pBuffer, nDescLen);
	delete pBuffer;
	return strHex;
}

std::string CEncrypt::BlowFishDecrypt(const std::string& strSrc, const std::string& strPassword, BlowfishType nEncryptType /*= 0*/)
{
	if (strSrc.empty() || strPassword.empty()){
		return "";
	}
	
	size_t nPasswordLen = strPassword.length();
	size_t nSrcLen = strSrc.length() / 2;
	nSrcLen = (nSrcLen + 7) / 8 * 8;
	int nDescLen = nSrcLen + 1;

	std::string strData = hexStringToByte((char*)strSrc.c_str(), strSrc.length());
	CBlowFish blowfish((unsigned char*)strPassword.c_str(), nPasswordLen, CBlowFish::SBlock(1UL, 0UL));
	blowfish.Decrypt((unsigned char*)strData.c_str(), nSrcLen, nEncryptType);

	if (strData.length() <= 4){
		return nullptr;
	}
	UNION_INT _xInt;
	_xInt.b[0] = strData.at(0);
	_xInt.b[1] = strData.at(1);
	_xInt.b[2] = strData.at(2);
	_xInt.b[3] = strData.at(3);
	UINT uTextLen = _xInt.ival;
	std::string strOut;
	strOut.resize(uTextLen);
	strOut = strData.substr(4, uTextLen);		
	return strOut;
}

std::string CEncrypt::byteHexToString(char* pSrc, int nLen)
{
	std::string strOut;
	BYTE buf[3] = { 0 };

	for (unsigned int i = 0; i < nLen; i++)
	{		
		memset(buf, 0, 3);
		buf[0] = toHex((BYTE)pSrc[i] >> 4);
		buf[1] = toHex((BYTE)pSrc[i] % 16);
		strOut += (char *)buf;
	}
	return strOut;
}

std::string CEncrypt::hexStringToByte(char* pSrc, int nLen)
{
	std::string strOut;
	for (unsigned int i = 0; i < nLen / 2; i++)
	{
		int pos = i * 2;
		strOut += (toByte((BYTE)pSrc[pos]) << 4) | (toByte((BYTE)pSrc[pos + 1]));
	}
	return strOut;
}