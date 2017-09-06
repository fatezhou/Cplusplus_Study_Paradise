#ifndef __TOOLS_H__
#define __TOOLS_H__

#include "../include/global.h"

#include <chrono>

class CTools{
public:
	static tstring GetLocalTime();
	static tstring GetProcessDir();
	static tstring ToTString(std::string str);
	static std::string ToString(tstring str);
	static tstring GuidMaker();
	static tstring ToBase64(const tstring& strSrc);
	static tstring FromBase64(const tstring& strSrc);
	static int CreateDir(tstring& strDirPath);
	static tstring LoadFile(const tstring& strPath, int nLen = -1);
};

#endif