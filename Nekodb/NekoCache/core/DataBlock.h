#ifndef __DATA_BLOCK_H__
#define __DATA_BLOCK_H__

#include "../../include/global.h"

namespace Neko{
	class CDataBlock{
	public:
		CDataBlock(std::string strId, char* pDataPtr = 0, int nDataLen = 0, long nTime = -1);
		~CDataBlock(){};

		inline std::string GetId(){ return m_strDataId; };
		inline long GetTime(){ return m_nTime; };
		inline char* GetDataPtr(int& nDataLen){ nDataLen = m_nDataLen; return m_pDataPtr; };

	private:		
		CDataBlock(){};
		int m_nDataLen;
		char* m_pDataPtr;
		std::string m_strDataId;
		long m_nTime;
	};
}

#endif