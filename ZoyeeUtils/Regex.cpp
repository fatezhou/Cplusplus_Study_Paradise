#include "Regex.h"
#include <regex>
using namespace std;
using namespace ZoyeeUtils;

CRegex::CRegex(void)
{
}


CRegex::~CRegex(void)
{
}

CRegex::CRegex(const char* pSrc, const char* pRegex) : m_strSrc(pSrc), m_strRegex(pRegex)
{

}


std::vector<std::string>& CRegex::GetSearch()
{
	m_vec.clear();
	std::regex e(m_strRegex.c_str());
	for (std::sregex_token_iterator iter(m_strSrc.begin(), m_strSrc.end(), e), iterEnd; iter != iterEnd; iter++){
		m_vec.push_back(iter->str());
	}
	return m_vec;
}

std::vector<std::string>& CRegex::GetSearch( char* pSrc, char* pRegex )
{
	static std::vector<std::string> vec;
	std::regex e(pRegex);
	std::string strSrc(pSrc);

	for (std::sregex_token_iterator iter(strSrc.begin(), strSrc.end(), e), iterEnd; iter != iterEnd; iter++){
		vec.push_back(iter->str());
	}
	return vec;
}