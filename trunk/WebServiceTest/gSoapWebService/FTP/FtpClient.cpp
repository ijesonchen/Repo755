#include "StdAfx.h"
#include "FtpClient.h"

#pragma comment(lib, "Wininet.lib")

using namespace std;

CFtpClient::CFtpClient(const CString username, 
					   const CString password,
					   const CString hostname,
					   const int hostport /*= DEFAULT_FTP_PORT*/)
	: m_strUserName(username)
	, m_strPassword(password)
	, m_strHostName(hostname)
	, m_nHostPort(hostport)
	, m_nSleepMs(DEFAULT_SLEEP_TIME_MS)
	, hInternet(NULL)
	, hFtp(NULL)
	, DEFAULT_USER_ANGENT_NAME(_T("CFtpClient Agent"))
	, ERROR_LOGIN_FIRST(_T("Login first."))
	{
	}
	

CFtpClient::~CFtpClient(void)
{
}

// Set message when function failed, use GetLastError and InternetGetLastResponseInfo
void CFtpClient::SetFtpMessage(CString strFunctionName)
{
	DWORD dwLastError = GetLastError(); 

	if (dwLastError == ERROR_INTERNET_EXTENDED_ERROR)
	{
		DWORD dwBufferLeng = 0;
		DWORD dwNetError = 0;
		TCHAR* pBuffer = NULL;
		BOOL bRet = InternetGetLastResponseInfo(&dwNetError, pBuffer, &dwBufferLeng);
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			int nLen = dwBufferLeng;
			pBuffer = new TCHAR[nLen];
			bRet = InternetGetLastResponseInfo(&dwNetError, pBuffer, &dwBufferLeng);
			if (bRet)
			{
				m_strMessage.Format(_T("%s ERROR_INTERNET_EXTENDED_ERROR %u: %s"), 
					strFunctionName.GetString(), dwNetError, pBuffer);
			}
			else
			{
				m_strMessage.Format(_T("%s get ERROR_INTERNET_EXTENDED_ERROR failed."), 
					strFunctionName.GetString());
			}
			delete pBuffer;
			pBuffer = NULL;
		}
	}
	else
	{
		LPVOID lpMsgBuf;

		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwLastError,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &lpMsgBuf,
			0, NULL );

		m_strMessage.Format(_T("%s failed %d: %s"), 
			strFunctionName.GetString(), dwLastError, lpMsgBuf);

		LocalFree(lpMsgBuf);
	}
}

bool CFtpClient::Login(void)
{
	m_strMessage.Empty();
	CString strFunctionName(_T(""));

	hInternet = InternetOpen(DEFAULT_USER_ANGENT_NAME, 
					INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);

	if (!hInternet)
	{
		SetFtpMessage(strFunctionName);
		return false;
	}

	hFtp = InternetConnect(hInternet, m_strHostName, m_nHostPort, 
		m_strUserName.GetString(), m_strPassword.GetString(), 
		INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, NULL);

	if (!hFtp)
	{
		InternetCloseHandle(hFtp);
		hFtp = NULL;
		SetFtpMessage(strFunctionName);
		return false;
	}

	return true;
}

void CFtpClient::Logout(void)
{
	if (hInternet)
	{
		InternetCloseHandle(hInternet);
		hInternet = NULL;
	}

	if (hFtp)
	{
		InternetCloseHandle(hFtp);
		hFtp = NULL;
	}
}

