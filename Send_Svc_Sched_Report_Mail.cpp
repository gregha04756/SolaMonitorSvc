#include "stdafx.h"
#include "SolaMonitorSvc.h"
#include "MailAcctInfo.h"
#include "SMSvcMsg.h"

using namespace std;

DWORD Send_Svc_Sched_Report_Mail(void)
{
	std::list<std::wstring> *p_rcpnt_list;
	std::list<std::wstring>::iterator it_rl;
	BOOL b_result;
	BOOL b_success;
	int i_result;
	LPMAILSENDPARMS p_msp;
	DWORD dw_result;
	DWORD dw_rc;
	DWORD dw_wait_result;
	DWORD dw_Mail_Send_Thread_ID;
	HANDLE h_Mail_Send_Thread;
	HANDLE hdup_Mail_Send_Thread;
	SYSTEMTIME sys_time;
	char date_time[32];
#if DEBUGMAILSEND
	std::wstring ws_debug;
#endif

	h_Mail_Send_Thread = NULL;
	hdup_Mail_Send_Thread = NULL;
	b_success = true;
	dw_rc = ERROR_SUCCESS;
#if DEBUGMAILSEND
	try
	{
		ws_debug.assign(_T("Send_Svc_Sched_Report_Mail() starting"));
		SvcReportEvent((LPTSTR)ws_debug.c_str(),(DWORD)dw_rc,SVC_INFORMATION);
		p_rcpnt_list = p_MAI->Get_Recipients_List();
		for ( it_rl = p_rcpnt_list->begin(); it_rl != p_rcpnt_list->end(); it_rl++ )
		{
			ws_debug.assign(_T("Send_Svc_Sched_Report_Mail() "));
			ws_debug.append(it_rl->c_str());
			SvcReportEvent((LPTSTR)ws_debug.c_str(),(DWORD)dw_rc,SVC_INFORMATION);
		}
	}
	catch (std::exception &e)
	{
		dw_rc = GetLastError();
		SvcReportEvent(_T("Get_Recipients_Vector() fail"),dw_rc,SVC_ERROR);
	}
#endif
	p_msp = NULL;
	try
	{
		p_msp = (LPMAILSENDPARMS)new MAILSENDPARMS;
	}
	catch(std::bad_alloc &ba)
	{
		dw_rc = GetLastError();
	}
	if ( (NULL != p_msp) && (ERROR_SUCCESS == dw_rc) )
	{
		SecureZeroMemory((PVOID)p_msp,sizeof(MAILSENDPARMS));
		p_msp->lpMAI = p_MAI;
	}
	try
	{
		p_msp->p_msg = (std::list<string>*)new std::list<string>;
	}
	catch(std::bad_alloc &ba)
	{
		dw_rc = GetLastError();
	}
	if ( ERROR_SUCCESS == dw_rc )
	{
		dw_rc = Prep_Report(p_msp->p_msg);
	}
	if ( ERROR_SUCCESS == dw_rc )
	{
		h_Mail_Send_Thread = ::CreateThread(NULL,0,MailSendFunc,(LPVOID)p_msp,CREATE_SUSPENDED,&dw_Mail_Send_Thread_ID);
		dw_rc = (ERROR_SUCCESS != (dw_result = GetLastError())) ? dw_result : dw_rc;
		if ( NULL != h_Mail_Send_Thread )
		{
			b_result = ::DuplicateHandle(::GetCurrentProcess(),h_Mail_Send_Thread,::GetCurrentProcess(),&hdup_Mail_Send_Thread,0,true,DUPLICATE_SAME_ACCESS);
			if ( b_result )
			{
				dw_result = ::ResumeThread(h_Mail_Send_Thread);
				dw_rc = (ERROR_SUCCESS != (dw_result = GetLastError())) ? dw_result : dw_rc;
			}
			else
			{
				b_success = false;
				dw_rc = (ERROR_SUCCESS != (dw_result = GetLastError())) ? dw_result : dw_rc;
			}
		}
		else
		{
			dw_rc = (ERROR_SUCCESS != (dw_result = GetLastError())) ? dw_result : dw_rc;
			SvcReportEvent(_T("Create send mail thread failed "),dw_result,SVC_ERROR);
			b_success = false;
		}
	}
	if ( NULL != h_Mail_Send_Thread )
	{
		dw_wait_result = WaitForSingleObject(h_Mail_Send_Thread,60000);
		if ( WAIT_FAILED == dw_wait_result )
		{
			dw_rc = (ERROR_SUCCESS != (dw_result = GetLastError())) ? dw_result : dw_rc;
			SvcReportEvent(_T("Wait for send mail thread failed "),dw_wait_result,SVC_ERROR);
		}
		if ( WAIT_TIMEOUT == dw_wait_result )
		{
			SvcReportEvent(_T("Wait for send mail thread timeout "),dw_wait_result,SVC_INFORMATION);
		}
		if ( NULL != hdup_Mail_Send_Thread )
		{
			b_result = CloseHandle(hdup_Mail_Send_Thread);
			hdup_Mail_Send_Thread = NULL;
		}
		b_result = CloseHandle(h_Mail_Send_Thread);
		h_Mail_Send_Thread = NULL;
	}
	return dw_rc;
}
