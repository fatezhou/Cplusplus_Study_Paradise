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
		iter->second->RefreshTimeout();
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
	std::string strId;
	long lMin = 0x0fffffff;	
	for (auto iter = m_mapCached.begin(); iter != m_mapCached.end(); iter++){
		if (iter->second->IsTimeout()){
			m_mapCached.erase(iter);
			return;
		}
		else{
			if (iter->second->GetSpan() < lMin){
				lMin = iter->second->GetSpan();
				strId = iter->first;
			}
		}
	}

	auto iter = m_mapCached.find(strId);
	if (iter != m_mapCached.end()){
		m_mapCached.erase(iter);
	}
}

void Neko::CCached::init()
{
	SetCachedSize();
}