//************************************
// list files and folders
// dwFindFlags:
// 	INTERNET_FLAG_HYPERLINK
// 	INTERNET_FLAG_NEED_FILE
// 	INTERNET_FLAG_NO_CACHE_WRITE
// 	INTERNET_FLAG_RELOAD
// 	INTERNET_FLAG_RESYNCHRONIZE
//************************************
bool CFtpClient::ListAll(std::deque<CString>& dqFiles, 
						std::deque<CString>& dqFolders,
						DWORD   dwFindFlags /*= INTERNET_FLAG_RELOAD*/)
{
	m_strMessage.Empty();
	if (!hFtp)
	{
		return false;
	}
	CString strFunctionName(_T("ListAll"));

	WIN32_FIND_DATA dirInfo;
	HINTERNET       hFind;
	DWORD           dwRet;

	dqFolders.clear();

	hFind = FtpFindFirstFile( hFtp, TEXT( "*.*" ), 
		&dirInfo, dwFindFlags, 0 );
	if ( hFind == NULL )
	{
		if( ( dwRet = GetLastError() ) != ERROR_NO_MORE_FILES )
		{
			return false;
		}
		return true;
	}

	do
	{
		if (dirInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (dirInfo.cFileName[0] != '.')
			{
				dqFolders.push_back(dirInfo.cFileName);
			}
		}
		else
		{
			dqFiles.push_back(dirInfo.cFileName);
		}

	} while( InternetFindNextFile( hFind, (LPVOID) &dirInfo ) );


	bool bRet = true;
	if( ( dwRet = GetLastError() ) != ERROR_NO_MORE_FILES )
	{
		bRet = false;
	}

	InternetCloseHandle(hFind);

	return bRet;
}

//************************************
// Get file and folder names string.
// Folders:
//	folder names
// Files:
//	file names
//************************************
CString CFtpClient::GetListString(DWORD dwFindFlags /*= INTERNET_FLAG_RELOAD*/)
{
	m_strMessage.Empty();
	CString strFunctionName(_T("GetListString"));

	deque<CString> dqFolders;
	deque<CString> dqFiles;
	CString strList;
	if (ListAll(dqFiles, dqFolders, dwFindFlags))
	{
		if (unsigned iLoop = dqFolders.size())
		{
			strList += _T("Folders\n");
			for (unsigned ii = 0; ii < iLoop; ii++)
			{
				strList += _T(" ") + dqFolders[ii] + _T("\n");
			}
		}
		if (unsigned iLoop = dqFiles.size())
		{
			strList += _T("Files\n");
			for (unsigned ii = 0; ii < iLoop; ii++)
			{
				strList += _T(" ") + dqFiles[ii] + _T("\n");
			}
		}
		if (strList.IsEmpty())
		{
			strList = _T("No files or folders found.");
		}
	}
	return strList;
}

bool CFtpClient::ChangeFolder( CString strFolderName )
{
	m_strMessage.Empty();
	CString strFunctionName(_T("ChangeFolder"));

	if( !FtpSetCurrentDirectory( hFtp, strFolderName ) )
	{
		SetFtpMessage(strFunctionName);
		return false;
	}
	return true;
}

bool CFtpClient::CreateFolder(CString strFolderName)
{
	m_strMessage.Empty();
	CString strFunctionName(_T("CreateFolder"));

	if( !FtpCreateDirectory( hFtp, strFolderName.GetString() ) )
	{
		SetFtpMessage(strFunctionName);
		return false;
	}
	return true;
}

// Remove will fail if folder is not empty.
bool CFtpClient::RemoveFolder( CString strFolderName )
{
	m_strMessage.Empty();
	CString strFunctionName(_T("RemoveFolder"));

	if( !FtpRemoveDirectory( hFtp, strFolderName.GetString() ) )
	{
		SetFtpMessage(strFunctionName);
		return false;
	}
	return true;
}

// local file will be overwrote when exist.
bool CFtpClient::Download(CString strFtpFileName, CString strLocalFileName, 
						  DWORD  dwTransferType /*= FTP_TRANSFER_TYPE_BINARY*/)
{
	m_strMessage.Empty();
	CString strFunctionName(_T("Download"));

	dwTransferType |= INTERNET_FLAG_RELOAD;

	if( !FtpGetFile( hFtp, 
		strFtpFileName.GetString(), strLocalFileName.GetString(), 
		FALSE, FILE_ATTRIBUTE_NORMAL, dwTransferType, NULL ) )
	{
		SetFtpMessage(strFunctionName);
		return false;
	}
	return true;
}

