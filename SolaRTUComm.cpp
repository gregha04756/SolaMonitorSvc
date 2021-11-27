#include "stdafx.h"
#include "SolaMonitorSvc.h"
#include "SolaMBMap.h"
#include "SolaMBMaps.h"
#include "SolaChangeCode.h"
#include "SolaAlert.h"
#include "SolaLockout.h"
#include "SolaMultiValue.h"
#include "SMSvcMsg.h"

using namespace std;

DWORD dwTicksLastData;
BOOL bCRCError;

DWORD g_dwTotalRcvd;
DWORD g_dwTotalSent;
DWORD g_dwTotalCRCErrors;

// Forward declarations of functions included in this code module:
extern "C++" unsigned short calc_CRC16(unsigned char* puchMsg,unsigned short usDataLen);
extern "C++" int check_CRC16(unsigned char *buf,int buflen);
/*extern "C++" RTUCommResult MBWriteRead(HANDLE& hCOM,unsigned char* mbmsg,int cbmsg,unsigned char* reply,int cbreply,LPDWORD replylen);*/
/*extern "C++" RTUCommResult MBRead(HANDLE& hCOM,unsigned char* reply,int cbreply,LPDWORD replylen);*/
RTUCommResult ReadSolaMap(HANDLE& hCOM,unsigned char ucMBAddr,CSolaMBMap* lpMap);
RTUCommResult ReadSolaMap(HANDLE& hCOM,unsigned char ucMBAddr,CSolaMBMaps* lpMaps);
RTUCommResult ReadSolaAlerts(HANDLE& hCOM,unsigned char ucMBAddr,CSolaAlert* lpLog);
RTUCommResult ReadSolaLockouts(HANDLE& hCOM,unsigned char ucMBAddr,CSolaLockout* lpLog);
RTUCommResult GetSolaID(HANDLE& hCOM,unsigned char SolaMBAddress,LPSOLAIDRESPONSE lpSolaID);
RTUCommResult IsBusQuiet(HANDLE& hCOM);

