#include "stdafx.h"
#include "SolaMonitorSvc.h"
#include "MailAcctInfo.h"
#include "SMSvcMsg.h"
#include "SolaPage.h"

using namespace std;

const char *ch_Svc_Report = "Sola Monitoring Service reporting at ";
const char *ch_Unconfig = "UNCONFIGURED ";

float TempVal (BOOL units,short temp)
{
	return ((((9.0*((float)temp))/50.0)+32.0)*(units == FAHRENHEITUNITS))+((((float)temp)/10.0)*(units == CELSIUSUNITS));
}

short ssTempVal (BOOL units, short temp)
{
	return ((((9*temp)/50)+32)*(units == FAHRENHEITUNITS))+(((temp)/10)*(units == CELSIUSUNITS));
}

float HystVal (BOOL units,short temp)
{
	if ( units == FAHRENHEITUNITS )
	{
		return (9.0*((float)temp))/50.0;
	}
	return ((float)temp)/10.0;
}

signed short SolaTempVal (BOOL units,short temp)
{
	return (signed short)(10*((((5.0*((float)temp-32.0))/9.0)*(units == FAHRENHEITUNITS)) + ((float)temp*(units == CELSIUSUNITS))));
}

signed short SolaHystVal (BOOL units,short temp)
{
	return (signed short)(10*((((5.0*(float)temp)/9.0)*(units == FAHRENHEITUNITS)) + ((float)temp*(units == CELSIUSUNITS))));
}

DWORD Prep_Report(std::list<string> *p_ls)
{
	int i_r;
	PVOID p_v;
	DWORD dw_rc;
	SYSTEMTIME sys_time;
	char msg_line[80];
	TCHAR sz_msg_line[sizeof(msg_line)];

	dw_rc = ERROR_SUCCESS;
	p_ls->push_back(ch_Svc_Report);
	p_v = SecureZeroMemory((PVOID)msg_line,sizeof(msg_line));
	p_v = SecureZeroMemory((PVOID)sz_msg_line,sizeof(sz_msg_line));
	GetLocalTime(&sys_time);
	i_r = sprintf_s(msg_line,sizeof(msg_line),"%-4d-%02d-%02d %02d:%02d",sys_time.wYear,sys_time.wMonth,sys_time.wDay,sys_time.wHour,sys_time.wMinute);
	p_ls->push_back(msg_line);
	if ( g_bSolaConnected )
	{
		p_ls->push_back(g_lpSolaID->BurnerName);
		p_ls->push_back("is connected");
		dw_rc = Assemble_The_Report(p_ls);
	}
	else
	{
		p_ls->push_back(std::string("Sola is not connected"));
	}
	return dw_rc;
}

