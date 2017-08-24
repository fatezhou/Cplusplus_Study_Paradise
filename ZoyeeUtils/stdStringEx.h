#ifndef __STD_STRING_EX_H__
#define __STD_STRING_EX_H__
//Create by Ã¨²è
#include <string>
#include <list>
#include <vector>

namespace ZoyeeUtils{
	class string : public std::string{
	public:
		string(int nNumber);
		string();
		string(std::string& str);
		string(char* pText);
		string(string& str);
		string(const char* pFmt, ...);
		string(std::wstring& wstr);
		std::vector<std::string> Split(std::string strSep = ",");
		std::string Replace(std::string src, std::string desc);
		std::string Trim(std::string strWord);
		std::string TrimLeft(std::string strWord);
		std::string TrimRight(std::string strWord);
		int ToInt();
		float toFloat();
		std::string ToStdString();
		std::wstring ToStdWString();

		static std::string ToStdString(string& str);
		static std::wstring ToStdWString(string& str);
		static std::vector<std::string> Split(string str, std::string strSep = ",");
		static std::string Replace(std::string strText, std::string src, std::string desc);
	};
}
#endif