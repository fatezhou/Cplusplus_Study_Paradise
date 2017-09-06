#include "../common/NekoLog.h"
#include "core/Cached.h"

void _TextOut(const std::string& strText){
	printf("---[%s]\n", strText.c_str());
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
	Neko::CCached cached;
	cached.SetCachedSize(5);

	for (int i = 0; i < 10; i++){
		char sz[16];
		sprintf(sz, "%02d", i);
		cached.Set(sz, sz);
	}
}