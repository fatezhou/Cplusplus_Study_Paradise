#ifndef __DATA_BLOCK_H__
#define __DATA_BLOCK_H__

#include "../include/global.h"



class CDataBlock{
public:
	CDataBlock(){};
	CDataBlock(const emDataType dataType);

	void PushStr(const tstring& strKey, const tstring& strValue, const tstring& strId);
	void PushInt(const tstring& strKey, const int n, const tstring& strId);
	void PushBool(const tstring& strKey, const bool b, const tstring& strId);

	std::vector<tstring> FindInt(const tstring& strKey, int nValue);
	std::vector<tstring> FindStr(const tstring& strKey, tstring strValue);
	std::vector<tstring> FindBool(const tstring& strKey, bool bValue);

	void PushPureData(const tstring& strGuid, const tstring& strJson);
	tstring GetPureData(const tstring& strGuid);

private:
	emDataType m_dataType;

	std::map<tstring, tstring> m_mapPureStore;
	mapTStringMapInt m_mapInt;
	mapTStringMapSTR m_mapStr;
	mapTStringMapBOOL m_mapBool;
	
	std::mutex m_mapIntMutex;
	std::mutex m_mapStrMutex;
	std::mutex m_mapBoolMutex;
	std::mutex m_mapPureStoreMutex;
};
#endif