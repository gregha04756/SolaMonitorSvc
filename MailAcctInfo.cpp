#include "StdAfx.h"
#include "MailAcctInfo.h"

CMailAcctInfo::CMailAcctInfo(void)
{
	m_szMailServer = m_szMailSender = m_szUserName = m_szPassword = NULL;
	m_chMailServer = m_chMailSender = m_chUserName = m_chPassword = NULL;
	m_uiMailServerLen = m_uiMailSenderLen = m_uiUserNameLen = m_uiPasswordLen = m_uiRecipientsCnt = 0;
	m_bUseAuthentication = false;
	m_EncryptionType = NO_SECURITY;
	try
	{
		m_lwsRecipients = (list<wstring>*)new list<wstring>;
	}
	catch (std::exception &e)
	{
		throw e;
	}
}

CMailAcctInfo::~CMailAcctInfo(void)
{
#if _DEBUG
	std::string bs("MailAcctInfo destructor called");
	std::ofstream blah;
	blah.open("D:\\Visual Studio Projects\\SolaMonitorSvc\\blah.txt",ios::binary);
	blah.write(bs.c_str(),bs.size());
	blah.close();
#endif

	if ( m_szMailServer )
	{
		delete[] m_szMailServer;
		m_szMailServer = NULL;
	}
	if ( m_szMailSender )
	{
		delete[] m_szMailSender;
		m_szMailSender = NULL;
	}
	if ( m_szUserName )
	{
		delete[] m_szUserName;
		m_szUserName = NULL;
	}
	if ( m_szPassword )
	{
		delete[] m_szPassword;
		m_szPassword = NULL;
	}
	if ( m_chMailServer )
	{
		delete[] m_chMailServer;
		m_chMailServer = NULL;
	}
	if ( m_chMailSender )
	{
		delete[] m_chMailSender;
		m_chMailSender = NULL;
	}
	if ( m_chUserName )
	{
		delete[] m_chUserName;
		m_chUserName = NULL;
	}
	if ( m_chPassword )
	{
		delete[] m_chPassword;
		m_chPassword = NULL;
	}
	if ( m_lwsRecipients )
	{
		delete m_lwsRecipients;
		m_lwsRecipients = NULL;
	}
}

BOOL CMailAcctInfo::UseAuthentication()
{
	return m_bUseAuthentication;
}

SMTP_SECURITY_TYPE CMailAcctInfo::GetEncryptionType()
{
	return m_EncryptionType;
}

TCHAR* CMailAcctInfo::GetMailSender()
{
	return m_szMailSender;
}

char* CMailAcctInfo::chGetMailSender()
{
	return m_chMailSender;
}

UINT CMailAcctInfo::GetMailSenderLen()
{
	return m_uiMailSenderLen;
}

TCHAR* CMailAcctInfo::GetMailServer()
{
	return m_szMailServer;
}

char* CMailAcctInfo::chGetMailServer()
{
	return m_chMailServer;
}

UINT CMailAcctInfo::GetMailServerLen()
{
	return m_uiMailServerLen;
}

TCHAR* CMailAcctInfo::GetEmailAcctName()
{
	return m_szUserName;
}

char* CMailAcctInfo::chGetEmailAcctName()
{
	return m_chUserName;
}

TCHAR* CMailAcctInfo::GetPassword()
{
	return m_szPassword;
}

char* CMailAcctInfo::chGetPassword()
{
	return m_chPassword;
}

UINT CMailAcctInfo::GetPasswordLen()
{
	return m_uiPasswordLen;
}

BOOL CMailAcctInfo::SetAuthentication(BOOL b)
{
	return (m_bUseAuthentication = b);

}

SMTP_SECURITY_TYPE CMailAcctInfo::SetEncryptionType(SMTP_SECURITY_TYPE t)
{
	return (m_EncryptionType = t);
}

