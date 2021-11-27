
#include "stdafx.h"
#include "SolaMonitorSvc.h"
#include "SMSvcMsg.h"
#include "MailAcctInfo.h"

/* Define some constants */
LONGLONG const ll_1sec = 10000000LL;
int const i_24hr = 86400;
uint16_t const ui16_No_source_demand = 1;
uint16_t const ui16_Warm_weather_shutdown = 1;

#define INITIAL_STATE Entry_State

/* Timer handles and events are global so they can be accessed from different state functions */
BOOL b_Is_Scheduled;
/*LONG const l_notice_period = 1000;*/
LONG const l_poll_period = 1000; /* Poll timer period is 1000 milliseconds */
int32_t i32_Notice_Interval;
int32_t i32_Notice_Counter;
int32_t i32_Poll_Interval;
int32_t i32_Poll_Counter;
ULARGE_INTEGER li_send_time;
LARGE_INTEGER li_poll_time;
LPSYSTEMTIME st_send_time;
LPSYSTEMTIME st_now_time;
ULARGE_INTEGER li_ti;
HANDLE h_send_timer;
HANDLE h_poll_timer;
HANDLE h_Events[3];
enum Burner_Control_Status_State_Values cur_state;
enum Burner_Control_Status_State_Values prev_state;
int32_t i32_states;

func_ptr const Entry_state_fn;
func_ptr const Not_Connected_state_fn;
func_ptr const Disabled_state_fn;
func_ptr const Locked_out_state_fn;
func_ptr const Anti_short_cycle_state_fn;
func_ptr const Unconfigured_safety_data_state_fn;
func_ptr const Standby_Hold_state_fn;
func_ptr const Standby_Delay_state_fn;
func_ptr const Normal_Standby_state_fn;
func_ptr const Preparing_state_fn;
func_ptr const Ignition_state_fn;
func_ptr const Firing_state_fn;
func_ptr const Postpurge_state_fn;

func_ptr *lookup_state_fn(enum Burner_Control_Status_State_Values sv);
char const *lookup_state_name(enum Burner_Control_Status_State_Values sv);

struct state_descriptor const sd_descriptors[] = {
	"Entry_State",Entry_State,Entry_state_fn,
	"Not_Connected",Not_Connected,Not_Connected_state_fn,
	"Disabled",Burner_control_status_Disabled,Disabled_state_fn,
	"Locked_out",Burner_control_status_Locked_out,Locked_out_state_fn,
	"Anti_short_cycle",Burner_control_status_Anti_short_cycle,Anti_short_cycle_state_fn,
	"Unconfigured_safety_data",Burner_control_status_Unconfigured_safety_data,Unconfigured_safety_data_state_fn,
	"Standby_Hold",Burner_control_status_Standby_Hold,Standby_Hold_state_fn,
	"Standby_Delay",Burner_control_status_Standby_Delay,Standby_Delay_state_fn,
	"Normal_Standby",Burner_control_status_Normal_Standby,Normal_Standby_state_fn,
	"Preparing",Burner_control_status_Preparing,Preparing_state_fn,
	"Ignition",Burner_control_status_Ignition,Ignition_state_fn,
	"Firing",Burner_control_status_Firing,Firing_state_fn,
	"Postpurge",Burner_control_status_Postpurge,Postpurge_state_fn,
};


HANDLE Start_Monitor_Thread(LPMONITORTHREADPARMS p_m)
{
	BOOL b_r;
	HANDLE h_t;
	DWORD dw_r;
	h_t = NULL;

	p_m->dw_err = ERROR_SUCCESS;
	h_t = CreateThread(NULL,0,Monitor_Thread_Fn,p_m,CREATE_SUSPENDED,&(p_m->dw_tid));
	dw_r = GetLastError();
	if ( h_t )
	{
		b_r = DuplicateHandle(GetCurrentProcess(),h_t,GetCurrentProcess(),&(p_m->hdup_th),0,true,DUPLICATE_SAME_ACCESS);
		dw_r = GetLastError();
		if ( b_r )
		{
			dw_r = ResumeThread(h_t);
		}
		else
		{
			p_m->dw_err = dw_r;
		}
	}
	else
	{
		p_m->dw_err = dw_r;
	}
	return h_t;
}