DWORD WINAPI CommThread(LPVOID lpParam)
{
	BOOL bResult;
	BOOL bSuccess = true;
	BOOL bStatusChange = true;
	BOOL bConfigChange = true;
	int i;
	DWORD dwBytesRead;
	DWORD dwResult;
	static HWND hParentWnd = NULL;
	unsigned short usCRC16;
	unsigned short usBit;
	unsigned char MBMsg[512];
	union {	unsigned char SOLAResponse[512]; SOLAHRRESPONSE hr; SOLAIDRESPONSE SolaID; };
	PVOID lpvTemp;
	RTUCommResult rcr;

	bCRCError = false;
	g_dwTotalRcvd = 0L;
	g_dwTotalSent = 0L;
	g_dwTotalCRCErrors = 0L;

//	bResult = ::DuplicateHandle(::GetCurrentProcess(),ghSvcStopEvent,GetCurrentProcess(),&hEvents[0],0,false,DUPLICATE_SAME_ACCESS);

	dwTicksLastData = ::GetTickCount();
	while ( bSuccess && !g_bQuit && ((dwResult = ::WaitForSingleObject(ghSvcStopEvent,0)) == WAIT_TIMEOUT) )
	{
		lpvTemp = ::SecureZeroMemory((PVOID)g_lpSolaID,sizeof(SOLAIDRESPONSE));
		bStatusChange = true;
		bConfigChange = true;
		while ( !g_bQuit && bSuccess && !g_bSolaConnected && ((dwResult = ::WaitForSingleObject(ghSvcStopEvent,0)) == WAIT_TIMEOUT) )
		{
			bSuccess = (HardError != GetSolaID(g_hCOM,g_ucSolaMBAddress,g_lpSolaID));
			g_bSolaConnected = (g_lpSolaID->SolaID != 0);
			if ( g_bSolaConnected )
			{
				SvcReportEvent(_T("SolaConnected"),(DWORD)g_lpSolaID->SolaID,SVC_INFORMATION);
			}
		}
		bSuccess = (HardError != ReadSolaMap(g_hCOM,g_ucSolaMBAddress,pcAllSolaMaps));

/* Get alert log */
		if ( bSuccess && !g_bQuit && g_bSolaConnected )
		{
			bSuccess = (HardError != ReadSolaAlerts(g_hCOM,g_ucSolaMBAddress,pcAlertLog));
		}

/* Get Lockout history log */
		if ( bSuccess && !g_bQuit && g_bSolaConnected )
		{
			bSuccess = (HardError != ReadSolaLockouts(g_hCOM,g_ucSolaMBAddress,pcLockoutLog));
		}
		while ( !g_bQuit && bSuccess && g_bSolaConnected && ((dwResult = ::WaitForSingleObject(ghSvcStopEvent,0)) == WAIT_TIMEOUT) )
		{
/* Always get dynamic trend data */
			if ( bSuccess && !g_bQuit && g_bSolaConnected )
			{
				rcr = ReadSolaMap(g_hCOM,g_ucSolaMBAddress,pcTrendMaps);
				bSuccess = (HardError != rcr);
				dwTicksLastData = ((rcr == NoError) ? ::GetTickCount() : dwTicksLastData);
			}

/* Check if there's a status update */
			bStatusChange = false;
			usBit = 1;
			for ( i = 0; !g_bQuit && bSuccess && i < pcStatusChangeCodes->GetSize(); i++ )
			{
				if ( pcStatusChangeCodes->ItemBitMask(i) & pcSystemStatus->GetValue((int)0) )
				{
					bStatusChange = true;
					if ( i == 1 )
					{
						i = i;
					}
					if ( bSuccess && pcStatusChangeCodes->SolaMBMap(i) != NULL )
					{
						bSuccess = (HardError != ReadSolaMap(g_hCOM,g_ucSolaMBAddress,pcStatusChangeCodes->SolaMBMap(i)));
					}
					if ( bSuccess && pcStatusChangeCodes->SolaAlertLog(i) != NULL )
					{
						bSuccess = (HardError != ReadSolaAlerts(g_hCOM,g_ucSolaMBAddress,pcAlertLog));
					}
					if ( bSuccess && pcStatusChangeCodes->SolaLockoutLog(i) != NULL )
					{
						bSuccess = (HardError != ReadSolaLockouts(g_hCOM,g_ucSolaMBAddress,pcLockoutLog));
					}
				}
				usBit <<= 1;
			}

/* Check if there's a configuration update. */
/* System ID and Access has to be handled separately because of string */
/* values. */
			bConfigChange = false;
			usBit = 1;
			if ( bSuccess && usBit & pcSystemStatus->GetValue((int)1) )
			{
				bConfigChange = true;
				bSuccess = (HardError != ReadSolaMap(g_hCOM,g_ucSolaMBAddress,pcSystemIDMaps));
			}
			for ( i = 0; !g_bQuit && bSuccess && i < pcConfigChangeCodes->GetSize() && pcSystemStatus->GetValue((int)1); i++ )
			{
				if ( pcConfigChangeCodes->ItemBitMask(i) & pcSystemStatus->GetValue((int)1) )
				{
					bConfigChange = true;
					if ( pcConfigChangeCodes->SolaMBMap(i) != NULL )
					{
						bSuccess = (HardError != ReadSolaMap(g_hCOM,g_ucSolaMBAddress,pcConfigChangeCodes->SolaMBMap(i)));
					}
				}
				usBit <<= 1;
			}

			::EnterCriticalSection(&gRWDataCritSect);
			while ( !g_bQuit && bSuccess && !g_MBSndRcvReqQ.empty() )
			{
				i = 0;
				while ( !g_bQuit && *g_MBSndRcvReqQ.front().ppchToSnd < *g_MBSndRcvReqQ.front().ppchEndSnd && *g_MBSndRcvReqQ.front().ppchToSnd < g_MBSndRcvReqQ.front().pchSndBuf + g_MBSndRcvReqQ.front().nSndBufSize  )
				{
					MBMsg[i++] = *(*g_MBSndRcvReqQ.front().ppchToSnd)++;
				}
				usCRC16 = calc_CRC16(MBMsg,i);
				MBMsg[i++] = usCRC16 & 0x00ff;
				MBMsg[i++] = (usCRC16 >> 8) & 0x00ff;
				bSuccess = (HardError != MBWriteRead(g_hCOM,MBMsg,i,g_MBSndRcvReqQ.front().pchRcvBuf,g_MBSndRcvReqQ.front().nRcvBufSize,&dwBytesRead));
				*g_MBSndRcvReqQ.front().ppchEndRcv = g_MBSndRcvReqQ.front().pchRcvBuf + dwBytesRead;
				g_MBSndRcvReqQ.pop();
			}
			::LeaveCriticalSection(&gRWDataCritSect);
			dwResult = ::GetTickCount();
			g_bSolaConnected = ((dwResult - dwTicksLastData) < SOLAREADTIMEOUT);
//			nResult = ::printf("%ld %ld %ld\r",g_dwTotalSent,g_dwTotalRcvd,g_dwTotalCRCErrors);
		}
	} /* End of the main while() loop */

	if ( !bSuccess )
	{
		g_bSolaConnected = false;
		bResult = ::SetEvent(ghWRThreadFailEvent);
	}

	return 0;
}

