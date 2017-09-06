#include "../common/NekoLog.h"

void _TextOut(const std::string& strText){
	printf("---[%s]\n", strText.c_str());
}

int main(int n, char** v){
	Neko::CLog::GetInstance()->SetWriteFunction(_TextOut);
	Neko::CLog::GetInstance()->Debug(__FUNCTION__, __LINE__, "Hello, %s", "World");
	Neko::CLog::GetInstance()->SetAsync(true);
	Neko::CLog::GetInstance()->Debug(__FUNCTION__, __LINE__, "Hello, %s", "World");
	Neko::CLog::GetInstance()->Debug(__FUNCTION__, __LINE__, "Hello, %s", "World");
	Neko::CLog::GetInstance()->Debug(__FUNCTION__, __LINE__, "Hello, %s", "World");
	Neko::CLog::GetInstance()->SetAsync(false);
	Neko::CLog::GetInstance()->Debug(__FUNCTION__, __LINE__, "Hello, %s", "World2");
	WaitForSingleObject(CreateEventA(NULL, true, false, NULL), INFINITE);
}