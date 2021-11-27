#include "stdafx.h"
#include "SolaMonitorSvc.h"

const char* KeyMailServer = "MailServer";
const char* MailServerValueName = "ServerName";
const char* KeyMailSender = "MailSender";
const char* MailSenderValueName = "SenderName";
const char* MailSenderReplyToValueName = "ReplyTo";
const char* KeyAccountInfo = "AccountInfo";
const char* AccountInfoUserValueName = "UserName";
const char* AccountInfoPasswordValueName = "Password";
const char* AccountInfoAuthValueName = "UseAuthentication";
const char* AccountInfoEncryptionValueName = "Encryption";
const char* KeyRecipients = "Recipients";
const char* RecipientsPrefixValueName = "Recipient";
const char* KeyBoilers = "Boilers";
const char* BoilersPrefixValueName = "Modbus";
const char* KeySchedule = "Schedule";
const char* ScheduledSendValueName = "ScheduledSend";
const char* ScheduleDaysValueName = "SchedDays";
const char* ScheduleTimeHoursValueName = "TimeHours";
const char* ScheduleTimeMinutesValueName = "TimeMinutes";
const char* ScheduleSengLogsValueName = "SendLogs";
const char* ScheduleANHoursValueName = "NoticeHours";
const char* ScheduleANMinutesValueName = "NoticeMinutes";
const char* KeyCOMSetup = "COMSetup";
const char* COMSetupCOMPortValueName = "COMPort";
const char* COMSetupPollIntervalHoursValueName = "PollIntervalHours";
const char* COMSetupPollIntervalMinutesValueName = "PollIntervalMinutes";
const char* COMSetupPollIntervalSecondsValueName = "PollIntervalSeconds";

static const INIFILEVALUES IniFileContents[] = {
KeyMailServer, MailServerValueName, Character, IDC_EDITMAILSERVER,
KeyMailSender, MailSenderValueName, Character, IDC_EDITMAILSENDER,
KeyMailSender, MailSenderReplyToValueName, Character, IDC_EDITREPLYTO,
KeyAccountInfo, AccountInfoUserValueName, Character, IDC_EDITUSERNAME,
KeyAccountInfo, AccountInfoPasswordValueName, Character, IDC_EDITPASSWORD,
KeyAccountInfo, AccountInfoAuthValueName, Boolean, IDC_CHKUSEAUTHENT,
KeyAccountInfo, AccountInfoEncryptionValueName, Encryption, IDC_ENCRYPTION,
KeyRecipients, RecipientsPrefixValueName, CharacterList, IDC_LISTRECIPIENTS,
KeyBoilers, BoilersPrefixValueName, DecimalList, IDC_LISTMBADDRESSES,
KeySchedule, ScheduledSendValueName, Boolean, IDC_CHKSNDSCHED,
KeySchedule, ScheduleDaysValueName, Decimal, IDC_EDITDAYS,
KeySchedule, ScheduleTimeHoursValueName, Decimal, IDC_EDITHRS,
KeySchedule, ScheduleTimeMinutesValueName, Decimal, IDC_EDITMINS,
KeySchedule, ScheduleSengLogsValueName, Boolean, IDC_CHKSNDLOGS,
KeySchedule, ScheduleANHoursValueName, Decimal, IDC_EDITNOTICEHRS,
KeySchedule, ScheduleANMinutesValueName, Decimal, IDC_EDITNOTICEMINS,
KeyCOMSetup, COMSetupCOMPortValueName, DecimalSelection, IDC_LISTSELECTCOM,
KeyCOMSetup, COMSetupPollIntervalHoursValueName, Decimal, IDC_EDITPOLLHRS,
KeyCOMSetup, COMSetupPollIntervalMinutesValueName, Decimal, IDC_EDITPOLLMINS,
KeyCOMSetup, COMSetupPollIntervalSecondsValueName, Decimal, IDC_EDITPOLLSECS
};

static const int g_ifcSize = sizeof(IniFileContents)/sizeof(INIFILEVALUES);

