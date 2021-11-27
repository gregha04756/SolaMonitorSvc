#include "stdafx.h"
#include "SolaMonitorSvc.h"
#include "MailAcctInfo.h"
#include "SMSvcMsg.h"

using namespace std;

BOOL Send_Svc_Start_Mail(void)
{
	BOOL b_result;
	BOOL b_success;
	int i_result;
	LPMAILSENDPARMS p_msp;
	DWORD dw_result;
	DWORD dw_wait_result;
	DWORD dw_Mail_Send_Thread_ID;
	HANDLE h_Mail_Send_Thread;
	HANDLE hdup_Mail_Send_Thread;
	const char *ch_Svc_Start = "Sola Monitor service starting at";
	std::string *p_str_msg;
	SYSTEMTIME sys_time;
	char date_time[32];

	h_Mail_Send_Thread = NULL;
	hdup_Mail_Send_Thread = NULL;
	b_success = true;
#if _DEBUG
	SvcReportEvent(_T("Launching MailSendFunc thread"),(DWORD)b_success,SVC_INFORMATION);
#endif
	p_msp = (LPMAILSENDPARMS)new MAILSENDPARMS;
	SecureZeroMemory((PVOID)p_msp,sizeof(MAILSENDPARMS));
	p_msp->p_msg = (std::list<string>*)new std::list<string>;
	p_msp->p_msg->push_back(std::string(ch_Svc_Start));
	SecureZeroMemory((PVOID)date_time,sizeof(date_time));
	GetLocalTime(&sys_time);
	sprintf(date_time,"%-4d-%02d-%02d %02d:%02d",sys_time.wYear,sys_time.wMonth,sys_time.wDay,sys_time.wHour,sys_time.wMinute);
	p_msp->p_msg->push_back(std::string(date_time));
	p_msp->lpMAI = p_MAI;
	h_Mail_Send_Thread = ::CreateThread(NULL,0,MailSendFunc,(LPVOID)p_msp,CREATE_SUSPENDED,&dw_Mail_Send_Thread_ID);
	dw_result = ::GetLastError();
	if ( NULL != h_Mail_Send_Thread )
	{
		b_result = ::DuplicateHandle(::GetCurrentProcess(),h_Mail_Send_Thread,::GetCurrentProcess(),&hdup_Mail_Send_Thread,0,true,DUPLICATE_SAME_ACCESS);
		dw_result = ::GetLastError();
		if ( b_result )
		{
			dw_result = ::ResumeThread(h_Mail_Send_Thread);
		}
		else
		{
			b_success = false;
		}
	}
	else
	{
		SvcReportEvent(_T("Create send mail thread failed "),dw_result,SVC_ERROR);
		b_success = false;
	}
	if ( NULL != h_Mail_Send_Thread )
	{
		dw_wait_result = WaitForSingleObject(h_Mail_Send_Thread,60000);
		if ( WAIT_FAILED == dw_wait_result )
		{
			SvcReportEvent(_T("Wait for send mail thread failed "),dw_wait_result,SVC_ERROR);
		}
		if ( WAIT_TIMEOUT == dw_wait_result )
		{
			SvcReportEvent(_T("Wait for send mail thread timeout "),dw_wait_result,SVC_ERROR);
		}
		if ( NULL != hdup_Mail_Send_Thread )
		{
			b_result = CloseHandle(hdup_Mail_Send_Thread);
		}
		b_result = CloseHandle(h_Mail_Send_Thread);
	}
	return b_success;
}