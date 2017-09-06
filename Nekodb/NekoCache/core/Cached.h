#ifndef __CACHED_H__
#define __CACHED_H__

#include "../../include/global.h"

namespace Neko{
	class CDataBlock;

	class CCached{
	public:
		CCached();
		int Set(const std::string& strId, const std::string& strData, long lTimeSeconds = 500);
		std::string& Get(const std::string strId, bool bRefreshTime = true);

		void SetCachedSize(int nSize = 500);
		int GetCachedSize();

	protected:
		void RemoveOneTimeoutData();

	private:
		void init();
		int m_nSize;
		std::map<std::string, CDataBlock*> m_mapCached;
		std::recursive_mutex m_mutexMap;
		std::string m_strNull = "";
	};
}

#endif