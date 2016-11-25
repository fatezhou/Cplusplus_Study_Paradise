#include "stdStringEx.h"
using namespace ZoyeeUtils;
using namespace std;

#include <stdarg.h>

ZoyeeUtils::CStdStringEx::CStdStringEx( int nNumber ) : std::string()
{
	resize(16);
	sprintf((char*)data(), "%d", nNumber);
}

std::list<std::string> ZoyeeUtils::CStdStringEx::Split( std::string strSep /*= ","*/ )
{
	list<string> lst;
	string strTemp = data();
	int nSepLen = strlen(strSep.c_str());
	int nPos = strTemp.find(strSep);
	while(nPos != -1){
		lst.push_back(strTemp.substr(0, nPos));
		strTemp.erase(0, nPos + nSepLen);
		nPos = strTemp.find(strSep);
	}
	if ( ! strTemp.empty()){
		lst.push_back(strTemp);
	}
	return lst;
}

std::string ZoyeeUtils::CStdStringEx::Replace( std::string src, std::string desc )
{
	int nPos = find(src);
	while(nPos != -1){
		string strHead = substr(0, nPos);
		strHead += desc;
		erase(0, nPos + strlen(src.c_str()));
		insert(0, strHead);
		nPos = find(src);
	}
	return *this;
}

std::string ZoyeeUtils::CStdStringEx::Replace( std::string strText, std::string src, std::string desc )
{
	int nPos = strText.find(src);
	while(nPos != -1){
		string strHead = strText.substr(0, nPos);
		strHead += desc;
		strText.erase(0, nPos + strlen(src.c_str()));
		strText.insert(0, strHead);
		nPos = strText.find(src);
	}
	return strText;
}

int ZoyeeUtils::CStdStringEx::ToInt()
{
	return atoi(data());
}

float ZoyeeUtils::CStdStringEx::toFloat()
{
	return atof(data());
}

CStdStringEx::CStdStringEx() : std::string(){

}

CStdStringEx::CStdStringEx(std::string& str) : std::string(str){

}

CStdStringEx::CStdStringEx(CStdStringEx& str) : std::string(){
	resize(str.size());
	memcpy((void*)data(), str.data(), str.size());
}

CStdStringEx::CStdStringEx(char* pText) : std::string(pText){

}

CStdStringEx::CStdStringEx(const char* pFmt, ...){
	va_list ap;
	va_start(ap, pFmt);
	int nPrintLen = vprintf(pFmt, ap);
	resize(nPrintLen);
	vsprintf((char*)data(), pFmt, ap);
	va_end(ap);
}