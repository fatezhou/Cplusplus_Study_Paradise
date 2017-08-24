#ifndef __I_FILE_CONFIG_H__
#define __I_FILE_CONFIG_H__



#include <string>
#include <list>
#include <vector>

namespace ZoyeeUtils{
	class IFileConfig;

	class CFileConfigFactory{
	public:
		enum ConfigType{
			em_Json,
			em_Ini,
			em_Xml
		};
		static IFileConfig* CreateConfigParser(ConfigType type);
		static void ReleaseConfingParser(IFileConfig* pConfigObject);
	};

	class IFileConfig{
	public:
		IFileConfig(){};
		void SetConfigPath(const char* pPath){};
		virtual int LoadConfigFile(const char* pConfigPath){return 0;};

		class Node{
		public:
			Node();
			void* p;
		};

		class VALUE{
		public:
			VALUE(int nValue){};
			VALUE(std::string strValue){};
			VALUE(bool bValue){};
			
			bool HasAttr();
			bool IsObject();
			bool IsArray();
			bool IsValue();

			bool ToBoolen();
			std::string ToString();
			int ToInt();			

			std::string strText;
			std::string strType;
			bool bHasAttr;
			void* pObject;
		};

		
		virtual std::list<std::string> GetKeys(const char* pParent){std::list<std::string> _list;return _list;};
		virtual std::list<std::string> GetKeys(VALUE& value){std::list<std::string> _list;return _list;};
		virtual VALUE GetValue(const char* pKey, int nDefaultValue, const char* pConfigPath = nullptr){VALUE value(0); return value;};
		virtual VALUE GetValue(const char* pKey, std::string strDefaultValue, const char* pConfigPath = nullptr){VALUE value(0); return value;};
		virtual VALUE GetValue(const char* pKey, bool bDefaultValue, const char* pConfigPath = nullptr){VALUE value(0); return value;};
	};
}

#endif