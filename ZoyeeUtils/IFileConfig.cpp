#include "IFileConfig.h"
#include "XmlConfigParser.h"

using namespace ZoyeeUtils;

IFileConfig* ZoyeeUtils::CFileConfigFactory::CreateConfigParser( ConfigType type )
{
	switch(type){
	case em_Json:
	case em_Ini:
	case em_Xml:
		return new CXmlConfigParser;
	default:
		return nullptr;
	}
	return nullptr;
}
