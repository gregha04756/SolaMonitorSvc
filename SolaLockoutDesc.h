#pragma once

typedef struct __tagLockoutDesc { const int nLockoutCode; const TCHAR* szLockoutText; } SOLALOCKOUTDESC, *LPSOLALOCKOUTDESC;

class CSolaLockoutDesc
{
public:
	CSolaLockoutDesc(void);
	CSolaLockoutDesc(const LPSOLALOCKOUTDESC lpSLD, int nSize);
	~CSolaLockoutDesc(void);
	inline int GetSize(){return m_nSize;};
	const TCHAR* GetLockoutDesc(int ndx);
private:
	LPSOLALOCKOUTDESC m_lpSLD;
	int m_nSize;
};