DWORD WINAPI Monitor_Thread_Fn(LPVOID lParam)
{
	BOOL b_r;
	int i_i;
	int i_ad;
	int i_ah;
	int i_am;
	int i_ph;
	int i_pm;
	int i_ps;
	int i_nh;
	int i_nm;
	int i_ns = 0;
	DWORD dw_wr;
	DWORD dw_rc;
	PVOID p_v;
	CIniFile *p_INI;
	LPMONITORTHREADPARMS p_m;
	FILETIME ft_send_time;
	std::list<LPSYSTEMTIME> *p_times;
	func_ptr *psf;
	enum Burner_Control_Status_State_Values sc;
	TCHAR sz_State_Transition[200];
	std::string str_State_Transition;

	cur_state = INITIAL_STATE;
	prev_state = INITIAL_STATE;
	dw_rc = ERROR_SUCCESS;
	p_m = (LPMONITORTHREADPARMS)lParam;
	p_INI = p_m->p_INI;
	delete p_m;
	i_ad = 0;
	i_ah = 0;
	i_am = 0;

	for ( i_i = 0; i_i < sizeof(h_Events)/sizeof(HANDLE); i_i++ )
	{
		h_Events[i_i] = NULL;
	}

/* Create timer handles */
	h_send_timer = CreateWaitableTimer(NULL,FALSE,NULL);
	dw_rc = GetLastError();
	if ( ERROR_SUCCESS == dw_rc )
	{
		h_poll_timer = CreateWaitableTimer(NULL,FALSE,NULL);
	}
/* Duplicate events to wait for */
	if ( NULL != ghSvcStopEvent )
	{
		b_r = DuplicateHandle(GetCurrentProcess(),ghSvcStopEvent,GetCurrentProcess(),&h_Events[0],0,true,DUPLICATE_SAME_ACCESS);
		dw_rc = GetLastError();
	}
	if ( NULL != h_send_timer )
	{
		b_r = DuplicateHandle(GetCurrentProcess(),h_send_timer,GetCurrentProcess(),&h_Events[1],0,true,DUPLICATE_SAME_ACCESS);
		dw_rc = GetLastError();
	}
	if ( NULL != h_poll_timer )
	{
		b_r = DuplicateHandle(GetCurrentProcess(),h_poll_timer,GetCurrentProcess(),&h_Events[2],0,true,DUPLICATE_SAME_ACCESS);
		dw_rc = GetLastError();
	}

/* Get reporting schedule */
	if ( (b_Is_Scheduled = Mail_Report_Is_Scheduled(p_INI)) )
	{
		dw_rc = Get_Schedule_Times(p_INI,i_ad,i_ah,i_am);
	}

/* Get polling schedule */
	dw_rc = Get_Polling_Time(p_INI,i_ph,i_pm,i_ps);
	li_poll_time = Calc_LI_Rel_TIme(0,0,0,1); /* Make polling timer have a one second interval */
	i32_Poll_Interval = (i_ph*3600)+(i_pm*60)+i_ps; /* Poll interval in seconds */
	i32_Poll_Counter = i32_Poll_Interval; /* Set polling counter which is deceremented every time polling timer signals*/

/* Get notification interval for abnormal condition i.e. Hold or Lockout*/
	dw_rc = Get_Notice_Time(p_INI,i_nh,i_nm);
	i32_Notice_Interval = (i_nh*3600)+(i_nm*60); /* Notice interval in seconds */
	i32_Notice_Counter = i32_Notice_Interval; /* Set notice counter which is deceremented every time notice timer signals*/

/* Set polling timeer, make it periodic */
	if ( !SetWaitableTimer(h_poll_timer,(const LARGE_INTEGER*)&li_poll_time,l_poll_period,NULL,NULL,0) )
	{
		dw_rc = GetLastError();
	}

	if (ERROR_SUCCESS == dw_rc)
	{
		try
		{
			st_send_time = (LPSYSTEMTIME)new SYSTEMTIME;
			st_now_time = (LPSYSTEMTIME)new SYSTEMTIME;
		}
		catch (std::bad_alloc &ba)
		{
			dw_rc = GetLastError();
		}
	}

	if ( ERROR_SUCCESS == dw_rc && (NULL != st_now_time) && (NULL != st_send_time) )
	{
		li_ti.QuadPart = (i_ad == 0 ) ? ((i_ah*3600)+(i_am*60))*ll_1sec : i_24hr*ll_1sec;
		GetSystemTime(st_now_time);
		dw_rc = Calc_LI_Time(st_now_time,(LARGE_INTEGER*)&li_send_time,st_send_time,i_ad,i_ah,i_am);
		while ((ERROR_SUCCESS == dw_rc) && (1L == Compare_Two_System_Times((const LPSYSTEMTIME)st_now_time,(const LPSYSTEMTIME)st_send_time)))
		{
/* st_now_time is later than st_send_time so add the schedule time increment until st_send_time is > or = st_now_time */
			dw_rc = Calc_LI_Time((LARGE_INTEGER*)&li_ti,(LARGE_INTEGER*)&li_send_time,st_send_time);
		}
	}
	if ( b_Is_Scheduled )
	{
		if ( !SetWaitableTimer(h_send_timer,(const LARGE_INTEGER*)&li_send_time,0,NULL,NULL,0) )
		{
			dw_rc = GetLastError();
		}
	}

/* State machine loop */
	if (ERROR_SUCCESS == dw_rc)
	{
		for (;;)
		{
			dw_wr = WaitForSingleObject(h_Events[0],0); /* Check if service stop event is set */
			dw_rc = GetLastError();
			if (WAIT_FAILED == dw_wr)
			{
				SvcReportEvent(_T("Monitor_Thread_Fn() wait failed"),dw_rc,SVC_ERROR);
				break;
			}
			if (dw_wr == WAIT_OBJECT_0)
			{
				break; /* The event for stopping this service was signalled so break out of this loop */
			}

			dw_wr = WaitForSingleObject(h_Events[1],0);
			dw_rc = GetLastError();
			if (WAIT_FAILED == dw_wr)
			{
				SvcReportEvent(_T("Monitor_Thread_Fn() wait failed"),dw_rc,SVC_ERROR);
				break;
			}
			if (b_Is_Scheduled && (dw_wr == WAIT_OBJECT_0)) /* Time to send a report */
			{
				dw_rc = Calc_LI_Time((LARGE_INTEGER*)&li_ti,(LARGE_INTEGER*)&li_send_time,st_send_time);
				dw_rc = Send_Svc_Sched_Report_Mail();
				SvcReportEvent(_T("Scheduled report time"),(DWORD)cur_state,SVC_INFORMATION);
				if ( !SetWaitableTimer(h_send_timer,(const LARGE_INTEGER*)&li_send_time,0,NULL,NULL,0) )
				{
					break;
				}
			}

			dw_wr = WaitForSingleObject(h_Events[2],INFINITE); /* Polling timer */
			dw_rc = GetLastError();
			if (WAIT_FAILED == dw_wr)
			{
				SvcReportEvent(_T("Monitor_Thread_Fn() wait failed"),dw_rc,SVC_ERROR);
				break;
			}
			if (dw_wr == WAIT_OBJECT_0) /* Call state function to check control status */
			{
#if DEBUGPOLL
				SvcReportEvent(_T("Polling time"),(DWORD)cur_state,SVC_INFORMATION);
#endif
				i32_Poll_Counter = (0 < i32_Poll_Counter) ? --i32_Poll_Counter : i32_Poll_Interval;
				if (0 > i32_Poll_Counter)
				{
					i32_Poll_Counter = i32_Poll_Interval;
				}

				if (!g_bSolaConnected)
				{
					cur_state = Not_Connected;
				}
				psf = lookup_state_fn(cur_state);
				if (NULL == psf)
				{
					SvcReportEvent(_T("Error: Undetermined state"),(DWORD)cur_state,SVC_ERROR);
				}
				else
				{
					cur_state = psf();
				}
				if (prev_state != cur_state)
				{
#if DEBUGSTATE
					p_v = SecureZeroMemory((PVOID)sz_State_Transition,(SIZE_T)sizeof(sz_State_Transition));
					str_State_Transition.assign("State transition from ");
					str_State_Transition.append(lookup_state_name(prev_state));
					str_State_Transition.append(" to ");
					str_State_Transition.append(lookup_state_name(cur_state));
					i_i = ::MultiByteToWideChar(
						CP_ACP,
						MB_PRECOMPOSED,
						(LPCSTR)str_State_Transition.c_str(),
						str_State_Transition.length(),
						sz_State_Transition,
						sizeof(sz_State_Transition)/sizeof(TCHAR));
					i32_states = (cur_state<<16)+prev_state;
					SvcReportEvent(sz_State_Transition,(DWORD)i32_states,SVC_INFORMATION);
#endif
					if (Burner_control_status_Locked_out == cur_state)
					{
						dw_rc = Send_Svc_Sched_Report_Mail();
						p_v = SecureZeroMemory((PVOID)sz_State_Transition,(SIZE_T)sizeof(sz_State_Transition));
						str_State_Transition.assign(lookup_state_name(prev_state));
						str_State_Transition.append(" reporting burner lockout");
						i_i = ::MultiByteToWideChar(
							CP_ACP,
							MB_PRECOMPOSED,
							(LPCSTR)str_State_Transition.c_str(),
							str_State_Transition.length(),
							sz_State_Transition,
							sizeof(sz_State_Transition)/sizeof(TCHAR));
							SvcReportEvent(sz_State_Transition,(DWORD)cur_state,SVC_INFORMATION);
					}
					prev_state = cur_state;
				}
				if (!i32_Notice_Counter)
				{
					dw_rc = Send_Svc_Sched_Report_Mail();
					SvcReportEvent(_T("Reporting burner lockout condition"),(DWORD)cur_state,SVC_INFORMATION);
					i32_Notice_Counter = i32_Notice_Interval;
#if DEBUGNOTICETIMER
					SvcReportEvent(_T("Notification timer"),(DWORD)cur_state,SVC_INFORMATION);
#endif
				}
				if (0 > i32_Notice_Counter)
				{
					i32_Notice_Counter = i32_Notice_Interval;
				}
			}
		}
	}

/* Service stopping */
	for ( i_i = 0; i_i < sizeof(h_Events)/sizeof(HANDLE); i_i++ )
	{
		if ( NULL != h_Events[i_i] )
		{
			b_r = CloseHandle(h_Events[i_i]);
			h_Events[i_i] = NULL;
		}
	}
	if ( NULL != h_send_timer )
	{
		b_r = CancelWaitableTimer(h_send_timer);
		b_r = CloseHandle(h_send_timer);
		h_send_timer = NULL;
	}
	if ( NULL != h_poll_timer )
	{
		b_r = CancelWaitableTimer(h_poll_timer);
		b_r = CloseHandle(h_poll_timer);
		h_poll_timer = NULL;
	}
	if ( NULL != st_now_time )
	{
		delete st_now_time;
		st_now_time = NULL;
	}
	if ( NULL != st_send_time )
	{
		delete st_send_time;
		st_send_time = NULL;
	}
	if ( ERROR_SUCCESS != dw_rc )
	{
		b_r = SetEvent(g_h_Monitor_Fail_Event);
	}
	return dw_rc;
}

