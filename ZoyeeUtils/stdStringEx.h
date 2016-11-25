#ifndef __STD_STRING_EX_H__
#define __STD_STRING_EX_H__
//Create by Ã¨²è
#include <string>
#include <list>

namespace ZoyeeUtils{
	class CStdStringEx : public std::string{
	public:
		CStdStringEx(int nNumber);
		CStdStringEx();
		CStdStringEx(std::string& str);
		CStdStringEx(char* pText);
		CStdStringEx(CStdStringEx& str);
		CStdStringEx(const char* pFmt, ...);
		std::list<std::string> Split(std::string strSep = ",");
		std::string Replace(std::string src, std::string desc);
		static std::string Replace(std::string strText, std::string src, std::string desc);
		int ToInt();
		float toFloat();
	};
}
#endif