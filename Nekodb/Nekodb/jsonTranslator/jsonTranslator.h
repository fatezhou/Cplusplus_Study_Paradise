#ifndef __JSON_TRANSLATOR_H__
#define __JSON_TRANSLATOR_H__

#include "../include/global.h"
#include "../3p/json/json.h"

#include "../core/db.h"

class CJsonTranslator{
public:
	CJsonTranslator(std::string strJson, CDB* pdb, const tstring* pStrId);
	void Translate();

protected:
	void GetKeys(tstring strParentKey, Json::Value& jValue);
	void PushTodb(tstring strKey, Json::Value& jValue);
	CDB* Getdb();
private:
	std::string m_strJson;
	std::vector<tstring> m_vecKeys;
	const tstring* m_pStrId;
	CDB* m_pdb;
};

#endif