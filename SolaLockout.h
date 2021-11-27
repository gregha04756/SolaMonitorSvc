#pragma once

#include "StdAfx.h"

typedef struct __tagSolaLockoutRecord {	unsigned short usLockoutCode;			//0-1
										unsigned short usAnnunciatorFirstOut;	//2-3
										unsigned short usBurnerControlState;	//4-5
										unsigned short usSequenceTime;			//6-7
										unsigned long ulCycle;					//8-11
										unsigned long ulHours;					//12-15
										unsigned short usIO;					//16-17
										unsigned short usAnnunciator;			//18-19
										unsigned short usOutletTemperature;		//20-21
										unsigned short usInletTemperature;		//22-23
										unsigned short usDHWTemperature;		//24-25
										unsigned short usODTemperature;			//26-27
										unsigned short usStackTemperature;		//28-29
										unsigned short us4to20mAInput;			//30-31
										unsigned char ucFaultData0;				//32
										unsigned char ucFaultData1;				//33
} SOLALOCKOUTRECORD, *LPSOLALOCKOUTRECORD;

typedef union __tagSolaLockoutUnion {	unsigned char cc[sizeof(SOLALOCKOUTRECORD)];
										SOLALOCKOUTRECORD slr; } SOLALOCKOUTUNION, *LPSOLALOCKOUTUNION;

typedef struct __tagSolaLockout {	unsigned char uchDevAddr;
									const unsigned char uchFuncCode;
									const unsigned short usStartRegAddr;
									const unsigned short usRegCount;
									LPSOLALOCKOUTUNION pLockoutUnion; } SOLALOCKOUT, *LPSOLALOCKOUT;

class CSolaLockout
{
public:
	CSolaLockout();
	CSolaLockout(LPSOLALOCKOUT p, int nSize);
	inline int GetSize(){return m_cbSize;};
	LPSOLALOCKOUT GetLPMap(int index);
public:
	~CSolaLockout(void);
private:
	LPSOLALOCKOUT m_lpSolaLockout;
	int m_cbSize;
};
