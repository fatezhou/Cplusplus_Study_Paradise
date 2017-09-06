// Test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <mutex>
#include <thread>
#include <map>
#include <windows.h>

std::mutex m;
std::recursive_mutex rm;

void func(int id){
	while (1){
		std::lock_guard<std::recursive_mutex> lock(rm);
		Sleep(1000);
		printf("[%d]\n", id);
	}
}

void Func1(){
	std::lock_guard<std::recursive_mutex> lock(rm);
}

int main(){
	std::thread(func, 1).detach();
	std::thread(func, 2).detach();
	std::thread(func, 3).detach();

	std::lock_guard<std::recursive_mutex> lock(rm);
	Func1();
	rm.unlock();
	while (1){
		Sleep(1000);
	}
}