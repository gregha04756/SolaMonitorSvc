#include "stdafx.h"
#include "SolaMonitorSvc.h"
#include "SolaMBMap.h"
#include "SolaMBMaps.h"
#include "SolaMultiValue.h"

using namespace std;


HANDLE g_h15Timer;
HANDLE g_h35Timer;

unsigned char* g_pMBRequest;
unsigned char* g_pMBResponse;

RTUCommResult MBWrite(HANDLE& hCOM,unsigned char* mbmsg,int cbmsg);
RTUCommResult MBRead(HANDLE& hCOM,unsigned char* reply,int cbreply,LPDWORD lpreplylen);

RTUCommResult MBWriteRead(HANDLE& hCOM,unsigned char* mbmsg,int cbmsg,unsigned char* reply,int cbreply,LPDWORD lpreplylen)
{
	RTUCommResult rcr;
	unsigned short usCRC16;
	int i;

	PVOID lpvTemp = ::SecureZeroMemory((PVOID)g_pMBRequest,512);
	for ( i = 0; (i < cbmsg) && (i < 512); i++ )
	{
		*(g_pMBRequest+i) = mbmsg[i];
	}
	usCRC16 = calc_CRC16(g_pMBRequest,(unsigned short)i);
	g_pMBRequest[i++] = usCRC16 & 0x00ff;
	g_pMBRequest[i++] = (usCRC16 >> 8) & 0x00ff;
	if ( (rcr = MBWrite(hCOM,g_pMBRequest,i)) != NoError )
	{
		return rcr;
	}
	rcr =  MBRead(hCOM,reply,cbreply,lpreplylen);
	return rcr;
}


RTUCommResult MBWrite(HANDLE& hCOM,unsigned char* mbmsg,int cbmsg)
{
	RTUCommResult rcr = HardError;
	COMSTAT COMStatus;
	BOOL bResult;
	BOOL bSuccess;
	BOOL bIOPending = false;
	DWORD dwResult;
	DWORD dwBytesWritten;
	LARGE_INTEGER liDueTime;
	liDueTime.QuadPart = -10000000LL;

	bIOPending = false;
	g_oWOverlap.Internal = 0;
	g_oWOverlap.InternalHigh = 0;
	g_oWOverlap.Offset = 0;
	g_oWOverlap.OffsetHigh = 0;
	bSuccess = ::SetWaitableTimer(g_h1SecTimer,&liDueTime,0,NULL,NULL,0);
	bResult = ::WriteFile(hCOM,mbmsg,cbmsg,NULL,&g_oWOverlap);
	switch (bResult)
	{
	case 0:
		dwResult = ::GetLastError();
		switch (dwResult)
		{
		case ERROR_IO_PENDING:
			bIOPending = true;
			break;
		default:
			bResult = ::ClearCommError(hCOM,&dwResult,&COMStatus);
			break;
		}
		break;
	default:
		break;
	}
	if ( bIOPending )
	{
		dwResult = ::WaitForMultipleObjects(sizeof(g_hWEvents)/sizeof(HANDLE),g_hWEvents,false,INFINITE);
	}
	if ( WAIT_FAILED == dwResult )
	{
		rcr = HardError;
	}
	if  ( 1 == (dwResult - WAIT_OBJECT_0) )
	{
		bResult = ::ResetEvent(g_hWriteCompletedEvent);
		bResult = ::GetOverlappedResult(hCOM,&g_oWOverlap,&dwBytesWritten,false);
		rcr = NoError;
		g_dwTotalSent += dwBytesWritten;
	}
	return rcr;
}

RTUCommResult MBRead(HANDLE& hCOM,unsigned char* reply,int cbreply,LPDWORD lpreplylen)
{
	RTUCommResult rcr = HardError;
	COMSTAT COMStatus;
	BOOL bSuccess = true;
	BOOL bResult;
	BOOL bIOPending = false;
	DWORD dwResult;
	int i;
	DWORD dwBytesRead;
	LARGE_INTEGER liDueTime;
	liDueTime.QuadPart = -10000000LL;

	*lpreplylen = (DWORD)0;
	bIOPending = false;
	bSuccess = ::SetWaitableTimer(g_h1SecTimer,&liDueTime,0,NULL,NULL,0);
	if  ( bSuccess )
	{
		g_oROverlap.Internal = 0;
		g_oROverlap.InternalHigh = 0;
		g_oROverlap.Offset = 0;
		g_oROverlap.OffsetHigh = 0;
		bResult = ::ReadFile(hCOM,g_pMBResponse,512,NULL,&g_oROverlap);
		switch (bResult)
		{
		case 0:
			dwResult = ::GetLastError();
			switch (dwResult)
			{
			case ERROR_IO_PENDING:
				bIOPending = true;
				break;
			default:
				bResult = ::ClearCommError(hCOM,&dwResult,&COMStatus);
				bSuccess = false;
				break;
			}
			break;
		default:
			break;
		}
	}
	if ( bSuccess && bIOPending )
	{
		dwResult = ::WaitForMultipleObjects(sizeof(g_hREvents)/sizeof(HANDLE),g_hREvents,false,INFINITE);
		if ( WAIT_FAILED == dwResult )
		{
			rcr = HardError;
		}
		if ( 1 == (dwResult - WAIT_OBJECT_0) )
		{
			bResult = ::ResetEvent(g_hReadCompletedEvent);
			bResult = ::GetOverlappedResult(hCOM,&g_oROverlap,&dwBytesRead,false);
			if ( bResult )
			{
				if ( dwBytesRead )
				{
					*lpreplylen = dwBytesRead;
					rcr = (check_CRC16(g_pMBResponse,(int)dwBytesRead) ? NoError : CRCError);
					for ( i = 0; (rcr == NoError) && (i < dwBytesRead) && (i < cbreply); i++ )
					{
						*(reply+i) = *(g_pMBResponse+i);
					}
					g_dwTotalRcvd += (dwBytesRead * (rcr == NoError));
					g_dwTotalCRCErrors += (rcr == CRCError);
				}
				else
				{
					rcr = Timeout;
				}
			}
		}
		if ( 2 == (dwResult - WAIT_OBJECT_0) )
		{
			rcr = Timeout;
		}
	}
	return rcr;
}