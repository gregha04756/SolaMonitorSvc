#include "stdafx.h"
#include "SolaMonitorSvc.h"
#include "MailAcctInfo.h"
#include "SMSvcMsg.h"

DWORD WINAPI MailSendFunc(LPVOID lpParam)
{
	BOOL bSuccess = true;
	BOOL bSMTPError = false;
	ECSmtp::CSmtpError SMTPErr = ECSmtp::CSMTP_NO_ERROR;
	BOOL bResult;
	HRESULT hRes;
	LRESULT lRes;
	INT_PTR ipResult;
	UINT uintResult;
	PVOID pvResult;
	CSmtp* lpMail;
	SMTP_SECURITY_TYPE SecType;
	size_t cchLen;
	int nResult;
	int i;
	int nRecipientsCnt;
	TCHAR szTemp[MAX_LOADSTRING];
	TCHAR szError[MAX_LOADSTRING];
	TCHAR* lpszError;
	char chTemp[MAX_LOADSTRING];
	LPMAILSENDPARMS lp;
	CMailAcctInfo* lpMAI;
	std::list<string> *p_Msg;
	std::list<string>::iterator it_str;

	lp = (LPMAILSENDPARMS)lpParam;
	p_Msg = NULL;
	if ( NULL != lp->p_msg )
	{
		try
		{
			p_Msg = (std::list<string>*)new std::list<string>;
			for ( it_str = lp->p_msg->begin();(NULL != p_Msg) && (it_str != lp->p_msg->end()); it_str++ )
			{
				p_Msg->push_back(it_str->c_str());
			}
			delete lp->p_msg;
			lp->p_msg = NULL;
		}
		catch (std::exception &e)
		{
			bSuccess = false;
		}
	}
	lpMAI = lp->lpMAI;
	delete lp;

	pvResult = ::SecureZeroMemory((PVOID)szError,sizeof(szError)/sizeof(TCHAR));

	try
	{
		lpMail = (CSmtp*)new CSmtp();
	}
	catch(ECSmtp e)
	{
		SMTPErr = e.GetErrorNum();
		nResult = ::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,(LPCSTR)e.GetErrorText().c_str(),e.GetErrorText().length(),szError,sizeof(szError)/sizeof(TCHAR));
		bSMTPError = true;
		bSuccess = false;
	}
#if DEBUGMAILSEND
			SvcReportEvent(_T("CSmtp allocated"),(DWORD)SMTPErr,SVC_INFORMATION);
#endif

#if DEBUGMAILSEND
		SvcReportEvent(lpMAI->GetEmailAcctName(),(DWORD)SMTPErr,SVC_INFORMATION);
#endif
	if ( !bSMTPError && bSuccess && lpMAI->UseAuthentication() )
	{
		pvResult = SecureZeroMemory((PVOID)chTemp,sizeof(chTemp));
		nResult = ::sprintf(chTemp,"%s",lpMAI->chGetEmailAcctName());
#if DEBUGMAILSEND
		SvcReportEvent(lpMAI->GetEmailAcctName(),(DWORD)bSuccess,SVC_INFORMATION);
#endif
		try
		{
			lpMail->SetLogin((const char*)lpMAI->chGetEmailAcctName());
		}
		catch(ECSmtp e)
		{
			SMTPErr = e.GetErrorNum();
			nResult = ::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,(LPCSTR)e.GetErrorText().c_str(),e.GetErrorText().length(),szError,sizeof(szError)/sizeof(TCHAR));
			bSMTPError = true;
			bSuccess = false;
		}
	}
#if DEBUGMAILSEND
			SvcReportEvent(_T("CSmtp login set"),(DWORD)SMTPErr,SVC_INFORMATION);
#endif
	if ( !bSMTPError && bSuccess && lpMAI->UseAuthentication() )
	{
		try
		{
			lpMail->SetPassword((const char*)lpMAI->chGetPassword());
		}
		catch(ECSmtp e)
		{
			SMTPErr = e.GetErrorNum();
			nResult = ::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,(LPCSTR)e.GetErrorText().c_str(),e.GetErrorText().length(),szError,sizeof(szError)/sizeof(TCHAR));
			bSMTPError = true;
			bSuccess = false;
		}
	}
#if DEBUGMAILSEND
			SvcReportEvent(_T("CSmtp password set"),(DWORD)SMTPErr,SVC_INFORMATION);
