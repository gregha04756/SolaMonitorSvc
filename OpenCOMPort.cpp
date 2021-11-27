#include "stdafx.h"

DWORD OpenCOMPort(HWND hParentWnd,int nCommPort,HANDLE &hCOM)
{
	DWORD dwResult = 0;
	DCB dcb;
	COMMTIMEOUTS CommTimeouts;
	BOOL bSuccess = true;
	BOOL bResult;
	TCHAR szCommPort[16];
	HRESULT hRes;

	if ( hCOM != NULL )
	{
		bResult = ::CloseHandle(hCOM);
	}
	hRes = ::StringCchPrintf(szCommPort,sizeof(szCommPort)/sizeof(TCHAR),_T("COM%d"),nCommPort);
	if ( 10 <= nCommPort )
	{
		hRes = ::StringCchPrintf(szCommPort,sizeof(szCommPort)/sizeof(TCHAR),_T("\\\\.\\COM%d"),nCommPort );
	}
	hCOM = CreateFile(szCommPort,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_OVERLAPPED,
		NULL);
	dwResult = ::GetLastError();
	if ( hCOM == INVALID_HANDLE_VALUE )
	{
		hCOM = NULL;
		bSuccess = false;
	}

	if ( bSuccess )
	{
		if ( !(bSuccess = GetCommState(hCOM,&dcb))) 
		{
			// Handle the error.
			dwResult = ::GetLastError();
		}
	}
	if ( bSuccess )
	{
		// Fill in DCB: 38,400 bps, 8 data bits, no parity, and 1 stop bit.
		dcb.BaudRate = CBR_38400;     // set the baud rate
//		g_dT35 = (3.5)/((double)dcb.BaudRate);
		dcb.ByteSize = 8;             // data size, xmit, and rcv
		dcb.Parity = NOPARITY;        // no parity bit
		dcb.StopBits = ONESTOPBIT;    // one stop bit
		dcb.fBinary = true;
		dcb.fAbortOnError = false;
		if ( !(bSuccess = SetCommState(hCOM,&dcb)) ) 
		{
			// Handle the error.
			dwResult = ::GetLastError();
		}	
	}
	
	if ( bSuccess )
	{
		if ( !(bSuccess = GetCommTimeouts(hCOM,&CommTimeouts)) )
		{
			// Handle the error.
			dwResult = ::GetLastError();
		}
	}
	if ( bSuccess )
	{
//		CommTimeouts.ReadIntervalTimeout = 25;           // max allowed time after receipt of first character
//		CommTimeouts.ReadTotalTimeoutConstant = 0;
//		CommTimeouts.ReadTotalTimeoutMultiplier = 0;      // #chars to read does not add to timeout amount
//		g_bHiResTimer = true;
#ifndef USE_35CHAR_TIMES
		CommTimeouts.ReadIntervalTimeout = 25;
		CommTimeouts.ReadTotalTimeoutConstant = 0;
#endif
#ifdef USE_35CHAR_TIMES
		CommTimeouts.ReadIntervalTimeout = 25;
		CommTimeouts.ReadTotalTimeoutConstant = 0;
#endif
		CommTimeouts.ReadTotalTimeoutMultiplier = 0;
		CommTimeouts.WriteTotalTimeoutConstant = 2000;
		CommTimeouts.WriteTotalTimeoutMultiplier = 60;    // 60ms per char sent
		if ( !(bSuccess = ::SetCommTimeouts(hCOM,&CommTimeouts)) ) 
		{
			// Handle the error.
			dwResult = ::GetLastError();
		}	
	}
	return dwResult;
}