enum Burner_Control_Status_State_Values Entry_state_fn(void)
{
	return Not_Connected;
}

enum Burner_Control_Status_State_Values Not_Connected_state_fn(void)
{
	if (!g_bSolaConnected)
	{
		i32_Notice_Counter--;
		return Not_Connected;
	}
	return (enum Burner_Control_Status_State_Values)pcBurnerControlStatus->GetValue((unsigned short)0x0020);
}

enum Burner_Control_Status_State_Values Disabled_state_fn(void)
{
	i32_Notice_Counter--;
	return (enum Burner_Control_Status_State_Values)pcBurnerControlStatus->GetValue((unsigned short)0x0020);
}

enum Burner_Control_Status_State_Values  Locked_out_state_fn(void)
{
	i32_Notice_Counter--;
	return (enum Burner_Control_Status_State_Values)pcBurnerControlStatus->GetValue((unsigned short)0x0020);
}

enum Burner_Control_Status_State_Values  Anti_short_cycle_state_fn(void)
{
	return (enum Burner_Control_Status_State_Values)pcBurnerControlStatus->GetValue((unsigned short)0x0020);
}

enum Burner_Control_Status_State_Values  Unconfigured_safety_data_state_fn(void)
{
	i32_Notice_Counter--;
	return (enum Burner_Control_Status_State_Values)pcBurnerControlStatus->GetValue((unsigned short)0x0020);
}