TCHAR* CMailAcctInfo::SetMailSender(TCHAR* p_s)
{
	int nResult;
	HRESULT hRes;
	PVOID lpv;
	int i;
	int len;
	len = 0;
	hRes = StringCchLength(p_s,MAX_EMAIL_ADDR_LENGTH,(size_t*)&len);
	if ( len )
	{
		if ( NULL != m_szMailSender && m_uiMailSenderLen > 0 )
		{
			delete[] m_szMailSender;
			m_szMailSender = NULL;
			delete[] m_chMailSender;
			m_chMailSender = NULL;
			m_uiMailSenderLen = 0;
		}
		try
		{
			m_szMailSender = (TCHAR*)new TCHAR[len+1];
			m_chMailSender = (char*)new char[len+1];
		}
		catch (bad_alloc& ba)
		{
			return NULL;
		}
		m_uiMailSenderLen = (UINT)len;
		lpv = ::SecureZeroMemory((PVOID)m_szMailSender,sizeof(TCHAR)*(len+1));
		lpv = ::SecureZeroMemory((PVOID)m_chMailSender,sizeof(char)*(len+1));
		hRes = StringCchPrintf(m_szMailSender,len+1,_T("%s"),p_s);
		nResult = ::WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,m_szMailSender,len,(LPSTR)m_chMailSender,len,NULL,NULL);
	}
	if ( !len && (NULL != m_szMailSender) && m_uiMailSenderLen )
	{
		delete[] m_szMailSender;
		m_szMailSender = NULL;
		delete[] m_chMailSender;
		m_chMailSender = NULL;
		m_uiMailSenderLen = 0;
	}
	return m_szMailSender;
}

TCHAR* CMailAcctInfo::SetMailServer(TCHAR* p_s)
{
	int nResult;
	HRESULT hRes;
	PVOID lpv;
	int i;
	int len;
	len = 0;
	hRes = StringCchLength(p_s,MAX_EMAIL_ADDR_LENGTH,(size_t*)&len);
	if ( len )
	{
		if ( NULL != m_szMailServer && m_uiMailServerLen > 0 )
		{
			delete[] m_szMailServer;
			m_szMailServer = NULL;
			delete[] m_chMailServer;
			m_chMailServer = NULL;
			m_uiMailServerLen = 0;
		}
		try
		{
			m_szMailServer = (TCHAR*)new TCHAR[len+1];
			m_chMailServer = (char*)new char[len+1];
		}
		catch (bad_alloc& ba)
		{
			return NULL;
		}
		m_uiMailServerLen = (UINT)len;
		lpv = ::SecureZeroMemory((PVOID)m_szMailServer,len+1);
		lpv = ::SecureZeroMemory((PVOID)m_chMailServer,len+1);
		hRes = StringCchPrintf(m_szMailServer,len+1,_T("%s"),p_s);
		nResult = ::WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,m_szMailServer,len,(LPSTR)m_chMailServer,len,NULL,NULL);
	}
	if ( !len && (NULL != m_szMailServer) && m_uiMailServerLen )
	{
		delete[] m_szMailServer;
		m_szMailServer = NULL;
		delete[] m_chMailServer;
		m_chMailServer = NULL;
		m_uiMailServerLen = 0;
	}
	return m_szMailServer;
}

TCHAR* CMailAcctInfo::SetPassword(TCHAR *p_s)
{
	int nResult;
	HRESULT hRes;
	PVOID lpv;
	int i;
	int len;
	len = 0;
	hRes = StringCchLength(p_s,MAX_EMAIL_ADDR_LENGTH,(size_t*)&len);
	if ( len )
	{
		if ( NULL != m_szPassword && m_uiPasswordLen > 0 )
		{
			delete[] m_szPassword;
			m_szPassword = NULL;
			delete[] m_chPassword;
			m_chPassword = NULL;
			m_uiPasswordLen = 0;
		}
		try
		{
			m_szPassword = (TCHAR*)new TCHAR[len+1];
			m_chPassword = (char*)new char[len+1];
		}
		catch (bad_alloc& ba)
		{
			return NULL;
		}
		m_uiPasswordLen = (UINT)len;
		lpv = ::SecureZeroMemory((PVOID)m_szPassword,len+1);
		lpv = ::SecureZeroMemory((PVOID)m_chPassword,len+1);
		hRes = StringCchPrintf(m_szPassword,len+1,_T("%s"),p_s);
		nResult = ::WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,m_szPassword,len,(LPSTR)m_chPassword,len,NULL,NULL);
	}
	if ( !len && (NULL != m_szPassword) && m_uiPasswordLen )
	{
		delete[] m_szPassword;
		m_szPassword = NULL;
		delete[] m_szPassword;
		m_chPassword = NULL;
		m_uiPasswordLen = 0;
	}
	return m_szPassword;
}

