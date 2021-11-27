//
//

#include "stdafx.h"
#include "SolaMonitorSvc.h"
#include "SMSvcMsg.h"
#include "MailAcctInfo.h"

SERVICE_STATUS gSvcStatus; 
SERVICE_STATUS_HANDLE gSvcStatusHandle; 
HANDLE ghSvcStopEvent = NULL;
HANDLE ghWRThreadFailEvent = NULL;
HANDLE g_hCOM = NULL;					/* Handle to COM port */
HANDLE g_h1SecTimer;
BOOL g_bSolaConnected;
unsigned char g_ucSolaMBAddress = 0x01;
CRITICAL_SECTION gCOMCritSect;
CRITICAL_SECTION gRWDataCritSect;
OVERLAPPED g_oWOverlap;
OVERLAPPED g_oROverlap;
HANDLE g_hWriteCompletedEvent;
HANDLE g_hReadCompletedEvent;
HANDLE g_h_Monitor_Fail_Event;
LPSOLAIDRESPONSE g_lpSolaID = NULL;
BOOL g_bQuit = false;
HANDLE g_hWEvents[3];
HANDLE g_hREvents[3];
std::queue<MBSNDRCVREQ> g_MBSndRcvReqQ;
const char* INIFileName = "D:\\Visual Studio Projects\\SolaMonitorSvc\\SolaMonCfg.ini";
static CIniFile* lpIniFile;
CMailAcctInfo *p_MAI;

VOID SvcInstall(void);
VOID WINAPI SvcCtrlHandler( DWORD ); 
VOID WINAPI SvcMain( DWORD, LPTSTR * ); 

VOID ReportSvcStatus( DWORD, DWORD, DWORD );
VOID SvcInit( DWORD, LPTSTR * ); 
VOID SvcReportEvent(LPTSTR szFunction,DWORD dwError,DWORD dwIdenifier);

//extern "C++" BOOL InitSolaDatabase();
//extern "C++" DWORD WINAPI CommThread(LPVOID lpParam);

//
// Purpose: 
//   Entry point for the process
//
// Parameters:
//   None
// 
// Return value:
//   None
//
void __cdecl _tmain(int argc, TCHAR *argv[]) 
{ 
    // If command-line parameter is "install", install the service. 
    // Otherwise, the service is probably being started by the SCM.

    if( lstrcmpi( argv[1], TEXT("install")) == 0 )
    {
        SvcInstall();
        return;
    }

    // TO_DO: Add any additional services for the process to this table.
    SERVICE_TABLE_ENTRY DispatchTable[] = 
    { 
        { SVCNAME, (LPSERVICE_MAIN_FUNCTION) SvcMain }, 
        { NULL, NULL } 
    }; 
 
    // This call returns when the service has stopped. 
    // The process should simply terminate when the call returns.

    if (!StartServiceCtrlDispatcher( DispatchTable )) 
    { 
        SvcReportEvent(_T("StartServiceCtrlDispatcher"),GetLastError(),SVC_ERROR); 
    } 
} 