// remote file will be overwrote when exist.
bool CFtpClient::Upload( CString strLocalFileName, CString strFtpFileName, 
						DWORD dwTransferType/*= FTP_TRANSFER_TYPE_BINARY*/ )
{
	m_strMessage.Empty();
	CString strFunctionName(_T("Upload"));

	if( !FtpPutFile( hFtp, 
		strLocalFileName.GetString(), 
		strFtpFileName.GetString(), 
		dwTransferType, NULL ) )
	{
		SetFtpMessage(strFunctionName);
		return false;
	}
	return true;
}

bool CFtpClient::DeleteFtpFile( CString strFtpFileName )
{
	m_strMessage.Empty();
	CString strFunctionName(_T("DeleteFtpFile"));

	if( !FtpDeleteFile( hFtp, strFtpFileName.GetString() ) )
	{
		SetFtpMessage(strFunctionName);
		return false;
	}
	return true;
}

bool CFtpClient::RenameFile(CString strOldFileName, CString strNewFileName)
{
	m_strMessage.Empty();
	CString strFunctionName(_T("RenameFile"));

	if( !FtpRenameFile( hFtp, strOldFileName.GetString(), strNewFileName.GetString() ) )
	{
		SetFtpMessage(strFunctionName);
		return false;
	}
	return true;
}

//************************************
// check, download and delete files from remoteDir to localPath. 
// bExit: CheckFile will loop until bExit is true
//************************************
bool CFtpClient::CheckFile( volatile LONG &bExit, CString ftpFullPath, const CString localPath )
{
	// check parameter
	CString strSlash;
	if (!CheckFullPath(strSlash, ftpFullPath))
	{
		return false;
	}

	while (bExit == 0)
	{
		Sleep(m_nSleepMs);

		std::deque<CString> dqFiles;
		std::deque<CString> dqFolders;

		bool bRet;
		Logout();

		bRet = Login();
		bRet = ChangeFolder(ftpFullPath);
		if (!bRet)
		{
			continue;
		}
		bRet = ListAll(dqFiles, dqFolders);

		if (dqFiles.size())
		{
			for (unsigned ii = 0, iLoop = dqFiles.size();
				ii < iLoop; ii++)
			{
				CString strFtpName = ftpFullPath + strSlash + dqFiles[ii];
				CString strLocalName; 
				strLocalName.Format(_T("%s\\%s"), localPath, dqFiles[ii]);
				if (Download(strFtpName, strLocalName.GetString()))
				{
					bRet = DeleteFtpFile(strFtpName);
				}
			}
			return true;
		}
	}
	return false;
}

// download and delete file in ftpFullPath, local path will automatically create
bool CFtpClient::DownloadAndDelete(CString ftpFullPath, CString ftpFileName, CString strLocalFullName)
{
	CString strSlash;
	if (!CheckFullPath(strSlash, ftpFullPath))
	{
		return false;
	}

	// create local path
	CString strLocalPath;
	int nPos = strLocalFullName.ReverseFind('\\');
	strLocalPath = strLocalFullName.Left(nPos);
	DWORD dwRet = ::GetFileAttributes(strLocalPath);
	if ( dwRet == INVALID_FILE_ATTRIBUTES || (dwRet & FILE_ATTRIBUTE_DIRECTORY) == 0) 
	{
		if (ERROR_SUCCESS != ::SHCreateDirectoryEx(NULL, strLocalPath, NULL))
		{
			return false;
		}
	}

	// download and delete
	CString strFtpFullName = ftpFullPath + strSlash + ftpFileName;
	if (Download(strFtpFullName, strLocalFullName))
	{
		DeleteFtpFile(strFtpFullName);
		return true;
	}
	return false;
}