enum Burner_Control_Status_State_Values  Standby_Hold_state_fn(void)
{
	i32_Notice_Counter--;
	return (enum Burner_Control_Status_State_Values)pcBurnerControlStatus->GetValue((unsigned short)0x0020);
}

enum Burner_Control_Status_State_Values  Standby_Delay_state_fn(void)
{
	i32_Notice_Counter--;
	return (enum Burner_Control_Status_State_Values)pcBurnerControlStatus->GetValue((unsigned short)0x0020);
}

enum Burner_Control_Status_State_Values  Normal_Standby_state_fn(void)
{
	uint16_t ui16_Demand_Source;
	ui16_Demand_Source = pcTrendStatus->GetValue((unsigned short)0x0006);
	if ((ui16_Demand_Source != ui16_No_source_demand) && (ui16_Demand_Source != ui16_Warm_weather_shutdown))
	{
		i32_Notice_Counter--;
	}
	if ((ui16_Demand_Source == ui16_No_source_demand) || (ui16_Demand_Source == ui16_Warm_weather_shutdown))
	{
		i32_Notice_Counter = i32_Notice_Interval;
	}
	return (enum Burner_Control_Status_State_Values)pcBurnerControlStatus->GetValue((unsigned short)0x0020);
}

enum Burner_Control_Status_State_Values  Preparing_state_fn(void)
{
	i32_Notice_Counter--;
	return (enum Burner_Control_Status_State_Values)pcBurnerControlStatus->GetValue((unsigned short)0x0020);
}