//
// Purpose: 
//   Installs a service in the SCM database
//
// Parameters:
//   None
// 
// Return value:
//   None
//
VOID SvcInstall()
{
    SC_HANDLE schSCManager;
    SC_HANDLE schService;
    TCHAR szPath[MAX_PATH];

    if( !GetModuleFileName( NULL, szPath, MAX_PATH ) )
    {
        printf("Cannot install service (%d)\n", GetLastError());
        return;
    }

    // Get a handle to the SCM database. 
 
    schSCManager = OpenSCManager( 
        NULL,                    // local computer
        NULL,                    // ServicesActive database 
        SC_MANAGER_ALL_ACCESS);  // full access rights 
 
    if (NULL == schSCManager) 
    {
        printf("OpenSCManager failed (%d)\n", GetLastError());
        return;
    }

    // Create the service

    schService = CreateService( 
        schSCManager,              // SCM database 
        SVCNAME,                   // name of service 
        SVCNAME,                   // service name to display 
        SERVICE_ALL_ACCESS,        // desired access 
        SERVICE_WIN32_OWN_PROCESS, // service type 
        SERVICE_DEMAND_START,      // start type 
        SERVICE_ERROR_NORMAL,      // error control type 
        szPath,                    // path to service's binary 
        NULL,                      // no load ordering group 
        NULL,                      // no tag identifier 
        NULL,                      // no dependencies 
        NULL,                      // LocalSystem account 
        NULL);                     // no password 
 
    if (schService == NULL) 
    {
        printf("CreateService failed (%d)\n", GetLastError()); 
        CloseServiceHandle(schSCManager);
        return;
    }
    else printf("Service installed successfully\n"); 

    CloseServiceHandle(schService); 
    CloseServiceHandle(schSCManager);
}

//
// Purpose: 
//   Entry point for the service
//
// Parameters:
//   dwArgc - Number of arguments in the lpszArgv array
//   lpszArgv - Array of strings. The first string is the name of
//     the service and subsequent strings are passed by the process
//     that called the StartService function to start the service.
// 
// Return value:
//   None.
//
VOID WINAPI SvcMain( DWORD dwArgc, LPTSTR *lpszArgv )
{
    // Register the handler function for the service

    gSvcStatusHandle = RegisterServiceCtrlHandler( 
        SVCNAME, 
        SvcCtrlHandler);

    if( !gSvcStatusHandle )
    { 
        SvcReportEvent(_T("RegisterServiceCtrlHandler"),GetLastError(),SVC_ERROR); 
        return; 
    } 

    // These SERVICE_STATUS members remain as set here

    gSvcStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS; 
    gSvcStatus.dwServiceSpecificExitCode = 0;    

    // Report initial status to the SCM

    ReportSvcStatus( SERVICE_START_PENDING, NO_ERROR, 3000 );

    // Perform service-specific initialization and work.

    SvcInit( dwArgc, lpszArgv );
}

