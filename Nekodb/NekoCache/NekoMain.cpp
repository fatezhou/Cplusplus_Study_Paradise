#include "../common/NekoLog.h"
#include "core/Cached.h"

void _TextOut(const std::string& strText){
	printf("---[%s]\n", strText.c_str());
}
#pragma comment(lib, "ole32")
std::string MakeGuid(){
	char szGUID[64];
#ifdef WIN32
	GUID uuid;
	CoCreateGuid(&uuid);
#else
	TmUUID_t    uuid;
	TmMsg_CreateUUID(&uuid);
#endif
	sprintf(szGUID, "%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X",
		uuid.Data1, uuid.Data2, uuid.Data3, uuid.Data4[0], uuid.Data4[1], uuid.Data4[2],
		uuid.Data4[3], uuid.Data4[4], uuid.Data4[5], uuid.Data4[6], uuid.Data4[7]);
	return szGUID;
}

int main(int n, char** v){
	//Neko::CLog::GetInstance()->SetWriteFunction(_TextOut);
	//Neko::CLog::GetInstance()->Debug(__FUNCTION__, __LINE__, "Hello, %s", "World");
	//Neko::CLog::GetInstance()->SetAsync(true);
	//Neko::CLog::GetInstance()->Debug(__FUNCTION__, __LINE__, "Hello, %s", "World");
	//Neko::CLog::GetInstance()->Debug(__FUNCTION__, __LINE__, "Hello, %s", "World");
	//Neko::CLog::GetInstance()->Debug(__FUNCTION__, __LINE__, "Hello, %s", "World");
	//Neko::CLog::GetInstance()->SetAsync(false);
	//Neko::CLog::GetInstance()->Debug(__FUNCTION__, __LINE__, "Hello, %s", "World2");
	//WaitForSingleObject(CreateEventA(NULL, true, false, NULL), INFINITE);
	srand(time(0));

	Neko::CCached cached;
	const int nSize = 50000;
	cached.SetCachedSize(20000);
	int nNow = GetTickCount();	
	printf("[init][%d]\n", GetTickCount() - nNow);
	nNow = GetTickCount();

	std::string* pStrGuid = new std::string[nSize];	
	printf("[after new][%d]\n", GetTickCount() - nNow);
	nNow = GetTickCount();

	for (int i = 0; i < nSize; i++){
		pStrGuid[i] = MakeGuid();
	}

	printf("[after make guids][%d]\n", GetTickCount() - nNow);
	nNow = GetTickCount();

	for (int i = 0; i < nSize; i++){
		if (i % 500 == 0){
			printf("[sets:%d][%d]\n", i, GetTickCount() - nNow);
			nNow = GetTickCount();
		}
		cached.Set(pStrGuid[i], pStrGuid[i], rand());
	}

	printf("[after sets][%d]\n", GetTickCount() - nNow);
	nNow = GetTickCount();
	printf("[ready to find][%d]\n", GetTickCount());
	int nGetPos[] = { 10000, 15000, 25000, 35000, 45000, 49990 };
	for (int i = 0; i < 6; i++){
		std::string strGet = cached.Get(pStrGuid[nGetPos[i]]);
		printf("[gets][%d]\n", GetTickCount() - nNow);
		nNow = GetTickCount();
		printf("strGet:%s\n", strGet.c_str());
	}
	printf("[finish find][%d]\n", GetTickCount());
	printf("[finish][%d]\n", GetTickCount() - nNow);
	nNow = GetTickCount();
	WaitForSingleObject(CreateEventA(NULL, true, false, NULL), INFINITE);
}