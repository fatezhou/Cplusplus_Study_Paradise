#ifndef __CACHED_H__
#define __CACHED_H__

#include "../../include/global.h"

namespace Neko{
	class CCacheCore{
	public:
		void SetMaxCacheSize(int nSize);
		int GetMaxCacheSize();

		int Set(const std::string& strId, char* pData, int nDataLen, long lTimeout);
		std::vector<std::pair<char*, int>> Get(const std::string& strId, bool bRefreshTimeout = true);
		void Remove(const std::string& strId, bool bAll = false);
		std::vector<std::string> GetIds();

	private:
		int m_nSize;
	};
}

#endif