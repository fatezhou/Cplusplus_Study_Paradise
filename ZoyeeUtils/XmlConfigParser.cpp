#include "XmlConfigParser.h"
#include "3p/tinyXml2/tinyxml2.h"

static tinyxml2::XMLDocument* GetXmlDoc(ZoyeeUtils::CXmlConfigParser* ptr){
	return (tinyxml2::XMLDocument*)ptr->GetDoc();
}

using namespace tinyxml2;

#define DOC GetXmlDoc(this)

int ZoyeeUtils::CXmlConfigParser::LoadConfigFile( const char* pConfigPath )
{
	if (this->xmlDoc == nullptr){
		this->xmlDoc = new tinyxml2::XMLDocument;
	}

	if (DOC->LoadFile(pConfigPath) != tinyxml2::XML_SUCCESS){
		return -1;
	}

	return 0;
}

void* ZoyeeUtils::CXmlConfigParser::GetDoc()
{
	return xmlDoc;
}

std::list<std::string> ZoyeeUtils::CXmlConfigParser::GetKeys( const char* pParent )
{
	//   root/animal/cat	
	XMLElement* pRoot = DOC->RootElement();
	const char* pRootKey = pRoot->Value();
	std::list<std::string> listKeys;
	if (pParent == nullptr){
		listKeys.push_back(pRootKey);		
	}else{
		std::vector<std::string> vecPaths = transToList(pParent);
		int nSize = vecPaths.size();
		if (nSize >= 2){
			XMLElement* pElement = pRoot->FirstChildElement(vecPaths[0].c_str());
			if(pElement){			
				XMLElement* pSubElement = pElement;
				XMLElement* pParentElement = pElement;
				for (int i = 1; i < nSize; i++){
					for (XMLElement *p; p != nullptr; p = p->NextSiblingElement(vecPaths[i].c_str())){
						pSubElement = p->FirstChildElement();
					}
				}
			}
		}
	}
	
	return listKeys;
}

ZoyeeUtils::CXmlConfigParser::CXmlConfigParser() : IFileConfig(), xmlDoc(nullptr)
{
	
}

std::vector<std::string> ZoyeeUtils::CXmlConfigParser::transToList( const char* pText, char cSplit /*= '/'*/ )
{
	std::vector<std::string> vec;
	if (pText == nullptr){
		return vec;
	}

	int nLen = strlen(pText);
	char* pSrc = new char[nLen + 1];
	memset(pSrc, 0, nLen + 1);
	sprintf(pSrc, "%s", pText);
	char* pCurrent = pSrc;
	for (int i = 0; i < nLen; i++){
		if (pSrc[i] == cSplit){
			pSrc[i] = 0;
			vec.push_back(pCurrent);
			pCurrent = &pSrc[i + 1];
		}
	}
	if (pCurrent[0] != 0){
		vec.push_back(pCurrent);
	}
	return vec;
}
