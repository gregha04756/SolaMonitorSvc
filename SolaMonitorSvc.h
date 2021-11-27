#pragma once

using namespace std;

#include "SolaLockout.h"
#include "SolaLockoutDesc.h"
#include "SolaMultiValue.h"
#include "SolaMBMap.h"
#include "SolaMBMaps.h"
#include "SolaAlert.h"
#include "MailAcctInfo.h"
#include "SolaChangeCode.h"
#include "SolaPage.h"

#define SVCNAME _T("SolaMonitorSvc")
#define MBCRCSIZE 2
#define SOLAREADTIMEOUT 5000 /* Sola disconnected if no data read for # of milliseconds */
#define MAX_LOADSTRING 100
#define REQBUFSIZ 512
#define RESPBUFSIZ 512
#define UNCONFIGUREDTEMP 0x8FFF
#define OUTSIDELOWRANGE 0x8400
#define CELSIUSUNITS 1
#define FAHRENHEITUNITS 0

#define IDC_EDITMAILSERVER			301
#define IDC_EDITUSERNAME			302
#define IDC_EDITPASSWORD			303
#define IDC_LISTRECIPIENTS			304
#define IDC_CHKUSEAUTHENT			305
#define IDC_BTNADDRECIPIENT			306
#define IDC_BTNRMVRECIPIENT			307
#define IDC_CHKSNDSCHED				308
#define IDC_CHKSNDLOGS				309
#define IDC_BTNTESTEMAIL			310
#define IDC_LISTSELECTCOM			311
#define IDC_EDITDAYS				312
#define IDC_EDITHRS					313
#define IDC_EDITMINS				314
#define IDC_EDITPOLLHRS				315
#define IDC_EDITPOLLMINS			316
#define IDC_EDITPOLLSECS			317
#define IDC_BTNTESTCOM				318
#define IDC_EDITRCPNT				319
#define IDC_EDITMAILSENDER			320
#define IDC_ENCRYPTION				321
#define IDC_LISTMBADDRESSES			322
#define IDC_BTNADDMODBUS			323
#define IDC_BTNRMVMODBUS			324
#define IDC_EDITMODBUS				325
#define IDC_EDITREPLYTO				326
#define IDC_EDITNOTICEHRS			329
#define IDC_EDITNOTICEMINS			330

/* Define Burner Control Status state values */
enum Burner_Control_Status_State_Values {
	Burner_control_status_Disabled = 0,
	Burner_control_status_Locked_out = 1,
	Burner_control_status_Reserved_02 = 2,
	Burner_control_status_Reserved_03 = 3,
	Burner_control_status_Anti_short_cycle = 4,
	Burner_control_status_Unconfigured_safety_data = 5,
	Burner_control_status_Reserved_06 = 6,
	Burner_control_status_Reserved_07 = 7,
	Burner_control_status_Reserved_08 = 8,
	Burner_control_status_Reserved_09 = 9,
	Burner_control_status_Reserved_10 = 10,
	Burner_control_status_Reserved_11 = 11,
	Burner_control_status_Reserved_12 = 12,
	Burner_control_status_Reserved_13 = 13,
	Burner_control_status_Reserved_14 = 14,
	Burner_control_status_Reserved_15 = 15,
	Burner_control_status_Reserved_16 = 16,
	Burner_control_status_Reserved_17 = 17,
	Burner_control_status_Reserved_18 = 18,
	Burner_control_status_Reserved_19 = 19,
	Burner_control_status_Reserved_20 = 20,
	Burner_control_status_Reserved_21 = 21,
	Burner_control_status_Reserved_22 = 22,
	Burner_control_status_Reserved_23 = 23,
	Burner_control_status_Reserved_24 = 24,
	Burner_control_status_Reserved_25 = 25,
	Burner_control_status_Reserved_26 = 26,
	Burner_control_status_Reserved_27 = 27,
	Burner_control_status_Reserved_28 = 28,
	Burner_control_status_Reserved_29 = 29,
	Burner_control_status_Reserved_30 = 30,
	Burner_control_status_Reserved_31 = 31,
	Burner_control_status_Reserved_32 = 32,
	Burner_control_status_Reserved_33 = 33,
	Burner_control_status_Standby_Hold = 34,
	Burner_control_status_Standby_Delay = 35,
	Burner_control_status_Reserved_36 = 36,
	Burner_control_status_Reserved_37 = 37,
	Burner_control_status_Reserved_38 = 38,
	Burner_control_status_Reserved_39 = 39,
	Burner_control_status_Reserved_40 = 40,
	Burner_control_status_Reserved_41 = 41,
	Burner_control_status_Reserved_42 = 42,
	Burner_control_status_Reserved_43 = 43,
	Burner_control_status_Reserved_44 = 44,
	Burner_control_status_Reserved_45 = 45,
	Burner_control_status_Reserved_46 = 46,
	Burner_control_status_Reserved_47 = 47,
	Burner_control_status_Normal_Standby = 48,
	Burner_control_status_Preparing = 49,
	Burner_control_status_Ignition = 50,
	Burner_control_status_Firing = 51,
	Burner_control_status_Postpurge = 52,
	Entry_State,
	Not_Connected
};