RTUCommResult GetSolaID(HANDLE& hCOM,unsigned char SolaMBAddress,LPSOLAIDRESPONSE lpSolaID)
{
	RTUCommResult rcr = HardError;
	BOOL bSuccess = true;
	BOOL bBusQuiet = true;
	unsigned char SOLAIDRequest[2];
	int i;
	union {unsigned char SOLAResponse[512]; HOLDINGREG hr; SOLAIDRESPONSE SolaID;};
	DWORD dwBytesWritten = 0;
	DWORD dwBytesRead = 0;
	PVOID lpvTemp;

	bSuccess = (HardError != (rcr = IsBusQuiet(hCOM)));
	if ( bSuccess )
	{
		bBusQuiet = (rcr == Timeout);
	}
	if ( bBusQuiet && bSuccess )
	{
		lpvTemp = ::SecureZeroMemory((PVOID)SOLAResponse,sizeof(SOLAResponse));
		SOLAIDRequest[0] = ::g_ucSolaMBAddress;
		SOLAIDRequest[1] = 0x11;
		rcr = MBWriteRead(hCOM,SOLAIDRequest,sizeof(SOLAIDRequest)/sizeof(unsigned char),SOLAResponse,sizeof(SOLAResponse),&dwBytesRead);
		bSuccess = (rcr != HardError);
	}
	if ( bBusQuiet && (rcr == NoError) && (dwBytesRead > 0) && (SolaID.ByteCount == 0x26) )
	{
		lpSolaID->SolaAddr = SolaID.SolaAddr;
		lpSolaID->FunctionCode = SolaID.FunctionCode;
		lpSolaID->SolaID = SolaID.SolaID;
		lpSolaID->RunIndicator = SolaID.RunIndicator;
		for ( i = 0; i < sizeof(SolaID.BurnerName); i++ )
		{
			*((lpSolaID->BurnerName)+i) = SolaID.BurnerName[i];
		}
		for ( i = 0; i < sizeof(SolaID.OSNumber); i++ )
		{
			*((lpSolaID->OSNumber)+i) = SolaID.OSNumber[i];
		}
		dwTicksLastData = ::GetTickCount();
	}
	return rcr;
}