#endif

	SecType = lpMAI->GetEncryptionType();
	if ( !bSMTPError && bSuccess && ((SecType == USE_SSL) || (SecType == USE_TLS)) )
	{
		try
		{
			lpMail->SetSecurityType(SecType);
		}
		catch(ECSmtp e)
		{
			SMTPErr = e.GetErrorNum();
			nResult = ::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,(LPCSTR)e.GetErrorText().c_str(),e.GetErrorText().length(),szError,sizeof(szError)/sizeof(TCHAR));
			bSMTPError = true;
			bSuccess = false;
		}
	}

	if ( !bSMTPError && bSuccess )
	{
		try
		{
			switch(SecType)
			{
			case DO_NOT_SET:
			case NO_SECURITY:
				lpMail->SetSMTPServer(lpMAI->chGetMailServer(),25);
				break;
			case USE_SSL:
				lpMail->SetSMTPServer(lpMAI->chGetMailServer(),465);
				break;
			case USE_TLS:
				lpMail->SetSMTPServer(lpMAI->chGetMailServer(),587);
				break;
			}
		}
		catch (ECSmtp e)
		{
			SMTPErr = e.GetErrorNum();
			nResult = ::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,(LPCSTR)e.GetErrorText().c_str(),e.GetErrorText().length(),szError,sizeof(szError)/sizeof(TCHAR));
			bSMTPError = true;
			bSuccess = false;
		}
	}
#if DEBUGMAILSEND
			SvcReportEvent(_T("CSmtp security type set"),(DWORD)SMTPErr,SVC_INFORMATION);
#endif

	if ( !bSMTPError && bSuccess )
	{
		try
		{
			lpMail->SetSenderMail((const char*)lpMAI->chGetMailSender());
			lpMail->SetReplyTo((const char*)lpMAI->chGetMailSender());
			lpMail->SetSenderName("Sola_Monitor_Service");
	  		lpMail->SetSubject("Information message");
		}
		catch (ECSmtp e)
		{
			SMTPErr = e.GetErrorNum();
			nResult = ::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,(LPCSTR)e.GetErrorText().c_str(),e.GetErrorText().length(),szError,sizeof(szError)/sizeof(TCHAR));
			bSMTPError = true;
			bSuccess = false;
		}
	}
#if DEBUGMAILSEND
			SvcReportEvent(_T("CSmtp sender name set"),(DWORD)SMTPErr,SVC_INFORMATION);
#endif

#if DEBUGMAILSEND
	pvResult = ::SecureZeroMemory((PVOID)szError,sizeof(szError));
	hRes = StringCchPrintf(szError,sizeof(szError)/sizeof(TCHAR),_T("MailSendFunc() adding %d recipients"),lpMAI->GetRecipientsCnt());
	SvcReportEvent(szError,(DWORD)SMTPErr,SVC_INFORMATION);
#endif
	if ( !bSMTPError && bSuccess )
	{
		for ( i = 0; (i < lpMAI->GetRecipientsCnt()) && bSuccess && !bSMTPError; i++ )
		{
#if DEBUGMAILSEND
			pvResult = ::SecureZeroMemory((PVOID)szError,sizeof(szError));
			pvResult = ::SecureZeroMemory((PVOID)chTemp,sizeof(chTemp));
			lpMAI->chGetRecipient((UINT)i,chTemp,(UINT)sizeof(chTemp));
			nResult = ::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,(LPCSTR)chTemp,strlen(chTemp),szError,sizeof(szError)/sizeof(TCHAR));
			SvcReportEvent(lpMAI->GetRecipient((UINT)i),(DWORD)SMTPErr,SVC_INFORMATION);
#endif
			try
			{
				pvResult = ::SecureZeroMemory((PVOID)chTemp,sizeof(chTemp)/sizeof(char));
				if ( NULL != lpMAI->chGetRecipient((UINT)i,chTemp,(UINT)sizeof(chTemp)/sizeof(char)) )
				{
#if DEBUGMAILSEND
			pvResult = ::SecureZeroMemory((PVOID)szError,sizeof(szError));
			nResult = ::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,(LPCSTR)chTemp,strlen(chTemp),szError,sizeof(szError)/sizeof(TCHAR));
			SvcReportEvent(szError,(DWORD)SMTPErr,SVC_INFORMATION);
#endif
					lpMail->AddRecipient((const char*)chTemp);
				}
			}
			catch (ECSmtp e)
			{
				SMTPErr = e.GetErrorNum();
				nResult = ::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,(LPCSTR)e.GetErrorText().c_str(),e.GetErrorText().length(),szError,sizeof(szError)/sizeof(TCHAR));
				bSMTPError = true;
				bSuccess = false;
				break;
			}
		}
