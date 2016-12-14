#ifndef __REGEX_H__
#define __REGEX_H__

#include <string>
#include <vector>

namespace ZoyeeUtils{
	class CRegexStringMaker{
	public:
		std::string ToString();

	private:
		std::vector<std::string> m_vec;
	};

	class CRegex
	{
	public:
		CRegex(void);
		CRegex(const char* pSrc, const char* pRegex);
		~CRegex(void);
		std::vector<std::string>& GetSearch();
		static std::vector<std::string>& GetSearch(char* pSrc, char* pRegex);

	private:
		std::vector<std::string> m_vec;
		std::string m_strSrc;
		std::string m_strRegex;
	};
}


#endif