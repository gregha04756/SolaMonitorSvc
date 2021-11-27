#include "stdafx.h"
#include "SolaMonitorSvc.h"
#include "MailAcctInfo.h"
#include "SMSvcMsg.h"

using namespace std;

CMailAcctInfo* Setup_Mail_Scct_Info(CIniFile *p_INI)
{
	std::list<std::wstring> *p_rcpnt_list;
	std::list<std::wstring>::iterator it_rl;
	CMailAcctInfo *p_MAI_Local;
	BOOL b_result;
	BOOL b_success;
	int i_result;
	DWORD dw_result;

	b_success = true;
	p_MAI_Local = NULL;
	p_rcpnt_list = NULL;
	try
	{
		p_MAI_Local = (CMailAcctInfo*)new CMailAcctInfo();
	}
	catch (bad_alloc& ba)
	{
		b_success = false;
		p_MAI_Local = NULL;
	}

	if ( b_success )
	{
		b_success = (NULL != p_MAI_Local->SetMailServer((TCHAR*)LoadIniFileValues(p_INI,IDC_EDITMAILSERVER)));
	}
	if ( b_success )
	{
		b_success = (NULL != p_MAI_Local->SetMailSender((TCHAR*)LoadIniFileValues(p_INI,IDC_EDITMAILSENDER)));
	}
	b_result = p_MAI_Local->SetAuthentication(*((BOOL*)LoadIniFileValues(p_INI,IDC_CHKUSEAUTHENT)));
	if ( p_MAI_Local->UseAuthentication() )
	{
		b_success = (NULL != p_MAI_Local->SetEmailAcctName((TCHAR*)LoadIniFileValues(p_INI,IDC_EDITUSERNAME)));
		if ( b_success )
		{
			b_success = (NULL != p_MAI_Local->SetPassword((TCHAR*)LoadIniFileValues(p_INI,IDC_EDITPASSWORD)));
		}
		if ( b_success )
		{
			switch ((SMTP_SECURITY_TYPE)*((int*)LoadIniFileValues(p_INI,IDC_ENCRYPTION)))
			{
			case NO_SECURITY:
				p_MAI_Local->SetEncryptionType(NO_SECURITY);
				break;
			case USE_TLS:
				p_MAI_Local->SetEncryptionType(USE_TLS);
				break;
			case USE_SSL:
				p_MAI_Local->SetEncryptionType(USE_SSL);
				break;
			default:
				b_success = false;
				break;
			}
		}
	}
	if ( b_success )
	{
/*		p_MAI_Local->SetRecipientsCnt(p_INI->GetNumValues(std::string(KeyRecipients))); */
		p_rcpnt_list = (std::list<std::wstring>*)LoadIniFileValues(p_INI,IDC_LISTRECIPIENTS);
		for ( it_rl = p_rcpnt_list->begin(); b_success && (it_rl != p_rcpnt_list->end()); it_rl++ )
		{
#if _DEBUG
	SvcReportEvent((TCHAR*)it_rl->c_str(),(DWORD)b_success,SVC_INFORMATION);
#endif
/*			p_MAI_Local->AddRecipient((TCHAR*)it_rl->c_str());*/
			p_MAI_Local->AddRecipient(*it_rl);
		}
		b_success = (0 != p_MAI_Local->GetRecipientsCnt());
	}
	if ( NULL != p_rcpnt_list )
	{
		delete p_rcpnt_list;
		p_rcpnt_list = NULL;
	}
	return p_MAI_Local;
}