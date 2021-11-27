#include "stdafx.h"
#include "SolaMonitorSvc.h"
#include "SMSvcMsg.h"

BOOL Mail_Report_Is_Scheduled(CIniFile* p_INI)
{
	int i_r;
	TCHAR *p_sz;
	std::string ss;
	BOOL *p_b;
	BOOL b_r;
	b_r = false;
	p_b = NULL;
	p_sz = NULL;
	try
	{
		b_r = *(p_b = (BOOL*)LoadIniFileValues(p_INI,IDC_CHKSNDSCHED)); 
	}
	catch (std::bad_alloc &ba)
	{
		b_r = false;
#if _DEBUG
		ss.assign(ba.what());
		p_sz = (TCHAR*)new TCHAR[ss.size()+1];
		SecureZeroMemory((PVOID)p_sz,(ss.size()+1)*sizeof(TCHAR));
		i_r = MultiByteToWideChar(CP_ACP,
			MB_PRECOMPOSED,
			ss.c_str(),
			ss.size(),
			p_sz,
			ss.size());
		SvcReportEvent(p_sz,GetLastError(),SVC_INFORMATION);
		if ( NULL != p_sz )
		{
			delete[] p_sz;
		}
#endif
	}
	if ( NULL != p_b )
	{
		delete p_b;
	}
	return b_r;
}

DWORD Get_Schedule_Times(CIniFile *p_INI,int &i_d,int &i_h,int &i_m)
{
	DWORD dw_rc;
	int *pi_d;
	int *pi_h;
	int *pi_m;
	int i_r;
	HRESULT h_r;
	TCHAR *p_sz;
	std::string ss;
	BOOL b_r;
	b_r = false;
	pi_d = pi_h = pi_m = NULL;
	p_sz = NULL;
	dw_rc = ERROR_SUCCESS;
	i_d = -1;
	i_d = -1;
	i_d = -1;
	try
	{
		i_d = *(pi_d = (BOOL*)LoadIniFileValues(p_INI,IDC_EDITDAYS)); 
		i_h = *(pi_h = (BOOL*)LoadIniFileValues(p_INI,IDC_EDITHRS)); 
		i_m = *(pi_m = (BOOL*)LoadIniFileValues(p_INI,IDC_EDITMINS)); 
	}
	catch (std::bad_alloc &ba)
	{
		b_r = false;
#if _DEBUG
		ss.assign(ba.what());
		p_sz = (TCHAR*)new TCHAR[ss.size()+1];
		SecureZeroMemory((PVOID)p_sz,(ss.size()+1)*sizeof(TCHAR));
		i_r = MultiByteToWideChar(CP_ACP,
			MB_PRECOMPOSED,
			ss.c_str(),
			ss.size(),
			p_sz,
			ss.size());
		SvcReportEvent(p_sz,GetLastError(),SVC_INFORMATION);
		if ( NULL != p_sz )
		{
			delete[] p_sz;
		}
#endif
	}
	if ( NULL != pi_d )
	{
		delete pi_d;
	}
	if ( NULL != pi_h )
	{
		delete pi_h;
	}
	if ( NULL != pi_m )
	{
		delete pi_m;
	}
#if _DEBUG
	p_sz = (TCHAR*)new TCHAR[MAX_LOADSTRING];
	SecureZeroMemory((PVOID)p_sz,32*sizeof(TCHAR));
	h_r = StringCchPrintf(p_sz,32,_T("Sched. days=%d hour=%d minute=%d"),i_d,i_h,i_m);
	SvcReportEvent(p_sz,GetLastError(),SVC_INFORMATION);
	if ( NULL != p_sz )
	{
		delete[] p_sz;
	}
#endif
	return  dw_rc;
}

DWORD Get_Polling_Time(CIniFile *p_INI,int &i_h,int &i_m,int &i_s)
{
	DWORD dw_rc;
	int *pi_h;
	int *pi_m;
	int *pi_s;
	int i_r;
	HRESULT h_r;
	TCHAR *p_sz;
	std::string ss;
	BOOL b_r;
	b_r = false;
	pi_h = pi_m = pi_s = NULL;
	p_sz = NULL;
	dw_rc = ERROR_SUCCESS;
	i_h = -1;
	i_h = -1;
	i_h = -1;
	try
	{
		i_h = *(pi_h = (BOOL*)LoadIniFileValues(p_INI,IDC_EDITPOLLHRS)); 
		i_m = *(pi_m = (BOOL*)LoadIniFileValues(p_INI,IDC_EDITPOLLMINS)); 
		i_s = *(pi_s = (BOOL*)LoadIniFileValues(p_INI,IDC_EDITPOLLSECS)); 
	}
	catch (std::bad_alloc &ba)
	{
		b_r = false;
#if _DEBUG
		ss.assign(ba.what());
		p_sz = (TCHAR*)new TCHAR[ss.size()+1];
		SecureZeroMemory((PVOID)p_sz,(ss.size()+1)*sizeof(TCHAR));
		i_r = MultiByteToWideChar(CP_ACP,
			MB_PRECOMPOSED,
			ss.c_str(),
			ss.size(),
			p_sz,
			ss.size());
		SvcReportEvent(p_sz,GetLastError(),SVC_INFORMATION);
		if ( NULL != p_sz )
		{
			delete[] p_sz;
		}
#endif
	}
	if ( NULL != pi_h )
	{
		delete pi_h;
		pi_h = NULL;
	}
	if ( NULL != pi_m )
	{
		delete pi_m;
		pi_m = NULL;
	}
	if ( NULL != pi_s )
	{
		delete pi_s;
		pi_s = NULL;
	}
#if _DEBUG
	p_sz = (TCHAR*)new TCHAR[MAX_LOADSTRING];
	SecureZeroMemory((PVOID)p_sz,MAX_LOADSTRING*sizeof(TCHAR));
	h_r = StringCchPrintf(p_sz,MAX_LOADSTRING,_T("Polling hrs=%d mins=%d secs=%d"),i_h,i_m,i_s);
	SvcReportEvent(p_sz,GetLastError(),SVC_INFORMATION);
	if ( NULL != p_sz )
	{
		delete[] p_sz;
	}
#endif
	return  dw_rc;
}

