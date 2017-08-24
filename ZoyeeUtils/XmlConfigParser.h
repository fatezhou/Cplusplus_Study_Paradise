#ifndef __XML_CONFIG_PARSER_H__
#define __XML_CONFIG_PARSER_H__

#include "IFileConfig.h"

namespace ZoyeeUtils{
	class CXmlConfigParser : public IFileConfig{
	public:
		CXmlConfigParser();
		int LoadConfigFile(const char* pConfigPath);
		void* GetDoc();
		std::list<std::string> GetKeys( const char* pParent );


	private:
		std::vector<std::string> transToList(const char* pText, char cSplit = '/');
		void* xmlDoc;	
	};
}

#endif