TCHAR* CMailAcctInfo::SetEmailAcctName(TCHAR *p_s)
{
	int nResult;
	HRESULT hRes;
	PVOID lpv;
	int i;
	int len;
	len = 0;
	hRes = StringCchLength(p_s,MAX_EMAIL_ADDR_LENGTH,(size_t*)&len);
	if ( len )
	{
		if ( NULL != m_szUserName && m_uiUserNameLen > 0 )
		{
			delete[] m_szUserName;
			m_szUserName = NULL;
			delete[] m_chUserName;
			m_chUserName = NULL;
			m_uiUserNameLen = 0;
		}
		try
		{
			m_szUserName = (TCHAR*)new TCHAR[len+1];
			m_chUserName = (char*)new char[len+1];
		}
		catch (bad_alloc& ba)
		{
			return NULL;
		}
		m_uiUserNameLen = (UINT)len;
		lpv = ::SecureZeroMemory((PVOID)m_szUserName,len+1);
		lpv = ::SecureZeroMemory((PVOID)m_chUserName,len+1);
		hRes = StringCchPrintf(m_szUserName,len+1,_T("%s"),p_s);
		nResult = ::WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,m_szUserName,len,(LPSTR)m_chUserName,len,NULL,NULL);
	}
	if ( !len && (NULL != m_szUserName) && m_uiUserNameLen )
	{
		delete[] m_szUserName;
		m_szUserName = NULL;
		delete[] m_chUserName;
		m_chUserName = NULL;
		m_uiUserNameLen = 0;
	}
	return m_szUserName;
}

#if 0
UINT CMailAcctInfo::SetRecipientsCnt(UINT uiCount)
{
	return (m_uiRecipientsCnt = uiCount);
}
#endif

UINT CMailAcctInfo::GetRecipientsCnt()
{
	if ( NULL != m_lwsRecipients )
	{
		return (m_uiRecipientsCnt = (UINT)m_lwsRecipients->size());
	}
	return (m_uiRecipientsCnt = (UINT)0);
}

#if 0
UINT CMailAcctInfo::IncRecipientsCnt()
{
	return ++m_uiRecipientsCnt;
}

UINT CMailAcctInfo::DecRecipientsCnt()
{
	return (m_uiRecipientsCnt = (m_uiRecipientsCnt ? --m_uiRecipientsCnt : 0));
}
#endif

TCHAR* CMailAcctInfo::AddRecipient(TCHAR* szRecipient)
{
	m_lwsRecipients->push_back(szRecipient);
	return (TCHAR*)szRecipient;
}

std::wstring* CMailAcctInfo::AddRecipient(std::wstring &ws_Recipient)
{
	m_lwsRecipients->push_back(ws_Recipient);
	return &ws_Recipient;
}

#if 0
BOOL CMailAcctInfo::RmvRecipient(TCHAR* szRecipient,size_t cchMax)
{
	int nResult;
	HRESULT hRes;
	PVOID lpv;
	size_t len;
	int i;
	hRes = ::StringCchLength(szRecipient,cchMax,&len);
	return true;
}

BOOL CMailAcctInfo::RmvRecipient(UINT uiIndex)
{
	int nResult;
	HRESULT hRes;
	PVOID lpv;
	size_t len;
	int i;
	return true;
}
#endif

TCHAR* CMailAcctInfo::GetRecipient(UINT uiIndex)
{
	list<wstring>::iterator itty;
	UINT i = 0;
	for ( itty = m_lwsRecipients->begin(); itty != m_lwsRecipients->end() && i < (UINT)m_lwsRecipients->size(); itty++ )
	{
		if ( i == uiIndex )
		{
			return (TCHAR*)itty->data();
		}
	}
	return (TCHAR*)NULL;
}

std::list<wstring> *CMailAcctInfo::Get_Recipients_List()
{
	return m_lwsRecipients;
}

char* CMailAcctInfo::chGetRecipient(UINT uiIndex,char* buf,UINT cbBufSiz)
{
	int nResult;
	list<wstring>::iterator itty;
	UINT i = 0;
	for ( itty = m_lwsRecipients->begin(); itty != m_lwsRecipients->end() && i < (UINT)m_lwsRecipients->size(); itty++ )
	{
		if ( (i++) == uiIndex )
		{
			nResult = ::WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,itty->c_str(),itty->size(),(LPSTR)buf,cbBufSiz,NULL,NULL);
			return buf;
		}
	}
	return (char*)NULL;
}