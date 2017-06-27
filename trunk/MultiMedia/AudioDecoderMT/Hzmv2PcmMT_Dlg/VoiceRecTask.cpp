#include "stdafx.h"
#include "WorkerThread.h"
#include "Hzmv2PcmMT_DlgDlg.h"
#include "VoiceRecTask.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "CommonFunction.h"
using namespace CommonFunction;


CString GetFileExt(const CString strFileName)
{
	int nIdx;
	nIdx = strFileName.ReverseFind('.');
	int nLength = strFileName.GetLength();

	if (nIdx > -1)
	{
		return strFileName.Right(nLength - nIdx - 1);
	}
	else
	{
		return _T("");
	}
}

int CVoiceRecTask::Run( LPVOID pParam )
{
	CWorkerThread *pThread = (CWorkerThread *)pParam;
	CHzmv2PcmMT_DlgDlg *pDoc = pThread->w_pDoc;

	CString strOut;
	strOut.Format(_T("%s\\%s.wav"),
			OUTPUT_DIR, GetFileNameByPathNoExt(w_strVoiceFile));

	CString strCmd;
// 	CString strFileExt;
// 	strFileExt = GetFileExt(w_strVoiceFile);
// 	if (!strFileExt.CompareNoCase(_T("hzmv")))
// 	{
// 		strCmd.Format(_T("Hzmv2PcmWin32_Release_Win32.exe \"%s\" \"%s\""),
// 			w_strVoiceFile, strOut);
// 	}
// 	else if (!strFileExt.CompareNoCase(_T("amr")))
// 	{
// 		strCmd.Format(_T("AmrDecoderWin32_Release_Win32.exe \"%s\" \"%s\""),
// 			w_strVoiceFile, strOut);
// 	}
// 	else
// 	{
// 		// format not supported.
// 		return -1;;
// 	}

	strCmd.Format(_T("AmrDecoderWin32_Debug_Win32.exe \"%s\" \"%s\""),
		w_strVoiceFile, strOut);


	STARTUPINFO st;
	ZeroMemory(&st, sizeof(STARTUPINFO));
	st.cb = sizeof (STARTUPINFO);
	st.lpReserved = NULL;
	st.lpDesktop = NULL;
	st.wShowWindow = SW_HIDE;
	st.cbReserved2 = NULL;
	st.lpReserved2 = NULL;
	st.dwFlags = STARTF_USESHOWWINDOW;

	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	if (!CreateProcess(NULL, strCmd.GetBuffer(), NULL, NULL,FALSE, 0, NULL,NULL,&st, &pi))
	{
		CString str;
		str.Format(_T("失败：%s"), w_strVoiceFile);
		pDoc->WriteMessage(str);
		return 1;
	}
	WaitForSingleObject(pi.hProcess, INFINITE);
	DWORD dwProcessExitCode = 0;
	GetExitCodeProcess(pi.hProcess, &dwProcessExitCode);
	if (0 != dwProcessExitCode)
	{
		CString str;
		str.Format(_T("失败：%s"), w_strVoiceFile);
		pDoc->WriteMessage(str);
	}
	else
	{
		CString str;
		str.Format(_T("成功：%s"), w_strVoiceFile);
		pDoc->WriteMessage(str);
	}

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	return dwProcessExitCode;



	//////////////////////////////////////////////////////////////////////////
	pDoc->WriteMessage(w_strVoiceFile);
	pDoc->WriteMessage(strOut);
	Sleep(500);

	return true;
}


void CVoiceRecTask::WriteLog(LPVOID pParam, size_t nDqSize)
{
	CWorkerThread *pThread = (CWorkerThread *)pParam;

	CString strMsg;
	strMsg.Format(_T("线程%d--获取一个识别任务，队列中有%d个任务"), pThread->w_nID, nDqSize);
	pThread->w_pDoc->WriteMessage(strMsg);
}
