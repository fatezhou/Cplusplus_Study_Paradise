#include "DataBlock.h"

Neko::CDataBlock::CDataBlock(std::string strId, char* pDataPtr, int nDataLen, long nTime)
{
	this->m_nDataLen = nDataLen;
	this->m_strDataId = strId;
	this->m_pDataPtr = pDataPtr;
	this->m_nTime = nTime;
}