enum Burner_Control_Status_State_Values  Ignition_state_fn(void)
{
	i32_Notice_Counter--;
	return (enum Burner_Control_Status_State_Values)pcBurnerControlStatus->GetValue((unsigned short)0x0020);
}

enum Burner_Control_Status_State_Values  Firing_state_fn(void)
{
	i32_Notice_Counter = i32_Notice_Interval;
	return (enum Burner_Control_Status_State_Values)pcBurnerControlStatus->GetValue((unsigned short)0x0020);
}

enum Burner_Control_Status_State_Values  Postpurge_state_fn(void)
{
	return (enum Burner_Control_Status_State_Values)pcBurnerControlStatus->GetValue((unsigned short)0x0020);
}

func_ptr *lookup_state_fn(enum Burner_Control_Status_State_Values sv)
{
	int i_i;
	for (i_i = 0; i_i < sizeof(sd_descriptors)/sizeof(state_descriptor); i_i++)
	{
		if (sv == sd_descriptors[i_i].sv)
		{
			return sd_descriptors[i_i].fp;
		}
	}
	return NULL;
}

char const *lookup_state_name(enum Burner_Control_Status_State_Values sv)
{
	int i_i;
	for (i_i = 0; i_i < sizeof(sd_descriptors)/sizeof(state_descriptor); i_i++)
	{
		if (sv == sd_descriptors[i_i].sv)
		{
			return sd_descriptors[i_i].state_name;
		}
	}
	return NULL;
}