/*					lpMail->AddRecipient("ghansen@entecinc.ca"); */
	}
#if DEBUGMAILSEND
			SvcReportEvent(_T("CSmtp recipients set"),(DWORD)SMTPErr,SVC_INFORMATION);
#endif

	try
	{
  		lpMail->SetXPriority(XPRIORITY_NORMAL);
  		lpMail->SetXMailer("The Bat! (v3.02) Professional");
  		lpMail->AddMsgLine("Hello,");
		lpMail->AddMsgLine("");
		lpMail->AddMsgLine("...");
		if ( NULL != p_Msg )
		{
			for ( it_str = p_Msg->begin(); it_str != p_Msg->end(); it_str++ )
			{
				lpMail->AddMsgLine(it_str->c_str());
			}
			delete p_Msg;
			p_Msg = NULL;
		}
		lpMail->AddMsgLine("This is an informational message from the Sola Monitoring Service");
		lpMail->Send();
	}
	catch(ECSmtp e)
	{
		SMTPErr = e.GetErrorNum();
		nResult = ::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,(LPCSTR)e.GetErrorText().c_str(),e.GetErrorText().length(),szError,sizeof(szError)/sizeof(TCHAR));
		bSMTPError = true;
		bSuccess = false;
	}
#if DEBUGMAILSEND
			SvcReportEvent(_T("Error: CSmtp send() "),(DWORD)SMTPErr,SVC_INFORMATION);
#endif

#if 0

	try
	{
		CSmtp mail;

//#define test_gmail_tls
#define test_gmail_ssl

#if defined(test_gmail_tls)
		mail.SetSMTPServer("smtp.gmail.com",587);
//		mail.SetSecurityType(CSmtp::USE_TLS);
		mail.SetSecurityType(USE_TLS);
#elif defined(test_gmail_ssl)
		mail.SetSMTPServer("smtp.gmail.com",465);
		mail.SetSecurityType(USE_SSL);
#elif defined(test_hotmail_TLS)
		mail.SetSMTPServer("smtp.live.com",25);
		mail.SetSecurityType(CSmtp::USE_TLS);
#elif defined(test_aol_tls)
		mail.SetSMTPServer("smtp.aol.com",587);
		mail.SetSecurityType(CSmtp::USE_TLS);
#elif defined(test_yahoo_ssl)
		mail.SetSMTPServer("plus.smtp.mail.yahoo.com",465);
		mail.SetSecurityType(CSmtp::USE_SSL);
#endif

		mail.SetLogin("gregha04756@gmail.com");
		mail.SetPassword("c3$$na402");
  		mail.SetSenderName("Greg");
  		mail.SetSenderMail("gregha04756@gmail.com");
  		mail.SetReplyTo("gregha04756@gmail.com");
  		mail.SetSubject("Test message");
  		mail.AddRecipient("gregha04756@hotmail.com");
  		mail.AddRecipient("ghansen@entecinc.ca");
  		mail.SetXPriority(XPRIORITY_NORMAL);
  		mail.SetXMailer("The Bat! (v3.02) Professional");
  		mail.AddMsgLine("Hello,");
		mail.AddMsgLine("");
		mail.AddMsgLine("...");
		mail.AddMsgLine("This is a test");
		mail.AddMsgLine("");
		mail.AddMsgLine("Regards");
		mail.ModMsgLine(5,"regards");
		mail.DelMsgLine(2);
		mail.AddMsgLine("Greg");
		
  		//mail.AddAttachment("../test1.jpg");
  		//mail.AddAttachment("c:\\test2.exe");
		//mail.AddAttachment("c:\\test3.txt");
		mail.Send();
	}
	catch(ECSmtp e)
	{
		std::cout << "Error: " << e.GetErrorText().c_str() << ".\n";
		bError = true;
	}
	if(!bError)
		std::cout << "Mail was sent successfully.\n";
#endif
	if ( NULL != lpMail )
	{
		delete lpMail;
		lpMail = NULL;
	}
	if ( NULL != p_Msg )
	{
		delete p_Msg;
		p_Msg = NULL;
	}
	return (DWORD)SMTPErr;
}