void* LoadIniFileValues(CIniFile* lpFile,int i_d)
{
	TCHAR szStrValue[512];
	TCHAR *p_sv;
	std::list<int> *p_ldv;
	int *p_dv;
	BOOL *p_bv;
	std::list<std::wstring> *p_lwsv;
	string vname;
	string vStr;
	BOOL bSuccess = true;
	BOOL bResult;
	UINT uiResult;
	UINT uiValueCnt;
	PVOID pv;
	int nResult;
	int i;
	long lValueID;
	LRESULT lRes;
	UINT uiKey;
	size_t st_len;
	HRESULT hres_r;

	for ( int ndx = 0; ndx < g_ifcSize; ndx++ )
	{
		if ( IniFileContents[ndx].Identifier == i_d )
		{
			uiKey = (UINT)lpFile->FindKey(IniFileContents[ndx].KeyName);
			switch (IniFileContents[ndx].ValueType)
			{
			case Character:
				lValueID = lpFile->FindValue(uiKey,IniFileContents[ndx].ValueName);
				pv = ::SecureZeroMemory((PVOID)szStrValue,sizeof(szStrValue));
				nResult = ::MultiByteToWideChar(CP_ACP,
					MB_PRECOMPOSED,
					lpFile->GetValue(uiKey,lValueID).c_str(),
					lpFile->GetValue(uiKey,lValueID).size(),
					szStrValue,
					sizeof(szStrValue)/sizeof(TCHAR));
				hres_r = StringCchLength(szStrValue,sizeof(szStrValue)/sizeof(TCHAR),&st_len);
				try
				{
					p_sv = (TCHAR*)new TCHAR[st_len+1];
				}
				catch(std::bad_alloc &ba)
				{
					throw ba;
				}
				pv = ::SecureZeroMemory((PVOID)p_sv,sizeof(TCHAR)*(st_len+1));
				hres_r = StringCchPrintf(p_sv,st_len+1,_T("%s"),szStrValue);
				return (void*)p_sv;
			case Decimal:
				try
				{
					p_dv = (int*)new int;
				}
				catch(std::bad_alloc &ba)
				{
					throw ba;
				}
				lValueID = lpFile->FindValue(uiKey,IniFileContents[ndx].ValueName);
				uiResult = (UINT)lpFile->GetValueI(IniFileContents[ndx].KeyName,IniFileContents[ndx].ValueName);
				*p_dv = (int)uiResult;
				return (void*)p_dv;
			case Boolean:
				try
				{
					p_bv = (BOOL*)new BOOL;
				}
				catch(std::bad_alloc &ba)
				{
					throw ba;
				}
				*p_bv = (lpFile->GetValueB(IniFileContents[ndx].KeyName,IniFileContents[ndx].ValueName)) ? true : false;
				return (void*)p_bv;
			case CharacterList:
				try
				{
					p_lwsv = (std::list<std::wstring>*)new std::list<std::wstring>;
				}
				catch(std::bad_alloc &ba)
				{
					throw ba;
				}
				uiValueCnt = lpFile->GetNumValues(uiKey);
				for ( i = 0; i < uiValueCnt; i++ )
				{
					vname = lpFile->ValueName(uiKey,(UINT)i);
					vStr = lpFile->GetValue(IniFileContents[ndx].KeyName,vname);
					pv = ::SecureZeroMemory((PVOID)szStrValue,sizeof(szStrValue));
					nResult = ::MultiByteToWideChar(CP_ACP,
						MB_PRECOMPOSED,
						vStr.c_str(),
						vStr.size(),
						szStrValue,
						sizeof(szStrValue)/sizeof(TCHAR));
					p_lwsv->push_back(std::wstring(szStrValue));
				}
				return (void*)p_lwsv;
			case DecimalList:
				try
				{
					p_ldv = (std::list<int>*)new std::list<int>;
				}
				catch(std::bad_alloc &ba)
				{
					throw ba;
				}
				uiValueCnt = lpFile->GetNumValues(uiKey);
				for ( i = 0; i < uiValueCnt; i++ )
				{
					vname = lpFile->ValueName(uiKey,(UINT)i);
					vStr = lpFile->GetValue(IniFileContents[ndx].KeyName,vname);
					pv = ::SecureZeroMemory((PVOID)szStrValue,sizeof(szStrValue)/sizeof(TCHAR));
					nResult = ::MultiByteToWideChar(CP_ACP,
						MB_PRECOMPOSED,
						vStr.c_str(),
						vStr.size(),
						szStrValue,
						sizeof(szStrValue)/sizeof(TCHAR));
					p_ldv->push_back(_wtoi(szStrValue));
				}
				return (void*)p_ldv;
			case DecimalSelection:
				try
				{
					p_dv = (int*)new int;
				}
				catch(std::bad_alloc &ba)
				{
					throw ba;
				}
				uiResult = (UINT)lpFile->GetValueI(IniFileContents[ndx].KeyName,IniFileContents[ndx].ValueName);
				*p_dv = (int)uiResult;
				return (void*)p_dv;
			case Encryption:
				try
				{
					p_dv = (int*)new int;
				}
				catch(std::bad_alloc &ba)
				{
					throw ba;
				}
				uiResult = (UINT)lpFile->GetValueI(IniFileContents[ndx].KeyName,IniFileContents[ndx].ValueName);
				*p_dv = (int)uiResult;
				return (void*)p_dv;
			default:
				break;
			}
		}
	}
	return NULL;
}