RTUCommResult IsBusQuiet(HANDLE& hCOM)
{
	RTUCommResult rcr = HardError;
	unsigned char uchBuf[512];
	PVOID lpVoid;
	DWORD dwBytesRead = 0;
	BOOL bSuccess = true;

	::EnterCriticalSection(&gCOMCritSect);
	bSuccess = ::PurgeComm(hCOM,PURGE_RXCLEAR);
	rcr = ((bSuccess) ? NoError : HardError);
//	if ( bSuccess )
//	{
//		bSuccess = GetCommTimeouts(hCOM,&CommTimeouts);
//		rcr = ((bSuccess) ? NoError : HardError);
//	}
//	if ( bSuccess )
//	{
//		OldCommTimeouts.ReadIntervalTimeout = CommTimeouts.ReadIntervalTimeout;
//		OldCommTimeouts.ReadTotalTimeoutConstant = CommTimeouts.ReadTotalTimeoutConstant;
//		OldCommTimeouts.ReadTotalTimeoutMultiplier = CommTimeouts.ReadTotalTimeoutMultiplier;
//		OldCommTimeouts.WriteTotalTimeoutConstant = CommTimeouts.WriteTotalTimeoutConstant;
//		OldCommTimeouts.WriteTotalTimeoutMultiplier = CommTimeouts.WriteTotalTimeoutMultiplier;
//		CommTimeouts.ReadIntervalTimeout = 1000;           /* Wait up to 1 second between characters*/
//		CommTimeouts.ReadTotalTimeoutConstant = 3000; /* Wait up to 3 seconds total */
//		CommTimeouts.ReadTotalTimeoutMultiplier = 0;      // #chars to read does not add to timeout amount
//		CommTimeouts.WriteTotalTimeoutConstant = 2000;
//		CommTimeouts.WriteTotalTimeoutMultiplier = 60;    // 60ms per char sent
//		bSuccess = ::SetCommTimeouts(hCOM,&CommTimeouts);
//		rcr = ((bSuccess) ? NoError : HardError);
//	}
	if ( bSuccess )
	{
		lpVoid = ::SecureZeroMemory((PVOID)uchBuf,(SIZE_T)sizeof(uchBuf));
		rcr = MBRead(hCOM,uchBuf,sizeof(uchBuf)/sizeof(char),&dwBytesRead);
		bSuccess = (rcr != HardError);
	}
//	if ( bSuccess )
//	{
//		CommTimeouts.ReadIntervalTimeout = OldCommTimeouts.ReadIntervalTimeout;
//		CommTimeouts.ReadTotalTimeoutConstant = OldCommTimeouts.ReadTotalTimeoutConstant;
//		CommTimeouts.ReadTotalTimeoutMultiplier = OldCommTimeouts.ReadTotalTimeoutMultiplier;
//		CommTimeouts.WriteTotalTimeoutConstant = OldCommTimeouts.WriteTotalTimeoutConstant;
//		CommTimeouts.WriteTotalTimeoutMultiplier = OldCommTimeouts.WriteTotalTimeoutMultiplier;
//		bSuccess = ::SetCommTimeouts(hCOM,&CommTimeouts);
//		rcr = ((bSuccess) ? rcr : HardError);
//	}
	::LeaveCriticalSection(&gCOMCritSect);
	return rcr;
}

RTUCommResult ReadSolaMap(HANDLE& hCOM,unsigned char ucMBAddr,CSolaMBMaps* lpMaps)
{
	RTUCommResult rcr = HardError;
	int j;
	union {unsigned char SOLAResponse[512];SOLAHRRESPONSE hr;SOLAIDRESPONSE SolaID;};

	for ( j = 0; !g_bQuit && j < lpMaps->GetSize(); j++ )
	{
		rcr = ReadSolaMap(hCOM,ucMBAddr,lpMaps->GetLPMap(j));
	}
	return (RTUCommResult)rcr;
}

