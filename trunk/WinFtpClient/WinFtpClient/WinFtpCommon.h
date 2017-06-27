
#include <afx.h>
#include "wininet.h"
#include <deque>

void ShowLastError(const char* szFunction);

HINTERNET OpenNet(void);

HINTERNET FtpLogin(HINTERNET hInternet, CString strServer, int nPort, 
						  CString strUser, CString strPassword);

// dwFindFlags:
// 	INTERNET_FLAG_HYPERLINK
// 	INTERNET_FLAG_NEED_FILE
// 	INTERNET_FLAG_NO_CACHE_WRITE
// 	INTERNET_FLAG_RELOAD
// 	INTERNET_FLAG_RESYNCHRONIZE

int GetFtpFolders(HINTERNET hFtp, std::deque<CString>& dqFolders,
				  DWORD dwFindFlags = INTERNET_FLAG_RELOAD);

int GetFtpFiles( HINTERNET hFtp, std::deque<CString>& dqFiles, 
				DWORD dwFindFlags = INTERNET_FLAG_RELOAD);

// with full and relative path
int ChangeFtpDir(HINTERNET hFtp, CString strFolderName);

// with full and relative path, parent path must be exist.
int CreateFtpDir(HINTERNET hFtp, CString strFolderName);

// folder must be empty
int RemoveFtpDir( HINTERNET hFtp, CString strFolderName);

int GetFtpFile(HINTERNET hFtp, 
			   CString strFtpFileName, CString strLocalFileName, 
			   DWORD dwTransferType = FTP_TRANSFER_TYPE_BINARY );

int PutFtpFile(HINTERNET hFtp, 
			   CString strFtpFileName, CString strLocalFileName, 
			   DWORD dwTransferType = FTP_TRANSFER_TYPE_BINARY );

int DeleteFtpFile(HINTERNET hFtp, CString strFtpFileName);

int RenameFtpFile(HINTERNET hFtp,
				  CString strOldFileName, CString strNewFileName);


BOOL PrintFtpDir(HINTERNET hFtp,
				 DWORD dwFindFlags = INTERNET_FLAG_RELOAD);