//
// Purpose: 
//   The service code
//
// Parameters:
//   dwArgc - Number of arguments in the lpszArgv array
//   lpszArgv - Array of strings. The first string is the name of
//     the service and subsequent strings are passed by the process
//     that called the StartService function to start the service.
// 
// Return value:
//   None
//
VOID SvcInit(DWORD dwArgc,LPTSTR *lpszArgv)
{
    // TO_DO: Declare and set any required variables.
    //   Be sure to periodically call ReportSvcStatus() with 
    //   SERVICE_START_PENDING. If initialization fails, call
    //   ReportSvcStatus with SERVICE_STOPPED.
	static BOOL bResult;
	static DWORD dwResult;
	static DWORD dwWaitResult;
	static HANDLE hWriteReadThread = NULL;
	static HANDLE h_Monitor_Thread = NULL;
	int i;
	int* p_nCommPort;
	static DWORD dwWriteReadThreadID = (DWORD)0;
	static HANDLE hEvents[5];
	PVOID lpvTemp;
	static LPMONITORTHREADPARMS p_m;
	static std::list<int> *p_dal;
	static std::list<int>::iterator it_dal;

	InitializeCriticalSection(&gCOMCritSect);
	InitializeCriticalSection(&gRWDataCritSect);
	if ( NULL == (g_pMBRequest = (unsigned char*)new unsigned char[512]) )
	{
		SvcReportEvent(_T("g_pMBRequest"),GetLastError(),SVC_ERROR);
        ReportSvcStatus(SERVICE_STOPPED,NO_ERROR,0);
        return;
	}
	if ( NULL == (g_pMBResponse = (unsigned char*)new unsigned char[512]) )
	{
		SvcReportEvent(_T("g_pMBResponse"),GetLastError(),SVC_ERROR);
        ReportSvcStatus(SERVICE_STOPPED,NO_ERROR,0);
        return;
	}

/* Open the SolaMonCfg.ini file and read the settings */
	try
	{
		lpIniFile = (CIniFile*)new CIniFile(INIFileName);
	}
	catch (bad_alloc& ba)
	{
		SvcReportEvent(_T("IniFile error"),GetLastError(),SVC_ERROR);
        ReportSvcStatus(SERVICE_STOPPED,NO_ERROR,0);
        return;
	}
	if ( !(lpIniFile->ReadFile()) )
	{
		SvcReportEvent(_T("IniFile read error"),GetLastError(),SVC_ERROR);
        ReportSvcStatus(SERVICE_STOPPED,NO_ERROR,0);
        return;
	}
	SvcReportEvent(_T("IniFile read ok"),GetLastError(),SVC_INFORMATION);

/* Set up e-mail account information */
	if ( NULL == (p_MAI = Setup_Mail_Scct_Info(lpIniFile)) )
	{
		SvcReportEvent(_T("Setup_Mail_Scct_Info() fail"),GetLastError(),SVC_ERROR);
        ReportSvcStatus(SERVICE_STOPPED,NO_ERROR,0);
        return;
	}
	try
	{
		p_nCommPort = (int*)LoadIniFileValues(lpIniFile,IDC_LISTSELECTCOM);
	}
	catch (exception &e)
	{
		SvcReportEvent(_T("Memory alloc error p_nCommPort"),GetLastError(),SVC_ERROR);
        ReportSvcStatus(SERVICE_STOPPED,NO_ERROR,0);
        return;
	}

/* Get list of Modbus addresses and number of devices to poll */
	if ( NULL == (p_dal = (std::list<int>*)LoadIniFileValues(lpIniFile,IDC_LISTMBADDRESSES)) )
	{
		SvcReportEvent(_T("Failure reading Modbus addresses"),GetLastError(),SVC_ERROR);
        ReportSvcStatus(SERVICE_STOPPED,NO_ERROR,0);
        return;
	}
	it_dal = p_dal->begin();
	g_ucSolaMBAddress = (unsigned char)*it_dal;
#if _DEBUG
	SvcReportEvent(_T("Modbus address"),g_ucSolaMBAddress,SVC_INFORMATION);
	SvcReportEvent(_T("Device count"),p_dal->size(),SVC_INFORMATION);
#endif

    // Create an event. The control handler function, SvcCtrlHandler,
    // signals this event when it receives the stop control code.

	for ( i = 0; i < sizeof(hEvents)/sizeof(HANDLE); i++ )
	{
		hEvents[i] = NULL;
	}
    ghSvcStopEvent = CreateEvent(
                         NULL,    // default security attributes
                         TRUE,    // manual reset event
                         FALSE,   // not signaled
                         NULL);   // no name

    if ( ghSvcStopEvent == NULL)
    {
		SvcReportEvent(_T("CreateEvent"),GetLastError(),SVC_ERROR);
        ReportSvcStatus(SERVICE_STOPPED,NO_ERROR,0);
        return;
    }

	ghWRThreadFailEvent = CreateEvent(
                         NULL,    // default security attributes
                         TRUE,    // manual reset event
                         FALSE,   // not signaled
                         NULL);   // no name

    if ( ghWRThreadFailEvent == NULL)
    {
		SvcReportEvent(_T("CreateEvent() failed"),GetLastError(),SVC_ERROR);
        ReportSvcStatus(SERVICE_STOPPED,NO_ERROR,0);
        return;
    }

	g_h_Monitor_Fail_Event = CreateEvent(
                         NULL,    // default security attributes
                         TRUE,    // manual reset event
                         FALSE,   // not signaled
                         NULL);   // no name

    if ( g_h_Monitor_Fail_Event == NULL)
    {
		SvcReportEvent(_T("CreatEvent() failed"),GetLastError(),SVC_ERROR);
        ReportSvcStatus(SERVICE_STOPPED,NO_ERROR,0);
        return;
    }

	bResult = ::DuplicateHandle(::GetCurrentProcess(),ghSvcStopEvent,::GetCurrentProcess(),&hEvents[0],0,true,DUPLICATE_SAME_ACCESS);
    if ( !bResult )
    {
		SvcReportEvent(_T("DuplicateHandle() failed"),GetLastError(),SVC_ERROR);
        ReportSvcStatus(SERVICE_STOPPED,NO_ERROR,0);
        return;
    }

	bResult = ::DuplicateHandle(::GetCurrentProcess(),ghWRThreadFailEvent,::GetCurrentProcess(),&hEvents[1],0,true,DUPLICATE_SAME_ACCESS);
    if ( !bResult )
    {
		SvcReportEvent(_T("DuplicateHandle"),GetLastError(),SVC_ERROR);
        ReportSvcStatus(SERVICE_STOPPED,NO_ERROR,0);
        return;
    }

	if ( (dwResult = OpenCOMPort(NULL,*p_nCommPort,g_hCOM)) != (DWORD)0 )
	{
		SvcReportEvent(_T("OpenCOMPort"),dwResult,SVC_ERROR);
        ReportSvcStatus(SERVICE_STOPPED,SVC_ERROR,0);
        return;
	}
	SvcReportEvent(_T("OpenCOMPort"),dwResult,SVC_INFORMATION);

	if ( !(bResult = InitSolaDatabase()) )
	{
		bResult = ::CloseHandle(g_hCOM);
		SvcReportEvent(_T("InitSolaDatabase"),GetLastError(),SVC_ERROR);
        ReportSvcStatus(SERVICE_STOPPED,SVC_ERROR,0);
        return;
	}
	SvcReportEvent(_T("InitSolaDatabase"),(DWORD)bResult,SVC_INFORMATION);

	g_hWriteCompletedEvent = ::CreateEvent(NULL,true,false,NULL);
	g_hReadCompletedEvent = ::CreateEvent(NULL,true,false,NULL);
	g_h1SecTimer = ::CreateWaitableTimer(NULL,true,NULL);

	lpvTemp = ::SecureZeroMemory((PVOID)&g_oWOverlap,sizeof(OVERLAPPED));
	for ( i = 0; i < sizeof(g_hWEvents)/sizeof(HANDLE); i++ )
	{
		g_hWEvents[i] = NULL;
	}
	bResult = ::DuplicateHandle(::GetCurrentProcess(),ghSvcStopEvent,::GetCurrentProcess(),&g_hWEvents[0],0,true,DUPLICATE_SAME_ACCESS);
	bResult = ::DuplicateHandle(::GetCurrentProcess(),g_hWriteCompletedEvent,::GetCurrentProcess(),&g_hWEvents[1],0,true,DUPLICATE_SAME_ACCESS);
	bResult = ::DuplicateHandle(::GetCurrentProcess(),g_h1SecTimer,::GetCurrentProcess(),&g_hWEvents[2],0,true,DUPLICATE_SAME_ACCESS);
	bResult = ::DuplicateHandle(::GetCurrentProcess(),g_hWriteCompletedEvent,::GetCurrentProcess(),&(g_oWOverlap.hEvent),0,true,DUPLICATE_SAME_ACCESS);

	lpvTemp = ::SecureZeroMemory((PVOID)&g_oROverlap,sizeof(OVERLAPPED));
	for ( i = 0; i < sizeof(g_hREvents)/sizeof(HANDLE); i++ )
	{
		g_hREvents[i] = NULL;
	}
	bResult = ::DuplicateHandle(::GetCurrentProcess(),ghSvcStopEvent,::GetCurrentProcess(),&g_hREvents[0],0,true,DUPLICATE_SAME_ACCESS);
	bResult = ::DuplicateHandle(::GetCurrentProcess(),g_hReadCompletedEvent,::GetCurrentProcess(),&g_hREvents[1],0,true,DUPLICATE_SAME_ACCESS);
	bResult = ::DuplicateHandle(::GetCurrentProcess(),g_h1SecTimer,::GetCurrentProcess(),&g_hREvents[2],0,true,DUPLICATE_SAME_ACCESS);
	bResult = ::DuplicateHandle(::GetCurrentProcess(),g_hReadCompletedEvent,::GetCurrentProcess(),&(g_oROverlap.hEvent),0,true,DUPLICATE_SAME_ACCESS);

	g_lpSolaID = (LPSOLAIDRESPONSE)new SOLAIDRESPONSE;
    if ( !g_lpSolaID )
    {
		SvcReportEvent(_T("new SOLAIDRESPONSE"),GetLastError(),SVC_ERROR);
        ReportSvcStatus(SERVICE_STOPPED,NO_ERROR,0);
        return;
    }

	hWriteReadThread = ::CreateThread(NULL,0,CommThread,NULL,CREATE_SUSPENDED,&dwWriteReadThreadID);
	dwResult = ::GetLastError();
	if ( hWriteReadThread )
	{
		bResult = ::DuplicateHandle(::GetCurrentProcess(),hWriteReadThread,::GetCurrentProcess(),&hEvents[2],0,true,DUPLICATE_SAME_ACCESS);
		dwResult = ::GetLastError();
		if ( bResult )
		{
			dwResult = ::ResumeThread(hWriteReadThread);
		}
		else
		{
			bResult = ::CloseHandle(g_hCOM);
			SvcReportEvent(_T("DuplicateHandle"),dwResult,SVC_ERROR);
			ReportSvcStatus(SERVICE_STOPPED,SVC_ERROR,0);
			return;
		}
	}
	else
	{
		bResult = ::CloseHandle(g_hCOM);
		SvcReportEvent(_T("CreateThread"),dwResult,SVC_ERROR);
		ReportSvcStatus(SERVICE_STOPPED,SVC_ERROR,0);
		return;
	}
	SvcReportEvent(_T("CreateThread"),(DWORD)dwResult,SVC_INFORMATION);

	bResult = ::DuplicateHandle(::GetCurrentProcess(),g_h_Monitor_Fail_Event,::GetCurrentProcess(),&hEvents[3],0,true,DUPLICATE_SAME_ACCESS);
	dwResult = ::GetLastError();
    if ( !bResult )
    {
		SvcReportEvent(_T("DuplicateHandle"),GetLastError(),SVC_ERROR);
        ReportSvcStatus(SERVICE_STOPPED,NO_ERROR,0);
        return;
    }

	SvcReportEvent(_T("Sending service start mail"),(DWORD)GetLastError(),SVC_INFORMATION);
    ReportSvcStatus(SERVICE_START_PENDING,NO_ERROR,10000);
	if ( !Send_Svc_Start_Mail() )
	{
		SvcReportEvent(_T("Send mail error"),GetLastError(),SVC_ERROR);
		ReportSvcStatus(SERVICE_STOPPED,SVC_ERROR,0);
		return;
	}

	p_m = (LPMONITORTHREADPARMS)new MONITORTHREADPARMS;
	lpvTemp = SecureZeroMemory((PVOID)p_m,sizeof(MONITORTHREADPARMS));
/*	p_m->lpMAI = p_MAI;*/
	p_m->p_INI = lpIniFile;
	if ( NULL == (hEvents[4] = Start_Monitor_Thread(p_m)) )
	{
		SvcReportEvent(_T("CreateThread"),p_m->dw_err,SVC_ERROR);
		ReportSvcStatus(SERVICE_STOPPED,SVC_ERROR,0);
		return;
	}
	
/* Report running status when initialization is complete */
    ReportSvcStatus(SERVICE_RUNNING,NO_ERROR,0);
	dwWaitResult = WaitForMultipleObjects(sizeof(hEvents)/sizeof(HANDLE),hEvents,false,INFINITE);

	dwResult = ::GetLastError();
	if ( WAIT_FAILED == dwWaitResult )
	{
		SvcReportEvent(_T("WaitForMultipleObjects"),dwResult,SVC_ERROR);
	}
	if ( 0 == (dwWaitResult-WAIT_OBJECT_0) )
	{
		SvcReportEvent(_T("ServiceStopped"),dwResult,SVC_INFORMATION);
	}
	if ( 1 == (dwWaitResult-WAIT_OBJECT_0) )
	{
		SvcReportEvent(_T("WRThreadFailed"),dwResult,SVC_ERROR);
	}
	if ( 2 == (dwWaitResult-WAIT_OBJECT_0) )
	{
		SvcReportEvent(_T("WRThreadEndNormally"),dwResult,SVC_INFORMATION);
	}
	if ( 3 == (dwWaitResult-WAIT_OBJECT_0) )
	{
		SvcReportEvent(_T("Monitor thread failed "),dwResult,SVC_ERROR);
	}
	if ( 4 == (dwWaitResult-WAIT_OBJECT_0) )
	{
		SvcReportEvent(_T("Monitor thread ended "),dwResult,SVC_ERROR);
	}
	dwWaitResult = ::WaitForSingleObject(hWriteReadThread,5000);
	if ( dwWaitResult == WAIT_FAILED )
	{
		SvcReportEvent(_T("WaitForSingleObject hWriteReadThread"),GetLastError(),SVC_ERROR);
	}
	bResult = ::CloseHandle(hWriteReadThread);
	bResult = ::CloseHandle(g_hCOM);
	bResult = ::CloseHandle(ghSvcStopEvent);
	bResult = ::CloseHandle(ghWRThreadFailEvent);
	bResult = ::CloseHandle(g_h_Monitor_Fail_Event);
	bResult = ::CloseHandle(g_oWOverlap.hEvent);
	bResult = ::CloseHandle(g_oROverlap.hEvent);
	for ( i = 0; i < sizeof(hEvents)/sizeof(HANDLE); i++ )
	{
		bResult = ::CloseHandle(hEvents[i]);
	}
	::DeleteCriticalSection(&gCOMCritSect);
	::DeleteCriticalSection(&gRWDataCritSect);
	if ( g_lpSolaID )
	{
		delete g_lpSolaID;
		g_lpSolaID = NULL;
	}
	for ( i = 0; i < sizeof(g_hWEvents)/sizeof(HANDLE); i++ )
	{
		if ( g_hWEvents[i] )
		{
			bResult = ::CloseHandle(g_hWEvents[i]);
		}
	}
	if ( g_oWOverlap.hEvent )
	{
		bResult = ::CloseHandle(g_oWOverlap.hEvent);
	}
	for ( i = 0; i < sizeof(g_hWEvents)/sizeof(HANDLE); i++ )
	{
		if ( g_hWEvents[i] )
		{
			bResult = ::CloseHandle(g_hWEvents[i]);
		}
	}
	if ( g_oWOverlap.hEvent )
	{
		bResult = ::CloseHandle(g_oWOverlap.hEvent);
	}
	if ( g_pMBRequest )
	{
		delete[] g_pMBRequest;
	}
	if ( g_pMBResponse )
	{
		delete[] g_pMBResponse;
	}
	if ( NULL != lpIniFile )
	{
		delete lpIniFile;
		lpIniFile = NULL;
	}

	if ( NULL != p_MAI )
	{
		delete p_MAI;
		p_MAI = NULL;
	}

	ReportSvcStatus(SERVICE_STOPPED,NO_ERROR,0);
	return;
}

