#include "dataBlock.h"
#include "../tools/Lock.h"

#pragma warning(disable:4503)

CDataBlock::CDataBlock(const emDataType dataType) : m_dataType(dataType)
{

}

void CDataBlock::PushStr(const tstring& strKey, const tstring& strValue, const tstring& strId)
{
	AutoLock lock(m_mapStrMutex);
	auto iter = m_mapStr.find(strKey);
	if (iter != m_mapStr.end()){
		auto subIter = iter->second.find(strValue);
		if (subIter != iter->second.end()){
			subIter->second.push_back(strId);
		}
		else{			
			VecTString vec;
			vec.push_back(strId);
			iter->second.insert(std::make_pair(strValue, vec));			
		}
	}
	else{
		VecTString vec;
		vec.push_back(strId);
		mapSTR _mapSTR;
		_mapSTR.insert(std::make_pair(strValue, vec));
		m_mapStr.insert(std::make_pair(strKey, _mapSTR));		
	}
}

void CDataBlock::PushInt(const tstring& strKey, const int n, const tstring& strId)
{
	AutoLock lock(m_mapIntMutex);
	auto iter = m_mapInt.find(strKey);
	if (iter != m_mapInt.end()){
		auto subIter = iter->second.find(n);
		if (subIter != iter->second.end()){
			subIter->second.push_back(strId);
		}
		else{
			VecTString vec;
			vec.push_back(strId);
			iter->second.insert(std::make_pair(n, vec));
		}
	}
	else{
		VecTString vec;
		vec.push_back(strId);
		mapINT _mapINT;
		_mapINT.insert(std::make_pair(n, vec));
		m_mapInt.insert(std::make_pair(strKey, _mapINT));
	}
}

void CDataBlock::PushBool(const tstring& strKey, const bool b, const tstring& strId)
{
	AutoLock lock(m_mapBoolMutex);
	auto iter = m_mapBool.find(strKey);
	if (iter != m_mapBool.end()){
		auto subIter = iter->second.find(b);
		if (subIter != iter->second.end()){
			subIter->second.push_back(strId);
		}
		else{
			VecTString vec;
			vec.push_back(strId);
			iter->second.insert(std::make_pair(b, vec));
		}
	}
	else{
		VecTString vec;
		vec.push_back(strId);
		mapBOOL _mapBOOL;
		_mapBOOL.insert(std::make_pair(b, vec));
		m_mapBool.insert(std::make_pair(strKey, _mapBOOL));
	}
}

std::vector<tstring> CDataBlock::FindInt(const tstring& strKey, int nValue)
{
	AutoLock lock(m_mapIntMutex);
	std::vector<tstring> vec;
	auto iter = m_mapInt.find(strKey);
	auto subIter = iter->second.find(nValue);
	vec = subIter->second;
	return vec;
}

std::vector<tstring> CDataBlock::FindStr(const tstring& strKey, tstring strValue)
{
	AutoLock lock(m_mapStrMutex);
	std::vector<tstring> vec;
	auto iter = m_mapStr.find(strKey);
	auto subIter = iter->second.find(strValue);
	vec = subIter->second;
	return vec;
}

std::vector<tstring> CDataBlock::FindBool(const tstring& strKey, bool bValue)
{
	AutoLock lock(m_mapBoolMutex);
	std::vector<tstring> vec;
	auto iter = m_mapBool.find(strKey);
	auto subIter = iter->second.find(bValue);
	vec = subIter->second;
	return vec;
}

void CDataBlock::PushPureData(const tstring& strGuid, const tstring& strJson)
{
	AutoLock lock(m_mapPureStoreMutex);
	m_mapPureStore.insert(std::make_pair(strGuid, strJson));
}

tstring CDataBlock::GetPureData(const tstring& strGuid)
{
	AutoLock lock(m_mapPureStoreMutex);
	auto iter = m_mapPureStore.find(strGuid);
	if (iter != m_mapPureStore.end()){
		return iter->second;
	}
	return _W("");
}
