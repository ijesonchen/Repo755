#include "StdAfx.h"
#include "PrivateProfile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool CPrivateProfile::GetString(LPCTSTR lpAppName, LPCTSTR lpKeyName, CString& strReturn)
{
	TCHAR buf[500];
	DWORD dwRet = ::GetPrivateProfileString(lpAppName, lpKeyName, _T(""), buf, 500, w_strIniFile);
	if (dwRet <= 0)
	{
		return false;
	}
	strReturn = buf;
	if (strReturn.Trim().IsEmpty())
	{
		return false;
	}
	return true;
}

bool CPrivateProfile::GetUInt(LPCTSTR lpAppName, LPCTSTR lpKeyName, unsigned int& nReturn)
{
	unsigned int nRet = GetPrivateProfileInt(lpAppName, lpKeyName, INT_MAX, w_strIniFile);
	if (nRet == INT_MAX)
	{
		return false;
	}
	nReturn = nRet;
	return true;
}

#pragma warning(disable: 4996)

bool CPrivateProfile::GetFloat(LPCTSTR lpAppName, LPCTSTR lpKeyName, float& fReturn)
{
	CString strReturn;
	if (!GetString(lpAppName, lpKeyName, strReturn))
	{
		return false;
	}
	if (_stscanf(strReturn, _T("%f"), &fReturn) != 1)
	{
		return false;
	}
	return true;
}

#pragma warning(default: 4996)

BOOL CPrivateProfile::WriteString( LPCTSTR lpAppName, LPCTSTR lpKeyName, const CString strWrite )
{
	return ::WritePrivateProfileString(lpAppName, lpKeyName, (LPCTSTR)strWrite, (LPCTSTR)w_strIniFile);
}
