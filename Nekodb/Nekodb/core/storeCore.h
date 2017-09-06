#ifndef __STORE_CORE_H__
#define __STORE_CORE_H__


#include "db.h"

class CStoreCore{
public:
	CStoreCore();
	~CStoreCore();

	CDB* Getdb(const tstring& strdbName);
	CDB* Newdb(const tstring& strdbName);	

	void MakedbFile(const tstring& strdbName);
private:
	void WriteIntoMappingFile(const tstring& strdbName, const tstring& strGuid);

	std::map<tstring, CDB*> m_mapdb;
	std::mutex mapdbMutex;
	std::map<tstring, tstring> m_mapdbFiles;

};

#endif