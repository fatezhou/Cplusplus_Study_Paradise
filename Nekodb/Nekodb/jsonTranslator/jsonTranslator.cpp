#include "jsonTranslator.h"
#include "../tools/tools.h"

CJsonTranslator::CJsonTranslator(std::string strJson, CDB* pdb, const tstring* pStrId) : m_strJson(strJson)
, m_pdb(pdb), m_pStrId(pStrId)
{
	pdb->GetDataBlock()->PushPureData(*pStrId, CTools::ToTString(strJson));
}

void CJsonTranslator::Translate()
{
	Json::Reader reader;
	Json::Value jRoot;
	if (reader.parse(m_strJson, jRoot) == false){
		return;
	}

	Json::Value::Members vecMemberKeys = jRoot.getMemberNames();
	int nSize = vecMemberKeys.size();
	for (int i = 0; i < nSize; i++){
		if (jRoot[vecMemberKeys[i]].isObject() == false){
			PushTodb(CTools::ToTString(vecMemberKeys[i]), jRoot[vecMemberKeys[i]]);
			m_vecKeys.push_back(CTools::ToTString(vecMemberKeys[i]));
		}
		else{
			GetKeys(CTools::ToTString(vecMemberKeys[i]), jRoot[vecMemberKeys[i]]);
		}
	}
}

void CJsonTranslator::GetKeys(tstring strParentKey, Json::Value& jValue)
{
	Json::Value::Members vecMemberKeys = jValue.getMemberNames();
	int nSize = vecMemberKeys.size();
	for (int i = 0; i < nSize; i++){
		if (jValue[vecMemberKeys[i]].isObject() == false){
			tstring strKeyDescribe = (strParentKey + _W(".") + CTools::ToTString(vecMemberKeys[i]));			
			m_vecKeys.push_back(strKeyDescribe);
			PushTodb(strKeyDescribe, jValue[vecMemberKeys[i]]);
		}
		else{
			tstring strKeys = strParentKey + _W(".") + CTools::ToTString(vecMemberKeys[i]);
			
			GetKeys(strKeys, jValue[vecMemberKeys[i]]);
		}
	}
}

void CJsonTranslator::PushTodb(tstring strKey, Json::Value& jValue)
{
	switch (jValue.type()){
		case Json::ValueType::intValue:{			
			Getdb()->GetDataBlock()->PushInt(strKey, jValue.asInt(), *m_pStrId);
			break;
		}
		case Json::ValueType::stringValue:{
			Getdb()->GetDataBlock()->PushStr(strKey, CTools::ToTString(jValue.asString()), *m_pStrId);
			break;
		}
		case Json::ValueType::booleanValue:{
			Getdb()->GetDataBlock()->PushBool(strKey, jValue.asBool(), *m_pStrId);
			break;
		}
		default:{
			break;
		}			
	}
}

CDB* CJsonTranslator::Getdb()
{
	return m_pdb;
}
