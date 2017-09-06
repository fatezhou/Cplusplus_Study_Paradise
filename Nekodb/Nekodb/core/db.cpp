#include "db.h"

CDB::CDB(const tstring& strdbName) : m_strdbName(strdbName)
{

}

tstring CDB::GetDatabaseName()
{
	return this->m_strdbName;
}

CDataBlock* CDB::GetDataBlock()
{
	return &this->m_dataBlock;
}

void CDB::SetdbPath(const tstring& strPath)
{
	this->m_strdbPath = strPath;
}
