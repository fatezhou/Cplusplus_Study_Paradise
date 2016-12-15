#include "../ApiHook/ApiHook.h"
#include <iostream>
//typedef void(*pTest)();
typedef void(*pfn_AddHook)(char* p);
//#include <tuple>

void Func(char* ptr){
	printf("%p", ptr);
}

int main(){
	HMODULE hModule = LoadLibraryA("../output/debug/apihook.dll");	/*
	//auto t = std::tr1::make_tuple(1, 2,3, "Hello");
	//int x = std::tr1::get<1>(t);*/
	//
	pfn_AddHook _addFunc = (pfn_AddHook)GetProcAddress(hModule, "AddHook");
	_addFunc("CreateFileA");
	CreateFileA("", 0, 0, 0, 0, 0, 0);
	char* ptr = new char[1];
	printf("%p\n", ptr);
	Func(ptr);
	return 0;
}