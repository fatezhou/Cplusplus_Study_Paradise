#ifndef __VALUE_BLOCK_H__
#define __VALUE_BLOCK_H__

#include "../include/global.h"

class CValueBlock{
public:
	CValueBlock();
	CValueBlock(const int nValue, const tstring& valueId);
	CValueBlock(const tstring& strValue, const tstring& valueId);
	CValueBlock(const bool bValue, const tstring& valueId);
	emDataType GetType();

	void SetStr(const tstring& strValue);
	void SetInt(const int nValue);
	void SetBool(const bool bValue);
	void SetValueId(const tstring& valueId);
	
	tstring GetStr();
	int GetInt();
	bool GetBool();
	tstring GetValueId();

private:	
	tstring m_valueId;
	emDataType m_type;
	tstring m_str;
	int m_n;
	bool m_b;
};

#endif