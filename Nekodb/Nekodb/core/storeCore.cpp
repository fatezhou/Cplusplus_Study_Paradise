#include "storeCore.h"
#include "../guidMaker/GuidProducer.h"
#include "../tools/tools.h"
#include "../tools/Lock.h"
#include "../3p/json/json.h"


CDB* CStoreCore::Getdb(const tstring& strdbName)
{
	AutoLock lock(mapdbMutex);
	auto iter = m_mapdb.find(strdbName);
	if (iter != m_mapdb.end()){
		return iter->second;
	}
	else{
		return nullptr;
	}
}

CDB* CStoreCore::Newdb(const tstring& strdbName)
{
	AutoLock lock(mapdbMutex);
	CDB* pdb = Getdb(strdbName);
	if (pdb == nullptr){
		pdb = new CDB(strdbName);
		m_mapdb[strdbName] = pdb;
	}
	return pdb;
}

void CStoreCore::MakedbFile(const tstring& strdbName)
{
	AutoLock lock(mapdbMutex);
	tstring* strId = CGuidProducer::GetInstance()->GetId();
	m_mapdbFiles.insert(std::make_pair(strdbName, *strId));

	tstring strDir = CTools::GetProcessDir() + _W("\\db\\");
	CTools::CreateDir(strDir);
	tstring strdbPath;
	strdbPath += strDir + *strId + _W(".db");

	tofstream file(strdbPath);
	file.close();

	//write into mapping files
	WriteIntoMappingFile(strdbName, *strId);
}

void CStoreCore::WriteIntoMappingFile(const tstring& strdbName, const tstring& strGuid)
{
	tstring strMappingFile = CTools::GetProcessDir() + _W("/dbMapping.file");
	tifstream file(strMappingFile);
	tstring strJson;
	if (file.is_open() == false){
		tofstream oFile(strMappingFile);
		oFile.close();
	}
	else{

	}
}
