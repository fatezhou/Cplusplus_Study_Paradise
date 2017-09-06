#ifndef __GLOBAL_H__
#define __GLOBAL_H__
#if _MSC_VER < 1700
#ERROR "no supper msvc 2012 below"
#else

#include <vector>
#include <list>
#include <string>
#include <map>
#include <algorithm>

#include <cstdio>
#include <iostream>
#include <fstream>
#include <mutex>
#include <thread>

#include <tchar.h>

#ifndef _UNICODE
#define tstring std::string
#define tchar char
#define tofstream std::ofstream
#define tifstream std::ifstream
#define tfstream std::fstream
#define _W(x) x
#else
#define tstring std::wstring
#define tchar wchar_t
#define tofstream std::wofstream
#define tifstream std::wifstream
#define tfstream std::wfstream
#define _W(x) L##x
#endif

#ifdef WIN32
#pragma comment(lib, "ole32")
#else
#endif

enum emDataType{
	_STR,
	_INT,
	_BOOL,
	_ARRAY
};

typedef std::vector<tstring> VecTString;
typedef std::map<int, VecTString> mapINT;
typedef std::map<tstring, VecTString> mapSTR;
typedef std::map<bool, VecTString> mapBOOL;

typedef std::map<tstring, mapINT> mapTStringMapInt;
typedef std::map<tstring, mapSTR> mapTStringMapSTR;
typedef std::map<tstring, mapBOOL> mapTStringMapBOOL;

#endif
#endif