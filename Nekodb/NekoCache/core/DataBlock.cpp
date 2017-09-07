#include "DataBlock.h"
#include <ctime>

Neko::CDataBlock::CDataBlock(const std::string& strId, const std::string& strData, long lTimeoutSecond /*= 500*/)
{
	this->m_strId = strId;
	this->m_strData = strData;
	this->m_lInsertTime = time(0);
	this->m_lFutureTimeoutTime = time(0) + lTimeoutSecond;
	this->m_lTimeout = lTimeoutSecond;
}

Neko::CDataBlock::~CDataBlock()
{

}

std::string& Neko::CDataBlock::GetId()
{
	return this->m_strId;
}

std::string& Neko::CDataBlock::GetData()
{
	return this->m_strData;
}

long Neko::CDataBlock::GetFutureTimeoutTime()
{
	return this->m_lFutureTimeoutTime;
}

void Neko::CDataBlock::RefreshTimeout()
{
	this->m_lInsertTime = time(0);
	this->m_lFutureTimeoutTime = time(0) + this->m_lTimeout;
}

void Neko::CDataBlock::SetData(const std::string& strData, bool bRefreshTime /*= true*/)
{
	this->m_strData = strData;
	if (bRefreshTime){
		this->m_lInsertTime = time(0);
		this->m_lFutureTimeoutTime = time(0) + this->m_lTimeout;
	}
}