//
// Purpose: 
//   Sets the current service status and reports it to the SCM.
//
// Parameters:
//   dwCurrentState - The current state (see SERVICE_STATUS)
//   dwWin32ExitCode - The system error code
//   dwWaitHint - Estimated time for pending operation, 
//     in milliseconds
// 
// Return value:
//   None
//
VOID ReportSvcStatus( DWORD dwCurrentState,
                      DWORD dwWin32ExitCode,
                      DWORD dwWaitHint)
{
	BOOL bResult;
    static DWORD dwCheckPoint = 1;

    // Fill in the SERVICE_STATUS structure.

    gSvcStatus.dwCurrentState = dwCurrentState;
    gSvcStatus.dwWin32ExitCode = dwWin32ExitCode;
    gSvcStatus.dwWaitHint = dwWaitHint;

    if (dwCurrentState == SERVICE_START_PENDING)
        gSvcStatus.dwControlsAccepted = 0;
    else gSvcStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;

    if ( (dwCurrentState == SERVICE_RUNNING) ||
           (dwCurrentState == SERVICE_STOPPED) )
        gSvcStatus.dwCheckPoint = 0;
    else gSvcStatus.dwCheckPoint = dwCheckPoint++;

    // Report the status of the service to the SCM.
    bResult = SetServiceStatus(gSvcStatusHandle,&gSvcStatus);
}

