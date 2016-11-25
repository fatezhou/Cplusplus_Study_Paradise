#ifndef __REG_EDIT_H__
#define __REG_EDIT_H__

#include <string>
#include <list>
#include <Windows.h>

namespace ZoyeeUtils{
	class CRegister{
	public:
		enum KeyRoot{
			em_classes_root,
			em_current_user,
			em_local_machine,
			em_user,
			em_current_config
		};
		enum ValueType{
			em_reg_sz,
			em_reg_dword
		};
		CRegister(KeyRoot rootType, std::string strKey, bool bIsX64 = true);
		~CRegister();
		std::list<std::string> EnumSubKey();
		std::list<std::pair<std::string, ValueType>> EnumValue();
		std::string GetValue(std::string strKey, bool bIsStr = true);
		int SetValue(std::string strKey, std::string strValue, bool bIsStr = true);
		int CreateKey(std::string strKey);
		int DeleteKey(std::string strKey);

	private:
		int deleteKey(std::string strKey, HKEY hFatherKey);
		std::string m_strKey;
		KeyRoot m_keyType;
		HKEY m_hRoot;
		bool m_bIsX64;
	};
}


#endif