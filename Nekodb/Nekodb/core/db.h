#ifndef __DB_H__
#define __DB_H__

#include "dataBlock.h"

class CDB{
public:
	CDB(const tstring& strdbName);
	~CDB();

	tstring GetDatabaseName();
	CDataBlock* GetDataBlock();

	void SaveToHardDisc();
	void LoadFromHardDisc();
	void SetdbPath(const tstring& strPath);

private:
	CDataBlock m_dataBlock;
	const tstring m_strdbName;
	tstring m_strdbPath;
};

#endif