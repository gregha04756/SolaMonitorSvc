#pragma once

#include "stdafx.h"

typedef struct __tagSolaAlertRecord {	unsigned short ucAlertCode;
										unsigned long ulCycle;
										unsigned long ulHours;
										unsigned char uchUnused;
										unsigned char uchCount; } SOLAALERTRECORD, *LPSOLAALERTRECORD;

typedef union __tagSolaAlertUnion {		unsigned char cc[12];
										SOLAALERTRECORD aa; } SOLAALERTUNION, *LPSOLAALERTUNION;

typedef struct __tagSolaAlert {	unsigned char uchDevAddr;
								const unsigned char uchFuncCode;
								const unsigned short usStartRegAddr;
								const unsigned short usRegCount;
								LPSOLAALERTUNION pAlertRecord; } SOLAALERT, *LPSOLAALERT;

typedef struct __tagAlertDesc { const int nAlertCode; const TCHAR* szAlertText; } SOLAALERTDESC, *LPSOLAALERTDESC;

class CSolaAlert
{
public:
	CSolaAlert();
	CSolaAlert(LPSOLAALERT p, int nSize) : m_lpSolaAlert(p), m_cbSize(nSize){}
	inline int GetSize(){ return m_cbSize;};
	inline LPSOLAALERT GetLPMap(int index){ return (m_lpSolaAlert+index);};
public:
	~CSolaAlert(void);
private:
	LPSOLAALERT m_lpSolaAlert;
	int m_cbSize;
};