/* See Table 12 in Sola Modbus Interface Specification. Register 0x0021 contains this value */
enum Burner_Control_States_Values {
	Burner_control_State_Initiate = 0,
	Burner_control_State_Standby_Delay = 1,
	Burner_control_State_Standby = 2,
	Burner_Control_State_Safe_Startup = 3,
	Burner_Control_State_Prepurge_Drive_to_Purge_Rate = 4,
	Burner_Control_State_Prepurge_Measured_Purge_Time = 5,
	Burner_Control_State_Prepurge_Drive_to_Lightoff_Rate = 6,
	Burner_Control_State_Preignition_Test = 7,
	Burner_Control_State_Preignition_Time = 8,
	Burner_Control_State_Pilot_Flame_Establishing_Period = 9,
	Burner_Control_State_Main_Flame_Establishing_Period = 10,
	Burner_Control_State_Direct_Burner_Ignition = 11,
	Burner_Control_State_Run = 12,
	Burner_Control_State_Postpurge = 13,
	Burner_Control_State_Lockout = 14,
	Burner_Control_State_Prepurge_Fulton_pulse = 15,
	Burner_Control_State_Ignition_Fulton_pulse = 16,
	Burner_Control_State_Combustion_Pressure_Establish_Fulton_pulse = 17,
	Burner_Control_State_Combustion_Pressure_Stabilization_Fulton_pulse = 18,
	Burner_Control_State_Main_Flame_Stabilization_Fulton_pulse = 19,
	Burner_Control_State_Safety_Processor_Offline = 255
};

typedef enum Burner_Control_Status_State_Values (func_ptr)(void);

struct state_descriptor {char const *state_name;enum Burner_Control_Status_State_Values const sv;func_ptr const *fp;};
#if 0
/* Define stuff for state machine */
typedef int (func_ptr)(void);

/* State functions array and enum state_codes must be in sync! */
enum state_codes {
	entry,
	not_connected,
	nominal,
	hold,
	lockout,
	fertig}; /* fertig is German for finished */

enum ret_codes {
	ok,
	gohold,
	golockout,
	fail,
	repeat};

struct transition {
    enum state_codes src_state;
    enum ret_codes   ret_code;
    enum state_codes dst_state;
};
#endif

typedef enum __tagIniFileValueType {
	Character,
	Decimal,
	Boolean,
	Encryption,
	CharacterList,
	DecimalList,
	DecimalSelection
}INIFILEVALUETYPE;

typedef struct __tagINIFileValue {
	const char* KeyName;
	const char* ValueName;
	INIFILEVALUETYPE ValueType;
	int Identifier;
}INIFILEVALUES,*LPINIFILEVALUES;

typedef struct __tagMailSendParms {
	std::list<string> *p_msg;
	CMailAcctInfo* lpMAI;
}MAILSENDPARMS,*LPMAILSENDPARMS;

typedef struct __tagMonitorThreadParms {
/*	CMailAcctInfo* lpMAI;*/
	CIniFile* p_INI;
	DWORD dw_tid;
	HANDLE hdup_th;
	DWORD dw_err;
}MONITORTHREADPARMS,*LPMONITORTHREADPARMS;

typedef struct __tagMBSndRcvReq {	HWND hPage;
									UINT nMsg;
									unsigned char* pchSndBuf;
									unsigned char** ppchToSnd;
									unsigned char** ppchEndSnd;
									int nSndBufSize;
									unsigned char* pchRcvBuf;
									unsigned char** ppchToRcv;
									unsigned char** ppchEndRcv;
									int nRcvBufSize;} MBSNDRCVREQ, *LPMBSNDRCVREQ;

typedef struct __tagHoldingRegister {	unsigned char uchDeviceFuncCode[3];
										unsigned char uchValueHi;
										unsigned char uchValueLo; } HOLDINGREG,*LPHOLDINGREG;

										typedef struct __tagSolaHRResponse {	unsigned char uchAddress;
										unsigned char uchFuncCode;
										unsigned char cbByteCount; } SOLAHRRESPONSE,*LPSOLAHRRESPONSE;

