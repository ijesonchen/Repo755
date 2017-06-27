// WinFtpClient.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "WinFtpClient.h"

#include "..\..\Public\DebugNew.h"

#include "WinFtpCommon.h"
#include <string.h>
#include "FtpClient.h"

#include <deque>
using namespace std;


// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

DWORD WINAPI ExitTimer(LPVOID pParam)
{
	LONG* pExit = (LONG*)pParam;
	Sleep(5000);
	printf("Timed out.");
	*pExit = true;
	return 0;
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// 初始化 MFC 并在失败时显示错误
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: MFC 初始化失败\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: 在此处为应用程序的行为编写代码。
	}

	setlocale(LC_ALL, "chs");

	const CString strServer(_T("dellserver1"));
	const int nPort = 21;
	const CString strUser(_T("ftpuser"));
	const CString strPassword(_T("ftppwd"));

	const CString strLocalRoot(_T("D:\\Data\\ftp"));

	const CString strUploadFile(_T("D:\\Data\\feaweight2.txt"));

	int nThrow = 0;

	HINTERNET hNet = NULL;
	HINTERNET hFtp = NULL;
	deque<CString> dqFolders;
	deque<CString> dqFiles;
	int nRet = 0;
	bool bRet = false;

	//////////////////////////////////////////////////////////////////////////
	// class test
	CFtpClient ftpClient(strUser, strPassword, strServer);
	ftpClient.Login();
	CString strList;
	strList = ftpClient.GetListString();


	bRet = ftpClient.DownloadAndDelete(_T("123"), _T("aa.txt"), _T("D:\\Data\\ftpdownl\\feaweight2.txt"));


	bRet = ftpClient.ChangeFolder(_T("456\\999/xxx"));
	strList = ftpClient.GetListString();

	CString strRemotePath(_T("/test"));

	LONG bExit = false;

	HANDLE hExitTimer = CreateThread(NULL, NULL, ExitTimer, &bExit, NULL, NULL);
	bRet = ftpClient.CheckFile(bExit, strRemotePath, strLocalRoot) ;
	CloseHandle(hExitTimer);
	hExitTimer = NULL;

	//////////////////////////////////////////////////////////////////////////
	// function test
	try
	{
		hNet = OpenNet();
		if (!hNet)
		{
			printf("error: %s\n", FtpLogin);
			nThrow = 3;
			throw nThrow;
		}

		hFtp = FtpLogin(hNet, strServer, nPort, strUser, strPassword);

		if (!hFtp)
		{
			printf("error: %s\n", FtpLogin);
			nThrow = 1;
			throw nThrow;
		}

		//////////////////////////////////////////////////////////////////////////
		// InternetGetLastResponseInfo

		nRet = GetFtpFile(hFtp, _T("456\\999\\aa.rtf"), strLocalRoot + _T("\\2a.rtf") );

		nRet = GetLastError();
		nRet = GetLastError();
		nRet = GetLastError();
		if (nRet == ERROR_INTERNET_EXTENDED_ERROR)
		{
			DWORD nBufferLength = 0;
			DWORD dwInternet = 0;
			TCHAR* pBuffer = NULL;
			BOOL bRet = InternetGetLastResponseInfo(&dwInternet, pBuffer, &nBufferLength);
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			{
				int nLen = nBufferLength;
				pBuffer = new TCHAR[nBufferLength];
				bRet = InternetGetLastResponseInfo(&dwInternet, pBuffer, &nBufferLength);
				delete pBuffer;
				pBuffer = NULL;
			}
		}

		nRet = GetLastError();

		//////////////////////////////////////////////////////////////////////////
		// with full or relative path
		printf("+++456/999 dir\n");
		nRet = ChangeFtpDir(hFtp, _T("456/999"));
		nRet = PrintFtpDir(hFtp);


		nRet = ChangeFtpDir(hFtp, _T("/"));
		nRet = PrintFtpDir(hFtp);
		nRet = RenameFtpFile(hFtp, _T("456\\999\\1a.rtf"), _T("456\\999\\2a.rtf"));

		nRet = ChangeFtpDir(hFtp, _T("456"));
		nRet = PrintFtpDir(hFtp);
		nRet = RenameFtpFile(hFtp, _T("999\\2a.rtf"), _T("999\\3a.rtf"));
		nRet = CreateFtpDir(hFtp, _T("999\\xxx"));

		nRet = ChangeFtpDir(hFtp, _T("/"));
		nRet = GetFtpFile(hFtp, _T("456\\999\\3a.rtf"), strLocalRoot + _T("\\2a.rtf") );

		// with full or relative path
		//////////////////////////////////////////////////////////////////////////


		nRet = PutFtpFile(hFtp, _T("upload.dat"), strUploadFile);
		nRet = PutFtpFile(hFtp, _T("upload.dat"), strUploadFile);

		nRet = RenameFtpFile(hFtp, _T("t.mp3"), _T("w.www"));

		printf("+++root dir\n");
		nRet = PrintFtpDir(hFtp);

		printf("+++change to 456\n");
		nRet = ChangeFtpDir(hFtp, _T("456"));

		nRet = PrintFtpDir(hFtp);

		printf("+++change to /\n");
		nRet = ChangeFtpDir(hFtp, _T("/"));

		nRet = PrintFtpDir(hFtp);

		printf("+++change to 123\n");
		nRet = ChangeFtpDir(hFtp, _T("123"));

		nRet = PrintFtpDir(hFtp);

		nRet = GetFtpFiles(hFtp, dqFiles);

		for (unsigned ii = 0, iLoop = dqFiles.size();
			ii < iLoop; ii++)
		{
			nRet = GetFtpFile(hFtp, dqFiles[ii], strLocalRoot + _T("\\") + dqFiles[ii]);
		}


		printf("+++change to .. and create\n");
		nRet = ChangeFtpDir(hFtp, _T(".."));
		nRet = CreateFtpDir(hFtp, _T("createtest"));
		nRet = PrintFtpDir(hFtp);

		printf("+++change to 456\n");
		nRet = ChangeFtpDir(hFtp, _T("123"));
		nRet = PrintFtpDir(hFtp);

		nRet = GetFtpFiles(hFtp, dqFiles);
		for (unsigned ii = 0, iLoop = dqFiles.size();
			ii < iLoop; ii++)
		{
			nRet = DeleteFtpFile(hFtp, dqFiles[ii]);
		}
		nRet = GetFtpFolders(hFtp, dqFolders);

		for (unsigned ii = 0, iLoop = dqFolders.size();
			ii < iLoop; ii++)
		{
			nRet = RemoveFtpDir(hFtp, dqFolders[ii]);
			if (nRet)
			{
				nRet = ChangeFtpDir(hFtp, dqFolders[ii]);
				nRet = GetFtpFiles(hFtp, dqFiles);
				for (unsigned jj = 0, jLoop = dqFiles.size();
					jj < jLoop; jj++)
				{
					nRet = DeleteFtpFile(hFtp, dqFiles[jj]);
				}
				nRet = ChangeFtpDir(hFtp, _T(".."));
				nRet = RemoveFtpDir(hFtp, dqFolders[ii]);

			}
		}
	}
	catch (int x)
	{
		printf("Throw code: %d", x);
	}
	catch (CException* e)
	{
		e->Delete();
	}

	if (hNet)
	{
		InternetCloseHandle(hNet);
		hNet = NULL;
	}
	if (hFtp)
	{
		InternetCloseHandle(hFtp);
		hFtp = NULL;
	}

	// login loop
	int ii = 0;
	bool bRunning = true;
	while (bRunning)
	{
		Sleep(1000);
		printf("\n++++++++++++++++++++++++\nlogin loop: %d\n\n\n", ++ii);

		hNet = OpenNet();
		if (!hNet)
		{
			printf("error: %s\n", OpenNet);
			continue;
		}

		hFtp = FtpLogin(hNet, strServer, nPort, strUser, strPassword);
		if (!hFtp)
		{
			InternetCloseHandle(hNet);
			hNet = NULL;
			printf("error: %s\n", FtpLogin);
			continue;
		}

		nRet = PrintFtpDir(hFtp);

		InternetCloseHandle(hNet);
		hNet = NULL;

		InternetCloseHandle(hFtp);
		hFtp = NULL;
	}

	return nRetCode;
}
