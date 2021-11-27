#pragma once

#include "stdafx.h"

#define MAX_EMAIL_ADDR_LENGTH 254

extern "C++" const char* KeyRecipients;

using namespace std;

class CMailAcctInfo
{
public:
	CMailAcctInfo(void);
	~CMailAcctInfo(void);
	TCHAR* GetMailServer();
	char* chGetMailServer();
	TCHAR* GetMailSender();
	char* chGetMailSender();
	TCHAR* GetEmailAcctName();
	char* chGetEmailAcctName();
	TCHAR* GetPassword();
	char* chGetPassword();
	TCHAR* GetRecipient(UINT uiIndex);
	char* chGetRecipient(UINT uiIndex,char* buf,UINT cbBufSiz);
	UINT GetRecipientsCnt();
	UINT GetMailServerLen();
	UINT GetMailSenderLen();
	UINT GetUserNameLen();
	UINT GetPasswordLen();
	BOOL UseAuthentication();
	SMTP_SECURITY_TYPE GetEncryptionType();
	TCHAR* AddRecipient(TCHAR* szRecipient);
	std::wstring *AddRecipient(std::wstring &ws_Recipient);
#if 0
	BOOL RmvRecipient(TCHAR* szRecipient,size_t cchMax);
	BOOL RmvRecipient(UINT uiIndex);
	UINT IncRecipientsCnt();
	UINT DecRecipientsCnt();
	UINT SetRecipientsCnt(UINT uiCount);
#endif
	TCHAR* SetMailServer(TCHAR *p_s);
	TCHAR* SetMailSender(TCHAR *p_s);
	TCHAR* SetEmailAcctName(TCHAR *p_s);
	TCHAR* SetPassword(TCHAR *p_s);
	BOOL SetAuthentication(BOOL b);
	SMTP_SECURITY_TYPE SetEncryptionType(SMTP_SECURITY_TYPE t);
	std::list<wstring> *Get_Recipients_List();
private:
	std::list<wstring> *m_lwsRecipients;
	UINT m_uiRecipientsCnt;
	TCHAR* m_szMailServer;
	char* m_chMailServer;
	UINT m_uiMailServerLen;
	TCHAR* m_szMailSender;
	char* m_chMailSender;
	UINT m_uiMailSenderLen;
	TCHAR* m_szUserName;
	char* m_chUserName;
	UINT m_uiUserNameLen;
	TCHAR* m_szPassword;
	char* m_chPassword;
	UINT m_uiPasswordLen;
	BOOL m_bUseAuthentication;
	SMTP_SECURITY_TYPE m_EncryptionType;
};
