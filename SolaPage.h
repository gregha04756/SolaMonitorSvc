#pragma once

#include "stdafx.h"
#include "SolaMBMap.h"

typedef struct __tagSolaPage {
const TCHAR* szLabel;
const int nIndex;
CSolaMBMap* pcSolaMBMap;
}SOLAPAGE, *LPSOLAPAGE;

using namespace std;

class CSolaPage
{
public:
	CSolaPage(void);
	CSolaPage(LPSOLAPAGE const p, int nSize) : m_lpSolaPage(p), m_nSize(nSize){}
	const TCHAR* ItemLabel(int nIndex);
	const int ItemIndex(int nIndex);
	CSolaMBMap* ItemMap(int nIndex);
	inline int GetSize(){return m_nSize;};
	~CSolaPage(void);
private:
	LPSOLAPAGE m_lpSolaPage;
	int m_nSize;
};
