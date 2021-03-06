#pragma once

#include "StdAfx.h"
										
typedef struct __tagSolaMultiValue { const int nValue; const TCHAR* szString; } SOLAMULTIVALUE, *LPSOLAMULTIVALUE;

class CSolaMultiValue
{
public:
	CSolaMultiValue(void);
	CSolaMultiValue(LPSOLAMULTIVALUE p, int nSize) : m_lpSolaMultiValue(p), m_cbSize(nSize){}
	LPSOLAMULTIVALUE GetLPMulti();
	const TCHAR* GetMultiString(int index);
	const TCHAR* GetMultiString(unsigned short usValue);
	inline int GetSize() {return m_cbSize;};
public:
	~CSolaMultiValue(void);
private:
	LPSOLAMULTIVALUE m_lpSolaMultiValue;
	int m_cbSize;
};