typedef struct __tagSolaIDResponse {	unsigned char SolaAddr;
										unsigned char FunctionCode;
										unsigned char ByteCount;
										unsigned char SolaID;
										unsigned char RunIndicator;
										char OSNumber[16];
										char BurnerName[20]; } SOLAIDRESPONSE,*LPSOLAIDRESPONSE;

enum MBConnType {RTU,
 TCP};

enum RTUCommResult {NoError,
 Timeout,
 HardError,
 CRCError};

extern "C++" CSolaMBMap* pcCHConfiguration;
extern "C++" CSolaMBMap* pcXCHConfig;
extern "C++" CSolaMBMap* pcModConfiguration;
extern "C++" CSolaMBMap* pcSystemIDProductType;
extern "C++" CSolaMBMap* pcSystemIDBurnerName;
extern "C++" CSolaMBMap* pcSystemIDInstallationData;
extern "C++" CSolaMBMap* pcSystemIDOEMID;
extern "C++" CSolaMBMap* pcSystemIDOSNumber;
extern "C++" CSolaMBMap* pcSystemIDDateCode;
extern "C++" CSolaMBMap* pcSystemIDSafetyProcesorBuild;
extern "C++" CSolaMBMap* pcSystemIDAppProcessorBuild;
extern "C++" CSolaMBMap* pcODResetConfig;
extern "C++" CSolaMBMap* pcDHWConfiguration;
extern "C++" CSolaMBMap* pcLLStatus;
extern "C++" CSolaMBMap* pcXLLStatus;
extern "C++" CSolaMBMap* pcLLConfig;
extern "C++" CSolaMBMap* pcXSystemConfig;
extern "C++" CSolaMBMap* pcBurnerControlStatus;
extern "C++" CSolaMBMap* pcTrendStatus;
extern "C++" CSolaMBMap* pcSystemStatus;
extern "C++" CSolaMBMap* pcSensorStatus;
extern "C++" CSolaMBMap* pcDemandModulationStatus;
extern "C++" CSolaMBMap* pcCHStatus;
extern "C++" CSolaMBMap* pcDHWStatus;
extern "C++" CSolaMBMap* pcPumpStatus;
extern "C++" CSolaMBMap* pcSystemConfiguration;
extern "C++" CSolaMBMap* pcSystemIDProductType;
extern "C++" CSolaMBMap* pcSystemIDBurnerName;
extern "C++" CSolaMBMap* pcSystemIDInstallationData;
extern "C++" CSolaMBMap* pcSystemIDOEMID;
extern "C++" CSolaMBMap* pcSystemIDOSNumber;
extern "C++" CSolaMBMap* pcSystemIDDateCode;
extern "C++" CSolaMBMap* pcSystemIDSafetyProcesorBuild;
extern "C++" CSolaMBMap* pcSystemIDAppProcessorBuild;
extern "C++" CSolaMBMap* pcAlarmCode;
extern "C++" CSolaMBMap* pcStatistics;
extern "C++" CSolaMBMaps* pcTrendMaps;
extern "C++" CSolaMBMaps* pcAllSolaMaps;
extern "C++" CSolaMBMaps* pcSystemIDMaps;
extern "C++" CSolaMBMap* pcAnnuncConfigGen;
extern "C++" CSolaMBMap* pcAnnuncConfig1;
extern "C++" CSolaMBMap* pcAnnuncConfig2;
extern "C++" CSolaMBMap* pcAnnuncConfig3;
extern "C++" CSolaMBMap* pcAnnuncConfig4;
extern "C++" CSolaMBMap* pcAnnuncConfig5;
extern "C++" CSolaMBMap* pcAnnuncConfig6;
extern "C++" CSolaMBMap* pcAnnuncConfig7;
extern "C++" CSolaMBMap* pcAnnuncConfig8;
extern "C++" CSolaMBMap* pcPIIAnnuncConfig;
extern "C++" CSolaMBMap* pcLCIAnnuncConfig;
extern "C++" CSolaMBMap* pcILKAnnuncConfig;
extern "C++" CSolaMultiValue* pcBurnerControlStateValues;
extern "C++" CSolaMultiValue* pcBurnerControlStatusValues;
extern "C++" CSolaLockout* pcLockoutLog;
extern "C++" CSolaAlert* pcAlertLog;
extern "C++" CSolaChangeCode* pcConfigChangeCodes;
extern "C++" CSolaChangeCode* pcStatusChangeCodes;
extern "C++" CSolaPage* pcSummaryPage;

extern "C++" MBConnType mbctMBConn;
extern "C++" LPSOLAIDRESPONSE g_lpSolaID;
extern "C++" std::queue<MBSNDRCVREQ> g_MBSndRcvReqQ;

