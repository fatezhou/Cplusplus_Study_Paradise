// Test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <mutex>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
using namespace std;

void Func(){	
	ofstream file("d:/1.txt");
	file << "Hello" << 1234;
}

class Base{
public:
	Base(){
		file.open("d:/123.txt");
	}
	ofstream& operator<<(const std::string& str){
		this->file << str;
		return this->file;
	};

	ofstream file;
};

#include <filesystem>
#include <list>
#include <algorithm>
#include <fstream>

int _tmain(int argc, _TCHAR* argv[])
{
	//std::ifstream file("d:/index.js");
	//std::string strLine;
	//strLine.resize(1024, 0);
	//file.read((char*)strLine.data(), 1000);

	//std::ofstream file("d:/123.txt", std::ios::app);
	//file.write("Hello", 5);

	//std::ifstream file("d:/123.txt");
	//std::string str;
	//while (!file.eof()){
	//	std::string strt;
	//	strt.resize(2, 0);
	//	file.read((char*)strt.data(), 2);
	//	str += strt;
	//}

	//char sz[1024] = "Hello";
	//std::string str;
	//str += sz;
	{
		std::wstring wstr = L"你好";
		std::wofstream log(L"d:/1122.txt", ios::out | ios::app);
		log.imbue(std::locale("chs"));
		log << wstr.c_str();
		log.write(wstr.c_str(), wstr.length());
		log.flush();
		log.close();
	}


	std::string str = "再见";
	std::ofstream loga("d:/1122.txt", ios::app);
	loga.write(str.c_str(), str.length());
	loga.close();
	return 0;
}

