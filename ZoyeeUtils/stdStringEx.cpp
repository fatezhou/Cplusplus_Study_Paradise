#include "stdStringEx.h"
#include <stdarg.h>
#include <Windows.h>
using namespace ZoyeeUtils;


ZoyeeUtils::string::string( int nNumber ) : std::string()
{
	resize(16);
	sprintf((char*)data(), "%d", nNumber);
}

std::vector<std::string> ZoyeeUtils::string::Split(std::string strSep /*= ","*/)
{
	std::vector<std::string> vec;
	string strTemp = data();
	int nSepLen = strlen(strSep.c_str());
	int nPos = strTemp.find(strSep);
	while(nPos != -1){
		vec.push_back(strTemp.substr(0, nPos));
		strTemp.erase(0, nPos + nSepLen);
		nPos = strTemp.find(strSep);
	}
	if ( ! strTemp.empty()){
		vec.push_back(strTemp);
	}
	return vec;
}

std::vector<std::string> ZoyeeUtils::string::Split(string str, std::string strSep /*= ","*/)
{
	return str.Split(strSep);
}

std::string ZoyeeUtils::string::Replace( std::string src, std::string desc )
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

std::string ZoyeeUtils::string::Replace( std::string strText, std::string src, std::string desc )
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

int ZoyeeUtils::string::ToInt()
{
	return atoi(data());
}

float ZoyeeUtils::string::toFloat()
{
	return atof(data());
}

std::string ZoyeeUtils::string::ToStdString()
{
	return *this;
}

std::string ZoyeeUtils::string::ToStdString(string& str)
{
	return str.ToStdString();
}

std::wstring ZoyeeUtils::string::ToStdWString()
{
	std::wstring wstr;
	int nLen = ::MultiByteToWideChar(CP_ACP, 0, (char*)data(), -1, NULL, NULL);
	if (nLen <= 0){
		return L"";
	}
	wstr.resize(nLen);
	MultiByteToWideChar(CP_ACP, 0, (char*)data(), -1, (wchar_t*)wstr.data(), nLen);
	return wstr;
}

std::wstring ZoyeeUtils::string::ToStdWString(string& str)
{
	return str.ToStdWString();
}

std::string ZoyeeUtils::string::Trim(std::string strWord)
{
	TrimLeft(strWord);
	TrimRight(strWord);
	return *this;
}

std::string ZoyeeUtils::string::TrimLeft(std::string strWord)
{
	int nFind = this->find(strWord);
	while (nFind == 0){
		*this = this->substr(strWord.length(), this->npos);
		nFind = this->find(strWord);
	}
	return *this;
}

std::string ZoyeeUtils::string::TrimRight(std::string strWord)
{
	int nFind = this->rfind(strWord);
	while (nFind == (this->length() - this->length())){
		*this = this->substr(0, nFind);
		nFind = this->rfind(strWord);
	}
	return *this;
}

string::string() : std::string(){

}

string::string(std::string& str) : std::string(str){

}

string::string(string& str) : std::string(){
	resize(str.size());
	memcpy((void*)data(), str.data(), str.size());
}

string::string(char* pText) : std::string(pText){

}

string::string(const char* pFmt, ...) : std::string(){
	va_list ap;
	va_start(ap, pFmt);
	int nPrintLen = vprintf(pFmt, ap);	
	resize(nPrintLen);
	vsprintf((char*)data(), pFmt, ap);
	va_end(ap);
}

string::string(std::wstring& wstr) : std::string(){
	std::string str;
	int nLen = ::WideCharToMultiByte(CP_ACP, 0, (wchar_t*)wstr.c_str(), -1, 0, 0, 0, 0);
	if (nLen > 0){
		resize(nLen);
		WideCharToMultiByte(CP_ACP, 0, (wchar_t*)wstr.c_str(), -1, (char*)data(), nLen, 0, 0);
	}
}