RTUCommResult ReadSolaMap(HANDLE& hCOM,unsigned char ucMBAddr,CSolaMBMap* lpMap)
{
	BOOL bOkCRC;
	BOOL bSuccess;
	RTUCommResult rcr = HardError;
	LPSOLAMBMAP lpSolaRequest = NULL;
	DWORD dwBytesRead;
	int j;
	int nOffset;
	unsigned char MBMsg[512];
	unsigned char uchRespLo;
	unsigned char uchRespHi;
	U32 u32Value;
	U32 u32Ret;
	union {unsigned char SOLAResponse[512];SOLAHRRESPONSE hr;SOLAIDRESPONSE SolaID;};
	short sRegCount;
	PVOID lpvTemp;

	if ( NULL != lpMap->GetLPMap((int)0) )
	{
		lpSolaRequest = lpMap->GetLPMap((int)0);
		sRegCount = (short)lpMap->GetRegRequestLen();
	}
	if ( NULL != lpMap->GetLPU32Map((int)0) )
	{
		sRegCount = (short)lpMap->GetU32RegTotal();
	}
	MBMsg[0] = ucMBAddr;
	MBMsg[1] = lpMap->GetFuncCode((int)0);
	MBMsg[2] = (lpMap->GetStartRegAddr((int)0) >> 8) & 0x00ff;
	MBMsg[3] = lpMap->GetStartRegAddr((int)0) & 0x00ff;
	MBMsg[4] = (sRegCount >> 8) & 0x00ff;
	MBMsg[5] = sRegCount & 0x00ff;
//	usCRC16 = calc_CRC16( MBMsg,6 );
//	MBMsg[6] = usCRC16 & 0x00ff;
//	MBMsg[7] = (usCRC16 >> 8) & 0x00ff;
	bSuccess = (HardError != (rcr = MBWriteRead(hCOM,MBMsg,6,SOLAResponse,sizeof(SOLAResponse),&dwBytesRead)));
	bOkCRC = false;
//	if ( bSuccess )
//	{
//		bOkCRC =  check_CRC16(SOLAResponse,dwBytesRead );
//		if ( bOkCRC && bCRCError )
//		{
//			bCRCError = false;
//		}
//	}
//	if ( !bOkCRC )
//	{
//		if ( !bCRCError )
//		{
//			bCRCError = true;
//		}
//	}
//	else
	if ( (rcr == NoError) && (dwBytesRead > 0) )
	{
		for ( nOffset = 0; nOffset < lpMap->GetRegGroupSize(); nOffset++ )
		{
			switch (lpMap->GetType((int)nOffset))
			{
			case Novalue:
				break;
			case Stringvalue:
				if ( lpMap->GetType((int)nOffset) == Stringvalue && (lpMap->GetStr((int)nOffset) != NULL) )
				{
					lpvTemp = (PVOID)lpMap->SetStr((int)nOffset,&SOLAResponse[sizeof(hr)],hr.cbByteCount);
				}
				break;
			case Unsigned32:
				j = nOffset;
				u32Value = SOLAResponse[sizeof(hr)+(j*4)+3] +
					(SOLAResponse[sizeof(hr)+(j*4)+2]*256L) +
					(SOLAResponse[sizeof(hr)+(j*4)+1]*65536L) +
					(SOLAResponse[sizeof(hr)+(j*4)]*16777216L);
				u32Ret = lpMap->SetU32Value((int)nOffset,u32Value);
				break;
			default:
				j = nOffset;
				uchRespLo = SOLAResponse[sizeof(hr)+(j*2)+1];
				uchRespHi = SOLAResponse[sizeof(hr)+(j*2)];
				lpMap->SetValue((int)nOffset,uchRespLo+(256*uchRespHi));
				break;
			}
		}
	}

	return (RTUCommResult)rcr;
}