extern "C++" HANDLE g_hCOM;
extern "C++" BOOL g_bSolaConnected;
extern "C++" unsigned char g_ucSolaMBAddress;
extern "C++" HANDLE ghSvcStopEvent;
extern "C++" HANDLE ghWRThreadFailEvent;
extern "C++" HANDLE g_h_Monitor_Fail_Event;
extern "C++" HANDLE g_hWriteCompletedEvent;
extern "C++" HANDLE g_hReadCompletedEvent;
extern "C++" CRITICAL_SECTION gCOMCritSect;
extern "C++" CRITICAL_SECTION gRWDataCritSect;
extern "C++" HANDLE g_h1SecTimer;
extern "C++" BOOL g_bQuit;
extern "C++" OVERLAPPED g_oWOverlap;
extern "C++" OVERLAPPED g_oROverlap;
extern "C++" HANDLE g_hWEvents[3];
extern "C++" HANDLE g_hREvents[3];
extern "C++" unsigned char* g_pMBRequest;
extern "C++" unsigned char* g_pMBResponse;
extern "C++" DWORD g_dwTotalRcvd;
extern "C++" DWORD g_dwTotalSent;
extern "C++" DWORD g_dwTotalCRCErrors;
extern "C++" CMailAcctInfo *p_MAI;
extern "C++" const int g_ifcSize;
extern "C++" const INIFILEVALUES IniFileContents[];
extern "C++" const char *KeyBoilers;
extern "C++" int const i_24hr;

extern "C++" BOOL InitSolaDatabase();
extern "C++" DWORD WINAPI CommThread(LPVOID lpParam);
extern "C++" DWORD OpenCOMPort(HWND hParentWnd,int nCommPort,HANDLE &hCOM);
extern "C++" unsigned short calc_CRC16(unsigned char* puchMsg,unsigned short usDataLen);
extern "C++" int check_CRC16(unsigned char *buf,int buflen);
extern "C++" VOID SvcReportEvent(LPTSTR szFunction,DWORD dwError,DWORD dwIdenifier);
extern "C++" DWORD WINAPI MailSendFunc(LPVOID lpParam);
extern "C++" void* LoadIniFileValues(CIniFile* lpFile,int i_d);
extern "C++" CMailAcctInfo* Setup_Mail_Scct_Info(CIniFile *p_INI);
extern "C++" BOOL Send_Svc_Start_Mail(void);
extern "C++" HANDLE Start_Monitor_Thread(LPMONITORTHREADPARMS p_m);
extern "C++" DWORD WINAPI Monitor_Thread_Fn(LPVOID lParam);
extern "C++" BOOL Mail_Report_Is_Scheduled(CIniFile* p_INI);
extern "C++" DWORD Get_Schedule_Times(CIniFile *p_INI,int &i_d,int &i_h,int &i_m);
extern "C++" DWORD Get_Polling_Time(CIniFile *p_INI,int &i_d,int &i_h,int &i_m);
extern "C++" DWORD Get_Notice_Time(CIniFile *p_INI,int &i_h,int &i_m);
extern "C++" std::wstring* Format_System_Time(std::wstring const &ws_pre,std::list<LPSYSTEMTIME>::iterator p_s);
extern "C++" DWORD Calc_LI_Time(LPSYSTEMTIME p_st_now,LARGE_INTEGER *p_li_send_time,LPSYSTEMTIME p_st_send,int &d,int &h,int &m);
extern "C++" DWORD Calc_LI_Time(LARGE_INTEGER *p_li_delta,LARGE_INTEGER *p_li_send_time,LPSYSTEMTIME p_st_send);
extern "C++" DWORD Send_Svc_Sched_Report_Mail(void);
extern "C++" LARGE_INTEGER Calc_LI_Rel_TIme(int i_days,int i_hours,int i_minutes,int i_seconds);
extern "C++" DWORD Prep_Report(std::list<string> *p_ls);
extern "C++" RTUCommResult MBWriteRead(HANDLE& hCOM,unsigned char* mbmsg,int cbmsg,unsigned char* reply,int cbreply,LPDWORD lpreplylen);
extern "C++" extern "C++" RTUCommResult MBRead(HANDLE& hCOM,unsigned char* reply,int cbreply,LPDWORD replylen);
extern "C++" DWORD Assemble_The_Report(std::list<string> *p_ls);
extern "C++" enum state_codes lookup_transitions(enum state_codes stcd,int const code);
extern "C++" LONG Compare_Two_System_Times(LPSYSTEMTIME const lp_st1,LPSYSTEMTIME const lp_st2);