//
// Purpose: 
//   Called by SCM whenever a control code is sent to the service
//   using the ControlService function.
//
// Parameters:
//   dwCtrl - control code
// 
// Return value:
//   None
//
VOID WINAPI SvcCtrlHandler( DWORD dwCtrl )
{
   // Handle the requested control code. 

   switch(dwCtrl) 
   {  
      case SERVICE_CONTROL_STOP: 
         ReportSvcStatus(SERVICE_STOP_PENDING, NO_ERROR, 0);

         // Signal the service to stop.

         SetEvent(ghSvcStopEvent);
         ReportSvcStatus(gSvcStatus.dwCurrentState, NO_ERROR, 0);
         
         return;
 
      case SERVICE_CONTROL_INTERROGATE: 
         break; 
 
      default: 
         break;
   } 
   
}

//
// Purpose: 
//   Logs messages to the event log
//
// Parameters:
//   szFunction - name of function that failed
// 
// Return value:
//   None
//
// Remarks:
//   The service must have an entry in the Application event log.
//
VOID SvcReportEvent(LPTSTR szFunction,DWORD dwError,DWORD dwIdentifier) 
{ 
    HANDLE hEventSource;
    LPCTSTR lpszStrings[2];
    TCHAR Buffer[80];
	BOOL bResult;
	HRESULT hRes;
	WORD wEventType = EVENTLOG_SUCCESS;

    hEventSource = RegisterEventSource(NULL,SVCNAME);

    if( NULL != hEventSource )
    {
		::SecureZeroMemory((PVOID)Buffer,sizeof(Buffer));
		if ( SVC_ERROR == dwIdentifier )
		{
			wEventType = EVENTLOG_ERROR_TYPE;
	        hRes = StringCchPrintf(Buffer,sizeof(Buffer)/sizeof(TCHAR),_T("%s failed with %d"),szFunction,dwError);
		}
		if ( SVC_INFORMATION == dwIdentifier )
		{
	        hRes = StringCchPrintf(Buffer,sizeof(Buffer)/sizeof(TCHAR),_T("%s returned with %d"),szFunction,dwError);
		}
        lpszStrings[0] = SVCNAME;
        lpszStrings[1] = Buffer;

        bResult = ReportEvent(hEventSource,        // event log handle
                    wEventType, // event type
                    0,                   // event category
                    dwIdentifier,           // event identifier
                    NULL,                // no security identifier
                    2,                   // size of lpszStrings array
                    0,                   // no binary data
                    lpszStrings,         // array of strings
                    NULL);               // no binary data

        DeregisterEventSource(hEventSource);
    }
}