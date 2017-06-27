#include "WinFtpCommon.h"

#include "..\..\Public\DebugNew.h"
 

/*
ERROR_INTERNET_EXTENDED_ERROR 
12003

An extended error was returned from the server. This is typically a string or buffer containing a verbose error message. Call InternetGetLastResponseInfo to retrieve the error text.
*/

using namespace std;

void ShowLastError(const char* szFunction) 
{ 
	// Retrieve the system error message for the last-error code

	DWORD dw = GetLastError(); 
	CString strFuncName;
	strFuncName = szFunction;

	LPVOID lpMsgBuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0, NULL );

	// Display the error message and exit the process

	CString strError;
	strError.Format(_T("%s failed with error %d: %s"), 
				strFuncName.GetString(), dw, lpMsgBuf);

	_tprintf(_T("%s\n"), strError.GetString());

	LocalFree(lpMsgBuf);
}




HINTERNET OpenNet(void)
{
	return InternetOpen(_T("WinInet FTP"), 
				INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
}

HINTERNET FtpLogin(HINTERNET hInternet, CString strServer, int nPort, 
					CString strUser, CString strPassword)
{
	return InternetConnect(hInternet, strServer, nPort, 
		strUser.GetString(), strPassword.GetString(), 
		INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, NULL);
}


// dwFindFlags:
// 	INTERNET_FLAG_HYPERLINK
// 	INTERNET_FLAG_NEED_FILE
// 	INTERNET_FLAG_NO_CACHE_WRITE
// 	INTERNET_FLAG_RELOAD
// 	INTERNET_FLAG_RESYNCHRONIZE

int GetFtpFolders( HINTERNET hFtp, std::deque<CString>& dqFolders, DWORD dwFindFlags /*= INTERNET_FLAG_RELOAD*/ )
{
	WIN32_FIND_DATA dirInfo;
	HINTERNET       hFind;
	DWORD           dwRet;
	BOOL            retVal = FALSE;

	dqFolders.clear();

	hFind = FtpFindFirstFile( hFtp, TEXT( "*.*" ), 
		&dirInfo, dwFindFlags, 0 );
	if ( hFind == NULL )
	{
		if( ( dwRet = GetLastError( ) ) == ERROR_NO_MORE_FILES )
		{
			dwRet = 0;
		}
		return dwRet;
	}

	do
	{
		if (dirInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			dqFolders.push_back(dirInfo.cFileName);
		}
	} while( InternetFindNextFile( hFind, (LPVOID) &dirInfo ) );

	if( ( dwRet = GetLastError( ) ) == ERROR_NO_MORE_FILES )
	{
		dwRet = 0;
	}
	InternetCloseHandle(hFind);
	return dwRet;
}

int GetFtpFiles( HINTERNET hFtp, std::deque<CString>& dqFiles, DWORD dwFindFlags /*= INTERNET_FLAG_RELOAD*/ )
{
	WIN32_FIND_DATA dirInfo;
	HINTERNET       hFind;
	DWORD           dwRet;
	BOOL            retVal = FALSE;

	dqFiles.clear();

	hFind = FtpFindFirstFile( hFtp, TEXT( "*.*" ), 
		&dirInfo, dwFindFlags, 0 );
	if ( hFind == NULL )
	{
		dwRet = GetLastError();
		if( dwRet == ERROR_NO_MORE_FILES )
		{
			dwRet = 0;
		}
		return dwRet;
	}

	do
	{
		if (dirInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			continue;
		}
		dqFiles.push_back(dirInfo.cFileName);
	} while( InternetFindNextFile( hFind, (LPVOID) &dirInfo ) );

	dwRet = GetLastError();
	if( dwRet == ERROR_NO_MORE_FILES )
	{
		dwRet = 0;
	}
	InternetCloseHandle(hFind);

	return dwRet;
}

int ChangeFtpDir(HINTERNET hFtp, CString strFolderName)
{
	if( !FtpSetCurrentDirectory( hFtp, strFolderName ) )
	{
		return GetLastError();
	}

	return 0;
}

int CreateFtpDir(HINTERNET hFtp, CString strFolderName)
{
	if( !FtpCreateDirectory( hFtp, strFolderName.GetString() ) )
	{
		return GetLastError();
	}

	return 0;
}

int RemoveFtpDir( HINTERNET hFtp, CString strFolderName)
{
	if( !FtpRemoveDirectory( hFtp, strFolderName.GetString() ) )
	{
		return GetLastError();
	}

	return 0;
}

int GetFtpFile(HINTERNET hFtp, 
			   CString strFtpFileName, CString strLocalFileName, 
			   DWORD dwTransferType /*= FTP_TRANSFER_TYPE_BINARY*/ )
{
	dwTransferType |= INTERNET_FLAG_RELOAD;

	if( !FtpGetFile( hFtp, 
			strFtpFileName.GetString(), strLocalFileName.GetString(), 
			FALSE, FILE_ATTRIBUTE_NORMAL, dwTransferType, NULL ) )
	{
		return GetLastError();
	}

	return 0;
}

int PutFtpFile(HINTERNET hFtp, 
				CString strFtpFileName, CString strLocalFileName, 
				DWORD dwTransferType /*= FTP_TRANSFER_TYPE_BINARY */)
{

	if( !FtpPutFile( hFtp, 
			strLocalFileName.GetString(), 
			strFtpFileName.GetString(), 
			dwTransferType, NULL ) )
	{
		return GetLastError();
	}

	return 0;
}

int DeleteFtpFile(HINTERNET hFtp, CString strFtpFileName)
{
	if( !FtpDeleteFile( hFtp, strFtpFileName.GetString() ) )
	{
		return GetLastError();
	}

	return 0;
}

int RenameFtpFile(HINTERNET hFtp,
					CString strOldFileName, CString strNewFileName)
{
	if( !FtpRenameFile( hFtp, strOldFileName.GetString(), strNewFileName.GetString() ) )
	{
		return GetLastError();
	}

	return 0;
}


BOOL PrintFtpDir(HINTERNET hFtp,
				 DWORD dwFindFlags /*= INTERNET_FLAG_RELOAD*/)
{
	BOOL bRet = FALSE;
	deque<CString> dqNames;

	bRet = GetFtpFolders(hFtp, dqNames, dwFindFlags);

	if (bRet)
	{
		printf("GetFtpFolders error: %d", bRet);
	}
	else
	{
		_tprintf(_T("Folders: \n"));
		for (unsigned ii = 0, iLoop = dqNames.size();
			ii < iLoop; ii++)
		{
			_tprintf(_T("%s\n"), dqNames[ii].GetString());
		}
	}

	bRet = GetFtpFiles(hFtp, dqNames, dwFindFlags);

	if (bRet)
	{
		printf("GetFtpFiles error: %d", bRet);
	}
	else
	{
		_tprintf(_T("Files: \n"));
		for (unsigned ii = 0, iLoop = dqNames.size();
			ii < iLoop; ii++)
		{
			_tprintf(_T("%s\n"), dqNames[ii].GetString());
		}
	}

	return TRUE;
}
