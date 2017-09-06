#ifndef __DATA_BLOCK_H__
#define __DATA_BLOCK_H__

#include "../../include/global.h"

namespace Neko{
	class CDataBlock{
	public:
		CDataBlock(const std::string& strId, const std::string& strData, long lTimeoutSecond = 500);
		~CDataBlock();

		std::string& GetId();
		std::string& GetData();
		long GetTimeoutSecond();
		long GetSpan();
		bool IsTimeout();
		void RefreshTimeout();
		void SetData(const std::string& strData, bool bRefreshTime = true);

	private:
		std::string m_strId;
		std::string m_strData;
		long m_lInsertTime;
		long m_lTimeoutSecond;
	};
}

#endif