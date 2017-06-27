#pragma once

#include <afx.h>
#include <WinInet.h>
#include <deque>

/************************************************************************/
/* 
very simple ftp class with MS WinINet Functions and CString.
ftp path support '/', '\' or mix style.
	eg. /root\level1/level2\file.bin
both relative and full path are supported (not fully tested).

CFtpClient has a working directory on ftp server.
all file or directory operations is made in working directory.
use ChangeFolder to change working directory.
ftpFullPath parameter must start with '/' or '\', or will add slash if possible.

copy constructor and operator= are not required.
*/
/************************************************************************/


class CFtpClient
{
public:
	CFtpClient(const CString username, 
				const CString password,
				const CString hostname,
				const int hostport = DEFAULT_FTP_PORT);

	~CFtpClient(void);

	// no copy constructor and operator=
private:
	CFtpClient(const CFtpClient& another){}
	CFtpClient& operator=(const CFtpClient& another){return *this;}

protected:
	// server parameter
	CString m_strUserName;
	CString m_strPassword;
	CString m_strHostName;
	int m_nHostPort;
	int m_nSleepMs;	// sleep milliseconds when check files

	// m$ handles
	HINTERNET hInternet; // internet handle for InternetOpen
	HINTERNET hFtp;		// ftp handle for InternetConnect

	// default parameter
	static const int DEFAULT_FTP_PORT = 21;			
	static const int DEFAULT_SLEEP_TIME_MS = 2000;	
	const CString DEFAULT_USER_ANGENT_NAME;
	const CString ERROR_LOGIN_FIRST;

	CString m_strMessage;
	

// 	/************************************************************************/
// 	/* Common functions                                                     */
// 	/************************************************************************/

protected:
	// Set message when function failed, use GetLastError and InternetGetLastResponseInfo
	void SetFtpMessage(CString strFunctionName);

public:
	// Get ftp message when function failed.
	CString GetFtpMessage(void)
	{
		return m_strMessage;
	}

	bool Login(void);
	void Logout(void);

	//************************************
	// list files and folders
	// dwFindFlags:
	// 	INTERNET_FLAG_HYPERLINK
	// 	INTERNET_FLAG_NEED_FILE
	// 	INTERNET_FLAG_NO_CACHE_WRITE
	// 	INTERNET_FLAG_RELOAD
	// 	INTERNET_FLAG_RESYNCHRONIZE
	//************************************
	bool ListAll(std::deque<CString>& dqFiles, std::deque<CString>& dqFolders, DWORD   dwFindFlags= INTERNET_FLAG_RELOAD);

	//************************************
	// Get file and folder names string
	// Folders:
	//	folder names
	// Files:
	//	file names
	//************************************
	CString GetListString(DWORD  dwFindFlags= INTERNET_FLAG_RELOAD);

	bool ChangeFolder(CString strFolderName);

	bool CreateFolder(CString strFolderName);

	// Remove will fail if folder is not empty.
	bool RemoveFolder(CString strFolderName);

	// local file will be overwrote when exist.
	bool Download(CString strFtpFileName, CString strLocalFileName, 
				DWORD  dwTransferType= FTP_TRANSFER_TYPE_BINARY);
	
	// remote file will be overwrote when exist.
	bool Upload(CString strLocalFileName, CString strFtpFileName, 
				DWORD dwTransferType= FTP_TRANSFER_TYPE_BINARY);
	
	bool DeleteFtpFile(CString strFtpFileName);

	bool RenameFile(CString strOldFileName, CString strNewFileName);

// 	/************************************************************************/
// 	/* Special purpose functions                                            */
// 	/************************************************************************/

private:
	// check if ftpFullPath is start from root ( \ or / ), add slash is possible.
	bool CheckFullPath(CString& strSlash, CString& ftpFullPath)
	{
		const TCHAR slashFtp = ftpFullPath.GetAt(0);
		if (slashFtp == '/')
		{
			strSlash = _T("/");
		}
		else if (slashFtp == '\\')
		{
			strSlash = _T("\\");
		}
		else if (isalnum(slashFtp))
		{
			strSlash = _T("/");
			ftpFullPath = strSlash + ftpFullPath;
		}
		else
		{
			return false;
		}

		return true;
	}

public:
	//************************************
	// check, download and delete files from remoteDir to localPath. 
	// bExit: CheckFile will loop until bExit is true
	// ftpFullPath: full path start with '/' or '\'
	//************************************
	bool CheckFile(volatile LONG &bExit, CString ftpFullPath, const CString localPath);

	// download and delete file in ftpFullPath, local path will automatically create
	bool DownloadAndDelete(CString ftpFullPath, CString ftpFileName, CString strLocalFullName);

	bool DeleteDir(CString strDirName)
	{
		return RemoveFolder(strDirName);
	}
};
