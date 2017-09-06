#include "valueBlock.h"

CValueBlock::CValueBlock(const int nValue, const tstring& valueId) : m_valueId(valueId), m_type(emDataType::_INT)
{
	m_n = nValue;
}

CValueBlock::CValueBlock(const tstring& strValue, const tstring& valueId) : m_valueId(valueId), m_type(emDataType::_STR)
{
	m_str = strValue;
}

CValueBlock::CValueBlock(const bool bValue, const tstring& valueId) : m_valueId(valueId), m_type(emDataType::_BOOL)
{
	m_b = bValue;
}

CValueBlock::CValueBlock()
{

}

void CValueBlock::SetStr(const tstring& strValue)
{
	m_str = strValue;
	m_type = emDataType::_STR;
}

void CValueBlock::SetInt(const int nValue)
{
	m_n = nValue;
	m_type = emDataType::_INT;
}

void CValueBlock::SetBool(const bool bValue)
{
	m_b = bValue;
	m_type = emDataType::_BOOL;
}

void CValueBlock::SetValueId(const tstring& valueId)
{
	m_valueId = valueId;
}

tstring CValueBlock::GetStr()
{
	return m_str;
}

int CValueBlock::GetInt()
{
	return m_n;
}

bool CValueBlock::GetBool()
{
	return m_b;
}

tstring CValueBlock::GetValueId()
{
	return m_valueId;
}

emDataType CValueBlock::GetType()
{
	return m_type;
}