DWORD Get_Notice_Time(CIniFile *p_INI,int &i_h,int &i_m)
{
	DWORD dw_rc;
	int *pi_h;
	int *pi_m;
	int i_r;
	HRESULT h_r;
	TCHAR *p_sz;
	std::string ss;
	BOOL b_r;
	b_r = false;
	pi_h = pi_m = NULL;
	p_sz = NULL;
	dw_rc = ERROR_SUCCESS;
	try
	{
		i_h = *(pi_h = (BOOL*)LoadIniFileValues(p_INI,IDC_EDITNOTICEHRS)); 
		i_m = *(pi_m = (BOOL*)LoadIniFileValues(p_INI,IDC_EDITNOTICEMINS)); 
	}
	catch (std::bad_alloc &ba)
	{
		b_r = false;
#if _DEBUG
		ss.assign(ba.what());
		p_sz = (TCHAR*)new TCHAR[ss.size()+1];
		SecureZeroMemory((PVOID)p_sz,(ss.size()+1)*sizeof(TCHAR));
		i_r = MultiByteToWideChar(CP_ACP,
			MB_PRECOMPOSED,
			ss.c_str(),
			ss.size(),
			p_sz,
			ss.size());
		SvcReportEvent(p_sz,GetLastError(),SVC_INFORMATION);
		if ( NULL != p_sz )
		{
			delete[] p_sz;
		}
#endif
	}
	if ( NULL != pi_h )
	{
		delete pi_h;
	}
	if ( NULL != pi_m )
	{
		delete pi_m;
	}
#if _DEBUG
	p_sz = (TCHAR*)new TCHAR[MAX_LOADSTRING];
	SecureZeroMemory((PVOID)p_sz,MAX_LOADSTRING*sizeof(TCHAR));
	h_r = StringCchPrintf(p_sz,MAX_LOADSTRING,_T("Notices at %d hour %d minute intervals"),i_h,i_m);
	SvcReportEvent(p_sz,GetLastError(),SVC_INFORMATION);
	if ( NULL != p_sz )
	{
		delete[] p_sz;
	}
#endif
	return  dw_rc;
}

DWORD Calc_LI_Time(LPSYSTEMTIME p_st_now,LARGE_INTEGER *p_li_send_time,LPSYSTEMTIME p_st_send,int &d,int &h,int &m)
{
	BOOL b_r;
	DWORD dw_rc;
	dw_rc = ERROR_SUCCESS;
	FILETIME ft_send_time;

	if ( memcpy_s(p_st_send,sizeof(SYSTEMTIME),p_st_now,sizeof(SYSTEMTIME)) )
	{
		dw_rc = GetLastError();
	}
	if ( d > 0 )
	{
		p_st_send->wHour = h;
		p_st_send->wMinute = m;
		p_st_send->wSecond = 0;
	}
	if ( !(b_r = SystemTimeToFileTime(p_st_send,&ft_send_time)) )
	{
		dw_rc = GetLastError();
	}
	p_li_send_time->LowPart = ft_send_time.dwLowDateTime;
	p_li_send_time->HighPart = ft_send_time.dwHighDateTime;

	return dw_rc;
}

DWORD Calc_LI_Time(LARGE_INTEGER *p_li_delta,LARGE_INTEGER *p_li_send_time,LPSYSTEMTIME p_st_send)
{
	BOOL b_r;
	FILETIME ft_send_time;
	DWORD dw_rc;
	p_li_send_time->QuadPart += p_li_delta->QuadPart;
	ft_send_time.dwLowDateTime = p_li_send_time->LowPart;
	ft_send_time.dwHighDateTime = p_li_send_time->HighPart;
	b_r = FileTimeToSystemTime(&ft_send_time,p_st_send);
	dw_rc = GetLastError();
	return dw_rc;
}

LARGE_INTEGER Calc_LI_Rel_TIme(int i_days,int i_hours,int i_minutes,int i_seconds)
{
	LARGE_INTEGER li_d;
	LARGE_INTEGER li_h;
	LARGE_INTEGER li_m;
	LARGE_INTEGER li_s;
	LARGE_INTEGER li_rt;
	li_d.QuadPart = i_days;
	li_h.QuadPart = i_hours;
	li_m.QuadPart = i_minutes;
	li_s.QuadPart = i_seconds;
	li_rt.QuadPart = -10000000*((i_days*i_24hr)+(i_hours*3600)+(i_minutes*60)+i_seconds);
	return li_rt;
}

/*
The return value is one of the following:

Return value  Description
------------  -----------
-1            First time is earlier than second time.
0             First time is equal to second time.
1             First time is later than second time.
*/
LONG Compare_Two_System_Times(LPSYSTEMTIME const lp_st1,LPSYSTEMTIME const lp_st2)
{
	BOOL b_r;
	FILETIME ft1;
	FILETIME ft2;
	b_r = SystemTimeToFileTime((const LPSYSTEMTIME)lp_st1,&ft1);
	b_r = SystemTimeToFileTime((const LPSYSTEMTIME)lp_st2,&ft2);
	return CompareFileTime((const FILETIME*)&ft1,(const FILETIME*)&ft2);
}
