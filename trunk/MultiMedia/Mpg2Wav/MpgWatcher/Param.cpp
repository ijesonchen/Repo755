#include "StdAfx.h"
#include "PrivateProfile.h"
#include "Param.h"
#include "CommonFunction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool CParam::IsValid()
{
	return (w_strWatcherDirectory.IsEmpty() &&
		w_strOutputDirectory.IsEmpty() &&
		w_strConverter.IsEmpty() &&
		w_strInterfaceFile.IsEmpty() &&
		w_strRootDirInput.IsEmpty() &&
		w_strRootDirOutput.IsEmpty() &&
		w_nWorkerThread);
}

bool CParam::ReadIniFile(const CString& strIniFile)
{
	w_strIniFile = strIniFile;
	CPrivateProfile privateProfile(w_strIniFile);

	CString strSectionDataBase = _T("Config");
	if (!privateProfile.GetString(strSectionDataBase, _T("WatcherDirectory"), w_strWatcherDirectory))
	{
		WiiLog(_T("WatcherDirectory×Ö¶Î¸ñÊ½´íÎó"));
		return FALSE;
	}
	if (!privateProfile.GetString(strSectionDataBase, _T("OutputDirectory"), w_strOutputDirectory))
	{
		WiiLog(_T("OutputDirectory×Ö¶Î¸ñÊ½´íÎó"));
		return FALSE;
	}
	if (!privateProfile.GetString(strSectionDataBase, _T("Converter"), w_strConverter))
	{
		WiiLog(_T("Converter×Ö¶Î¸ñÊ½´íÎó"));
		return FALSE;
	}
	if (!privateProfile.GetString(strSectionDataBase, _T("OutputInterface"), w_strInterfaceFile))
	{
		WiiLog(_T("OutputInterface×Ö¶Î¸ñÊ½´íÎó"));
		return FALSE;
	}
	return TRUE;
}

CString CParam::GetDetailInfo()
{
	CString str = _T("\r\n======================ÅäÖÃÐÅÏ¢======================\r\n");
	CString strTmp;

	strTmp.Format(_T("WatcherDirectory = %s\r\n"), w_strWatcherDirectory);	
	str += strTmp;
	strTmp.Format(_T("OutputDirectory = %s\r\n"), w_strOutputDirectory);	
	str += strTmp;
	strTmp.Format(_T("Converter = %s\r\n"), w_strConverter);	
	str += strTmp;
	strTmp.Format(_T("OutputInterface = %s\r\n"), w_strInterfaceFile);	
	str += strTmp;

	str += _T("=================================================\r\n");
	return str;
}

bool CParam::WriteStartFileID(unsigned int nStartFIleID)
{
	CPrivateProfile privateProfile(w_strIniFile);
	CString strSectionDataBase = _T("Config");
	CString strValue;
	strValue.Format(_T("%u"), nStartFIleID);
	if (!privateProfile.WriteString(strSectionDataBase, _T("StartFileID"), strValue))
	{
		return false;
	}
	return true;
}

bool CParam::Reload(void)
{
	if (w_strIniFile.IsEmpty())
	{
		return false;
	}
	else
	{
		return ReadIniFile(w_strIniFile);
	}
}
