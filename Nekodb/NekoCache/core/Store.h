#ifndef __STORE_H__
#define __STORE_H__

#include "../../include/global.h"

namespace Neko{
	class CDataBlock;

	class CStore{
	public:
		int Set(const std::string& strId, CDataBlock* pBlock);
		std::vector<CDataBlock*> Get(const std::string& strId);

		std::string ToJson(std::vector<CDataBlock*>& vec);
	private:
		std::map<std::string, std::vector<CDataBlock*>> m_mapCache;
	};
}

#endif