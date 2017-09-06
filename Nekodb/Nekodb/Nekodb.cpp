// Nekodb.cpp : 定义控制台应用程序的入口点。
//
#include "tools/logger.h"
#include "jsonTranslator/jsonTranslator.h"
#include "tools/tools.h"

int main(){
	//for (int i = 0; i < 10; i++){
	//	tstring strGuid = CTools::GuidMaker();
	//	std::wcout << strGuid << std::endl;
	//}



	std::string strJson = R"({"key1":[], "key2":1,"key3":"value3","key4":{"subKey1":41, "subKey2":"key4.subKey2.value2"}})";
	CDB* pdb = new CDB(_W("test"));
	tstring strId = _W("aabb");
	CJsonTranslator j(strJson, pdb, &strId);
	j.Translate();
	return 0;
}