DWORD Assemble_The_Report(std::list<string> *p_ls)
{
	float flDecimal2;
	int hh;
	int mm;
	int ss;
	int i;
	int i_r;
	LRESULT l_r;
	PVOID p_v;
	DWORD dw_rc;
	char report_line[100];
	char value_string[80];
	TCHAR sz_report_line[sizeof(report_line)];
	TCHAR sz_value_string[sizeof(value_string)];
	HRESULT hr_rc;
	dw_rc = ERROR_SUCCESS;

			for ( i = 0; !g_bQuit && i < pcSummaryPage->GetSize(); i++ )
			{
				p_v = SecureZeroMemory((PVOID)sz_report_line,sizeof(sz_report_line));
				p_v = SecureZeroMemory((PVOID)report_line,sizeof(report_line));
				p_v = SecureZeroMemory((PVOID)value_string,sizeof(value_string));
				if ( NULL != pcSummaryPage->ItemLabel(i) )
				{
					hr_rc = StringCchPrintf(sz_report_line,sizeof(sz_report_line)/sizeof(TCHAR),_T("%s-> "),pcSummaryPage->ItemLabel(i));
					i_r = WideCharToMultiByte(CP_ACP,
						WC_COMPOSITECHECK,
						sz_report_line,
						sizeof(sz_report_line)/sizeof(TCHAR),
						report_line,
						sizeof(report_line),
						NULL,
						NULL);
				}
				switch (pcSummaryPage->ItemMap(i)->GetType(pcSummaryPage->ItemIndex(i)))
				{
				case Temperature:
					if ( pcSummaryPage->ItemMap(i)->GetValue(pcSummaryPage->ItemIndex(i)) == (signed short)UNCONFIGUREDTEMP )
					{
						i_r = strcat_s(report_line,sizeof(report_line),ch_Unconfig);
					}
					else
					{
						if ( pcSummaryPage->ItemMap(i)->GetValue(pcSummaryPage->ItemIndex(i)) > 1300 )
						{
							i_r = strcat_s(report_line,sizeof(report_line),ch_Unconfig);
						}
						else
						{
							i_r = sprintf_s(value_string,sizeof(value_string),"%-.2f",
								TempVal(pcSystemConfiguration->GetValue((int)0),pcSummaryPage->ItemMap(i)->GetValue(pcSummaryPage->ItemIndex(i))));
							if ( CELSIUSUNITS == pcSystemConfiguration->GetValue(0) )
							{
								i_r = sprintf_s(value_string,sizeof(value_string),"%-.1f",
									TempVal(pcSystemConfiguration->GetValue((int)0),pcSummaryPage->ItemMap(i)->GetValue(pcSummaryPage->ItemIndex(i))));
							}
							i_r = strcat_s(report_line,sizeof(report_line),value_string);
						}
					}
					p_ls->push_back(report_line);
					break;
				case ODTemperature:
					/* Determine OD temperature source */
					switch (pcXSystemConfig->GetValue((int)0))
					{
					case 0: /* Unconfigured OD temperature source*/
						i_r = strcat_s(report_line,sizeof(report_line),ch_Unconfig);
						break;
					case 1: /* S5 connector */
						if ( pcSummaryPage->ItemMap(i)->GetValue(pcSummaryPage->ItemIndex(i)) == (signed short)UNCONFIGUREDTEMP )
						{
							i_r = strcat_s(report_line,sizeof(report_line),ch_Unconfig);
						}
						else
						{
							if ( pcSummaryPage->ItemMap(i)->GetValue(pcSummaryPage->ItemIndex(i)) > 1300 )
							{
								i_r = strcat_s(report_line,sizeof(report_line),"");
							}
							else
							{
								i_r = sprintf_s(value_string,sizeof(value_string),"%-.2f",
									TempVal(pcSystemConfiguration->GetValue((int)0),pcSummaryPage->ItemMap(i)->GetValue(pcSummaryPage->ItemIndex(i))));
								if ( CELSIUSUNITS == pcSystemConfiguration->GetValue(0) )
								{
									i_r = sprintf_s(value_string,sizeof(value_string),"%-.1f",
										TempVal(pcSystemConfiguration->GetValue((int)0),pcSummaryPage->ItemMap(i)->GetValue(pcSummaryPage->ItemIndex(i))));
								}
							}
							i_r = strcat_s(report_line,sizeof(report_line),value_string);
						}
						break;
					case 2: /* S10 connector */
						break;
					case 3: /* Modbus */
						break;
					case 4: /* EnviraCOM sensor */
						break;
					case 5: /* C7089 sensor on S10 connector */
						break;
					default:
						break;
					}
					p_ls->push_back(report_line);
					break;
				case TemperatureSetpoint:
					if ( pcSummaryPage->ItemMap(i)->GetValue(pcSummaryPage->ItemIndex(i)) == (signed short)UNCONFIGUREDTEMP )
					{
						i_r = strcat_s(report_line,sizeof(report_line),ch_Unconfig);
					}
					else
					{
						i_r = sprintf_s(value_string,sizeof(value_string),"%-.2f",
							TempVal(pcSystemConfiguration->GetValue((int)0),pcSummaryPage->ItemMap(i)->GetValue(pcSummaryPage->ItemIndex(i))));
						if ( CELSIUSUNITS == pcSystemConfiguration->GetValue(0) )
						{
							i_r = sprintf_s(value_string,sizeof(value_string),"%-.1f",
								TempVal(pcSystemConfiguration->GetValue((int)0),pcSummaryPage->ItemMap(i)->GetValue(pcSummaryPage->ItemIndex(i))));
						}
						i_r = strcat_s(report_line,sizeof(report_line),value_string);
					}
					p_ls->push_back(report_line);
					break;
				case Hysteresis:
					i_r = sprintf_s(value_string,sizeof(value_string),"%-.2f",
					HystVal(pcSystemConfiguration->GetValue((int)0),pcSummaryPage->ItemMap(i)->GetValue(pcSummaryPage->ItemIndex(i))));
					if ( CELSIUSUNITS == pcSystemConfiguration->GetValue(0) )
					{
						i_r = sprintf_s(value_string,sizeof(value_string),"%-.1f",
							HystVal(pcSystemConfiguration->GetValue((int)0),pcSummaryPage->ItemMap(i)->GetValue(pcSummaryPage->ItemIndex(i))));
					}
					i_r = strcat_s(report_line,sizeof(report_line),value_string);
					p_ls->push_back(report_line);
					break;
				case Numericvalue:
					i_r = sprintf_s(value_string,sizeof(value_string),"%d",
						pcSummaryPage->ItemMap(i)->GetValue(pcSummaryPage->ItemIndex(i)));
					i_r = strcat_s(report_line,sizeof(report_line),value_string);
					p_ls->push_back(report_line);
					break;
				case Decimal1pl:
					flDecimal2 = (float)pcSummaryPage->ItemMap(i)->GetValue(pcSummaryPage->ItemIndex(i));
					i_r = sprintf_s(value_string,sizeof(value_string),"%-6.1f", flDecimal2/10.0);
					i_r = strcat_s(report_line,sizeof(report_line),value_string);
					p_ls->push_back(report_line);
					break;
				case Decimal2pl:
					flDecimal2 = (float)pcSummaryPage->ItemMap(i)->GetValue(pcSummaryPage->ItemIndex(i));
					i_r = sprintf_s(value_string,sizeof(value_string),"%-6.2f", flDecimal2/10.0);
					i_r = strcat_s(report_line,sizeof(report_line),value_string);
					p_ls->push_back(report_line);
					break;
				case Fanspeed:
					{
						if ( OUTSIDELOWRANGE == (unsigned short)pcSummaryPage->ItemMap(i)->GetValue(pcSummaryPage->ItemIndex(i)) )
						{
							i_r = sprintf_s(value_string,sizeof(value_string),"LOW");
						}
						else
						{
							i_r = sprintf_s(value_string,sizeof(value_string),"%d", pcSummaryPage->ItemMap(i)->GetValue(pcSummaryPage->ItemIndex(i)));
						}
						i_r = strcat_s(report_line,sizeof(report_line),value_string);
						p_ls->push_back(report_line);
					}
					break;
				case SensorMultivalue:
					if ( pcSummaryPage->ItemMap(i)->GetLPMulti(pcSummaryPage->ItemIndex(i)) != NULL )
					{
						_ASSERT(pcSummaryPage->ItemMap(i)->GetLPMulti(pcSummaryPage->ItemIndex(i)) != NULL);
						if ( pcSummaryPage->ItemMap(i)->GetMultiItemValue(pcSummaryPage->ItemIndex(i),pcSummaryPage->ItemMap(i)->GetValue(pcSummaryPage->ItemIndex(i))) != 1 )
						{
							int usA = pcSummaryPage->ItemMap(i)->GetStartRegAddr(pcSummaryPage->ItemIndex(i));
							int nNdx = pcSummaryPage->ItemIndex(i);
							hr_rc = ::StringCchPrintf(sz_value_string,
								sizeof(sz_value_string)/sizeof(TCHAR),
								_T("%s"),
								pcSummaryPage->ItemMap(i)->GetMultiValueItem(nNdx,pcSummaryPage->ItemMap(i)->GetValue(nNdx)));
							i_r = WideCharToMultiByte(CP_ACP,
								WC_COMPOSITECHECK,
								sz_value_string,
								sizeof(sz_value_string)/sizeof(TCHAR),
								value_string,
								sizeof(value_string),
								NULL,
								NULL);
							i_r = strcat_s(report_line,sizeof(report_line),value_string);
							p_ls->push_back(report_line);
						}
					}
					break;
				case Multivalue:
					if ( pcSummaryPage->ItemMap(i)->GetLPMulti(pcSummaryPage->ItemIndex(i)) != NULL )
					{
						_ASSERT(pcSummaryPage->ItemMap(i)->GetLPMulti(pcSummaryPage->ItemIndex(i)) != NULL);
						int nV = pcSummaryPage->ItemMap(i)->GetValue(pcSummaryPage->ItemIndex(i));
						int nA = pcSummaryPage->ItemMap(i)->GetStartRegAddr(pcSummaryPage->ItemIndex(i));
						LPSOLAMULTIVALUE lpM = pcSummaryPage->ItemMap(i)->GetLPMulti(pcSummaryPage->ItemIndex(i));
						int nS = pcSummaryPage->ItemMap(i)->GetMultiListSize(pcSummaryPage->ItemIndex(i));
						TCHAR* szS = pcSummaryPage->ItemMap(i)->GetMultiValueItem(pcSummaryPage->ItemIndex(i),pcSummaryPage->ItemMap(i)->GetValue(pcSummaryPage->ItemIndex(i)));
						if ( szS )
						{
							hr_rc = ::StringCchPrintf(sz_value_string, sizeof(sz_value_string)/sizeof(TCHAR), _T("%s"),szS);
						}
						else
						{
							hr_rc = ::StringCchPrintf(sz_value_string, sizeof(sz_value_string)/sizeof(TCHAR), _T("%s"),_T("Unknown status"));
						}
						i_r = WideCharToMultiByte(CP_ACP,
							WC_COMPOSITECHECK,
							sz_value_string,
							sizeof(sz_value_string)/sizeof(TCHAR),
							value_string,
							sizeof(value_string),
							NULL,
							NULL);
						i_r = strcat_s(report_line,sizeof(report_line),value_string);
						p_ls->push_back(report_line);
					}
					break;
				case Alarmcode:
					{
						U16 ac = pcSummaryPage->ItemMap(i)->GetValue(pcSummaryPage->ItemIndex(i));
					}
					break;
				case Lockoutcode:
					if ( (pcSummaryPage->ItemMap(i)->GetValue(pcSummaryPage->ItemIndex(i)) != 0) &&
						(pcSummaryPage->ItemMap(i)->GetLPMulti(pcSummaryPage->ItemIndex(i)) != NULL) )
					{
						_ASSERT(pcSummaryPage->ItemMap(i)->GetLPMulti(pcSummaryPage->ItemIndex(i)) != NULL);
						i_r = (signed short)pcSummaryPage->ItemMap(i)->GetValue(pcSummaryPage->ItemIndex(i));
						TCHAR* szS = pcSummaryPage->ItemMap(i)->GetMultiValueItem(pcSummaryPage->ItemIndex(i),pcSummaryPage->ItemMap(i)->GetValue(pcSummaryPage->ItemIndex(i)));
						hr_rc = ::StringCchPrintf(sz_value_string,sizeof(sz_value_string)/sizeof(TCHAR),_T("LOCKOUT %d: %s"),i_r,szS);
						i_r = WideCharToMultiByte(CP_ACP,
							WC_COMPOSITECHECK,
							sz_value_string,
							sizeof(sz_value_string)/sizeof(TCHAR),
							value_string,
							sizeof(value_string),
							NULL,
							NULL);
						i_r = strcat_s(report_line,sizeof(report_line),value_string);
						p_ls->push_back(report_line);
					}
					break;
				case Holdcode:
					if ( (pcSummaryPage->ItemMap(i)->GetValue(pcSummaryPage->ItemIndex(i)) != 0) &&
						(pcSummaryPage->ItemMap(i)->GetLPMulti(pcSummaryPage->ItemIndex(i)) != NULL) )
					{
						_ASSERT(pcSummaryPage->ItemMap(i)->GetLPMulti(pcSummaryPage->ItemIndex(i)) != NULL);
						i_r = (signed short)pcSummaryPage->ItemMap(i)->GetValue(pcSummaryPage->ItemIndex(i));
						TCHAR* szS = pcSummaryPage->ItemMap(i)->GetMultiValueItem(pcSummaryPage->ItemIndex(i),pcSummaryPage->ItemMap(i)->GetValue(pcSummaryPage->ItemIndex(i)));
						hr_rc = ::StringCchPrintf(sz_value_string,sizeof(sz_value_string)/sizeof(TCHAR),_T("HOLD %d: %s"),i_r,szS);
						i_r = WideCharToMultiByte(CP_ACP,
							WC_COMPOSITECHECK,
							sz_value_string,
							sizeof(sz_value_string)/sizeof(TCHAR),
							value_string,
							sizeof(value_string),
							NULL,
							NULL);
						i_r = strcat_s(report_line,sizeof(report_line),value_string);
						p_ls->push_back(report_line);
					}
					break;
				case Seconds:
					hh = pcSummaryPage->ItemMap(i)->GetValue(pcSummaryPage->ItemIndex(i))/3600;
					mm = (pcSummaryPage->ItemMap(i)->GetValue(pcSummaryPage->ItemIndex(i))-(hh*3600))/60;
					ss = pcSummaryPage->ItemMap(i)->GetValue(pcSummaryPage->ItemIndex(i))-(hh*3600)-(mm*60);
					i_r = sprintf_s(value_string, sizeof(value_string),"%02d:%02d:%02d",hh,mm,ss);
					i_r = strcat_s(report_line,sizeof(report_line),value_string);
					p_ls->push_back(report_line);
					break;
				case Timevalue:
					hh = pcSummaryPage->ItemMap(i)->GetValue(pcSummaryPage->ItemIndex(i))/3600;
					mm = (pcSummaryPage->ItemMap(i)->GetValue(pcSummaryPage->ItemIndex(i))-(hh*3600))/60;
					ss = pcSummaryPage->ItemMap(i)->GetValue(pcSummaryPage->ItemIndex(i))-(hh*3600)-(mm*60);
					i_r = sprintf_s(value_string, sizeof(value_string),"%02d:%02d:%02d",hh,mm,ss);
					i_r = strcat_s(report_line,sizeof(report_line),value_string);
					p_ls->push_back(report_line);
					break;
				default:
					break;
				}
			}

	return dw_rc;
}