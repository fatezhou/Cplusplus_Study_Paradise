// Test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <mutex>
#include <thread>
#include <map>
#include <windows.h>

void func(){
	std::atexit([]{printf("exit"); });
}

int main(){
	{func(); }
	printf("ready exit");
	return 0;
}