#include "Cached.h"
#include "DataBlock.h"

#define LockMap() std::lock_guard<std::recursive_mutex> lock(this->m_mutexMap);

Neko::CCached::CCached()
{
	init();
}

int Neko::CCached::Set(const std::string& strId, const std::string& strData, long lTimeSeconds /*= 500*/)
{
	LockMap();
	CDataBlock* pDataBlock = new CDataBlock(strId, strData, lTimeSeconds);
	if (m_mapCached.size() + 1> m_nSize){
		RemoveOneTimeoutData();
	}

	m_mapCached.insert(std::make_pair(strId, pDataBlock));
	m_mapTimeoutChecker.insert(std::make_pair(pDataBlock->GetFutureTimeoutTime(), strId));
	return m_mapCached.size();
}

std::string& Neko::CCached::Get(const std::string strId, bool bRefreshTime /*= true*/)
{
	LockMap();
	auto iter = m_mapCached.find(strId);
	if (iter == m_mapCached.end()){
		return m_strNull;
	}

	if (bRefreshTime){
		long lTimeout = iter->second->GetFutureTimeoutTime();
		std::multimap<long, std::string>::iterator iterBegin, iterEnd;
		iterBegin = m_mapTimeoutChecker.equal_range(lTimeout).first;
		iterEnd = m_mapTimeoutChecker.equal_range(lTimeout).second;

		for (auto timeoutIter = iterBegin; timeoutIter != iterEnd; timeoutIter++){
			if (timeoutIter->second == strId){
				m_mapTimeoutChecker.erase(timeoutIter);
				break;
			}
		}
		iter->second->RefreshTimeout();
		m_mapTimeoutChecker.insert(std::make_pair(iter->second->GetFutureTimeoutTime(), strId));
	}
	return iter->second->GetData();
}

void Neko::CCached::SetCachedSize(int nSize)
{
	this->m_nSize = nSize;
}

int Neko::CCached::GetCachedSize()
{
	return this->m_nSize;
}

void Neko::CCached::RemoveOneTimeoutData()
{	
	LockMap();
	auto iter = m_mapTimeoutChecker.begin();
	if (iter != m_mapTimeoutChecker.end()){
		std::string strId = iter->second;
		m_mapTimeoutChecker.erase(iter);

		auto findIter = m_mapCached.find(strId);
		if (findIter != m_mapCached.end()){
			m_mapCached.erase(findIter);
		}
	}
}

void Neko::CCached::init()
{
	SetCachedSize();
}