RTUCommResult ReadSolaAlerts(HANDLE& hCOM,unsigned char ucMBAddr,CSolaAlert* lpLog)
{
	BOOL bOkCRC;
	BOOL bSuccess;
	RTUCommResult rcr = HardError;
	LPSOLAALERT lpAlertRecord;
	DWORD dwBytesRead;
	int i;
	unsigned char MBMsg[512];
	union {unsigned char SOLAResponse[512];SOLAHRRESPONSE hr;SOLAIDRESPONSE SolaID;};
	for ( i = 0; !g_bQuit && i < lpLog->GetSize(); i++ )
	{
		lpAlertRecord = pcAlertLog->GetLPMap(i);
		MBMsg[0] = ucMBAddr;
		MBMsg[1] = lpAlertRecord->uchFuncCode;
		MBMsg[2] = (lpAlertRecord->usStartRegAddr >> 8) & 0x00ff;
		MBMsg[3] = lpAlertRecord->usStartRegAddr & 0x00ff;
		MBMsg[4] = (lpAlertRecord->usRegCount >> 8) & 0x00ff;
		MBMsg[5] = lpAlertRecord->usRegCount & 0x00ff;
//		usCRC16 = calc_CRC16( MBMsg,6 );
//		MBMsg[6] = usCRC16 & 0x00ff;
//		MBMsg[7] = (usCRC16 >> 8) & 0x00ff;
		bSuccess = (HardError != (rcr = MBWriteRead(hCOM,MBMsg,6,SOLAResponse,sizeof(SOLAResponse),&dwBytesRead)));
		bOkCRC = false;
//		if ( bSuccess )
//		{
//			bOkCRC =  check_CRC16(SOLAResponse,dwBytesRead );
//			if ( bOkCRC && bCRCError )
//			{
//				bCRCError = false;
//			}
//		}
//		if ( !bOkCRC )
//		{
//			if ( !bCRCError )
//			{
//				bCRCError = true;
//			}
//		}
		if ( (rcr == NoError) && (dwBytesRead > 0) )
		{
			lpAlertRecord->pAlertRecord->aa.ucAlertCode =  (256*SOLAResponse[sizeof(hr)]) + SOLAResponse[sizeof(hr)+1];
				lpAlertRecord->pAlertRecord->aa.ulCycle = (256*256*256*SOLAResponse[sizeof(hr)+2]) + (256*256*SOLAResponse[sizeof(hr)+3]) +
				(256*SOLAResponse[sizeof(hr)+4]) + (SOLAResponse[sizeof(hr)+5]);
			lpAlertRecord->pAlertRecord->aa.ulHours = (256*256*256*(SOLAResponse[sizeof(hr)+6])) + (256*256*SOLAResponse[sizeof(hr)+7]) +
				(256*SOLAResponse[sizeof(hr)+8]) + (256*256*256*SOLAResponse[sizeof(hr)+9]);
			lpAlertRecord->pAlertRecord->aa.uchCount =  SOLAResponse[sizeof(hr)+11];
		}
	}
	return rcr;
}

