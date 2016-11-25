#include "RegEdit.h"
using namespace std;
using namespace ZoyeeUtils;

#include <sstream>

ZoyeeUtils::CRegister::CRegister( KeyRoot rootType, std::string strKey, bool bIsX64 ) : m_strKey(strKey), m_keyType(rootType), m_bIsX64(bIsX64)
{
	HKEY hkMain;
	switch(rootType){
	case em_classes_root:
		hkMain = HKEY_CLASSES_ROOT;
		break;
	case em_local_machine:
		hkMain = HKEY_LOCAL_MACHINE;
		break;
	case em_current_user:
		hkMain = HKEY_CURRENT_USER;
		break;
	case em_user:
		hkMain = HKEY_USERS;
		break;
	case em_current_config:
		hkMain = HKEY_CURRENT_CONFIG;
		break;
	default:
		break;
	}

	RegOpenKeyExA(hkMain, strKey.c_str(), 0, KEY_ALL_ACCESS | (bIsX64 ? KEY_WOW64_64KEY : 0), &m_hRoot);
}

std::list<std::string> ZoyeeUtils::CRegister::EnumSubKey()
{	
	std::list<std::string> lstSubKey;
	char szKeys[128];
	int nIndex = 0;
	while(ERROR_SUCCESS == RegEnumKeyA(m_hRoot, nIndex++, szKeys, 128)){
		lstSubKey.push_back(szKeys);
	}	
	return lstSubKey;
}

std::string ZoyeeUtils::CRegister::GetValue( std::string strKey, bool bIsStr /*= true*/ )
{
	DWORD dwType = bIsStr ? REG_SZ : REG_DWORD;
	DWORD dwSize;
	char sz[1024] = {0};
	if (bIsStr){
		dwSize = sizeof(sz);
		if(RegQueryValueExA(m_hRoot, strKey.c_str(), 0, &dwType, (LPBYTE)&sz, &dwSize) == ERROR_SUCCESS){
			return sz;
		}
	}else{
		DWORD dw = 0;
		dwSize = sizeof(dw);
		if (RegQueryValueExA(m_hRoot, strKey.c_str(), 0, &dwType, (LPBYTE)&dw, &dwSize) == ERROR_SUCCESS){
			sprintf(sz, "%d", dw);
			return sz;
		}
	}
	return "";
}

int ZoyeeUtils::CRegister::SetValue( std::string strKey, std::string strValue, bool bIsStr /*= true*/ )
{
	DWORD dwType = bIsStr ? REG_SZ : REG_DWORD;
	DWORD dwValue, dwSize;
	char szValue[1024] = {0};
	DWORD* pData = nullptr;

	if (bIsStr){
		strcpy(szValue, strValue.c_str());
		pData = (DWORD*)&szValue;
		dwSize = strlen(szValue);
	}else{
		dwValue = atoi(strValue.c_str());
		dwSize = sizeof(dwValue);
		pData = &dwValue;
	}
	return RegSetValueExA(m_hRoot, strKey.c_str(), 0, dwType, (const BYTE *)pData, dwSize);
}

int ZoyeeUtils::CRegister::CreateKey( std::string strKey )
{
	HKEY hRes;
	DWORD dwRes;
	return RegCreateKeyExA(m_hRoot, strKey.c_str(), 0, 0, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hRes, &dwRes);
}

int ZoyeeUtils::CRegister::DeleteKey( std::string strKey )
{
	deleteKey(strKey, m_hRoot);
	return 0;
}

ZoyeeUtils::CRegister::~CRegister()
{
	RegCloseKey(m_hRoot);
}

int ZoyeeUtils::CRegister::deleteKey(std::string strKey, HKEY hFatherKey )
{
	char szKey[256] = "";
	int nIndex = 0;
	HKEY hSubKey;
	RegOpenKeyExA(hFatherKey, strKey.c_str(), 0, KEY_ALL_ACCESS | (m_bIsX64 ? KEY_WOW64_64KEY : 0), &hSubKey);
	if (hSubKey){
		while(RegEnumKeyA(hSubKey, nIndex, szKey, 256) == ERROR_SUCCESS){
			deleteKey(szKey, hSubKey);
		}
		RegDeleteKeyA(hFatherKey, strKey.c_str());
	}	
	return 0;
}

std::list<std::pair<std::string, ZoyeeUtils::CRegister::ValueType>> ZoyeeUtils::CRegister::EnumValue()
{
	std::list<std::pair<std::string, ZoyeeUtils::CRegister::ValueType>> lst;
	//todo 
	return lst;
}