RTUCommResult ReadSolaLockouts(HANDLE& hCOM,unsigned char ucMBAddr,CSolaLockout* lpLog)
{
	RTUCommResult rcr = HardError;
	BOOL bOkCRC;
	BOOL bSuccess;
	LPSOLALOCKOUT lpLockoutRecord;
	DWORD dwBytesRead;
	int i;
	unsigned char MBMsg[512];
	union {unsigned char SOLAResponse[512];SOLAHRRESPONSE hr;SOLAIDRESPONSE SolaID;};

	for ( i = 0; !g_bQuit && i < pcLockoutLog->GetSize(); i++ )
	{
		lpLockoutRecord = lpLog->GetLPMap(i);
		MBMsg[0] = ucMBAddr;
		MBMsg[1] = lpLockoutRecord->uchFuncCode;
		MBMsg[2] = (lpLockoutRecord->usStartRegAddr >> 8) & 0x00ff;
		MBMsg[3] = lpLockoutRecord->usStartRegAddr & 0x00ff;
		MBMsg[4] = (lpLockoutRecord->usRegCount >> 8) & 0x00ff;
		MBMsg[5] = lpLockoutRecord->usRegCount & 0x00ff;
//		usCRC16 = calc_CRC16( MBMsg,6 );
//		MBMsg[6] = usCRC16 & 0x00ff;
//		MBMsg[7] = (usCRC16 >> 8) & 0x00ff;
		bSuccess = (HardError != (rcr = MBWriteRead(hCOM,MBMsg,8,SOLAResponse,sizeof(SOLAResponse),&dwBytesRead)));
		bOkCRC = false;
//		if ( bSuccess )
//		{
//			bOkCRC =  check_CRC16(SOLAResponse,dwBytesRead );
//			if ( bOkCRC && bCRCError )
//			{
//				bCRCError = false;
//			}
//		}
//		if ( !bOkCRC )
//		{
//			if ( !bCRCError )
//			{
//				bCRCError = true;
//			}
//		}
		if ( (rcr == NoError) && (dwBytesRead > 0) )
		{
			lpLockoutRecord->pLockoutUnion->slr.usLockoutCode = (256*SOLAResponse[sizeof(hr)]) + SOLAResponse[sizeof(hr)+1];
			lpLockoutRecord->pLockoutUnion->slr.usAnnunciatorFirstOut = (256*SOLAResponse[sizeof(hr)+2]) + SOLAResponse[sizeof(hr)+3];
			lpLockoutRecord->pLockoutUnion->slr.usBurnerControlState = (256*SOLAResponse[sizeof(hr)+4]) + SOLAResponse[sizeof(hr)+5];
			lpLockoutRecord->pLockoutUnion->slr.usSequenceTime = (256*SOLAResponse[sizeof(hr)+6]) + SOLAResponse[sizeof(hr)+7];
			lpLockoutRecord->pLockoutUnion->slr.ulCycle = (16777216*SOLAResponse[sizeof(hr)+8]) + (65536*SOLAResponse[sizeof(hr)+9]) +
				(256*SOLAResponse[sizeof(hr)+10]) + (SOLAResponse[sizeof(hr)+11]);
			lpLockoutRecord->pLockoutUnion->slr.ulHours = (16777216*SOLAResponse[sizeof(hr)+12]) + (65536*SOLAResponse[sizeof(hr)+13]) +
				(256*SOLAResponse[sizeof(hr)+14]) + (SOLAResponse[sizeof(hr)+15]);
			lpLockoutRecord->pLockoutUnion->slr.usIO = (256*SOLAResponse[sizeof(hr)+16]) + SOLAResponse[sizeof(hr)+17];
			lpLockoutRecord->pLockoutUnion->slr.usAnnunciator = (256*SOLAResponse[sizeof(hr)+18]) + SOLAResponse[sizeof(hr)+19];
			lpLockoutRecord->pLockoutUnion->slr.usOutletTemperature = (256*SOLAResponse[sizeof(hr)+20]) + SOLAResponse[sizeof(hr)+21];
			lpLockoutRecord->pLockoutUnion->slr.usInletTemperature = (256*SOLAResponse[sizeof(hr)+22]) + SOLAResponse[sizeof(hr)+23];
			lpLockoutRecord->pLockoutUnion->slr.usDHWTemperature = (256*SOLAResponse[sizeof(hr)+24]) + SOLAResponse[sizeof(hr)+25];
			lpLockoutRecord->pLockoutUnion->slr.usODTemperature = (256*SOLAResponse[sizeof(hr)+26]) + SOLAResponse[sizeof(hr)+27];
			lpLockoutRecord->pLockoutUnion->slr.usStackTemperature = (256*SOLAResponse[sizeof(hr)+28]) + SOLAResponse[sizeof(hr)+29];
			lpLockoutRecord->pLockoutUnion->slr.us4to20mAInput = (256*SOLAResponse[sizeof(hr)+30]) + SOLAResponse[sizeof(hr)+31];
			lpLockoutRecord->pLockoutUnion->slr.ucFaultData0 = SOLAResponse[sizeof(hr)+32];
			lpLockoutRecord->pLockoutUnion->slr.ucFaultData1 = SOLAResponse[sizeof(hr)+33];
		}
	}
	return rcr;
}