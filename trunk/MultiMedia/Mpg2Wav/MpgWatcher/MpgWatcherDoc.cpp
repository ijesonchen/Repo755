
// MpgWatcherDoc.cpp : CMpgWatcherDoc 类的实现
//

#include "stdafx.h"
#include "MpgWatcher.h"

#include "MpgWatcherDoc.h"

#include "CommonFunction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMpgWatcherDoc

IMPLEMENT_DYNCREATE(CMpgWatcherDoc, CDocument)

BEGIN_MESSAGE_MAP(CMpgWatcherDoc, CDocument)
	ON_COMMAND(ID_WATCHER_START, &CMpgWatcherDoc::OnWatcherStart)
	ON_COMMAND(ID_WATCHER_STOP, &CMpgWatcherDoc::OnWatcherStop)
END_MESSAGE_MAP()


// CMpgWatcherDoc 构造/析构

CMpgWatcherDoc::CMpgWatcherDoc()
: m_bRunning(false)
{
	// TODO: 在此添加一次性构造代码

}

CMpgWatcherDoc::~CMpgWatcherDoc()
{
	DeleteCriticalSection(&m_csInputFile);
}

BOOL CMpgWatcherDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	reinterpret_cast<CEditView*>(m_viewList.GetHead())->SetWindowText(NULL);

	// read ini
	TCHAR szModuleFile[MAX_PATH];
	if (GetModuleFileName(NULL, szModuleFile, MAX_PATH) == 0)
	{
		WiiLog(_T("Error: GetModuleFileName"));
		return TRUE;
	}
	CString strModuleFile = szModuleFile;
	int nRetFind = strModuleFile.ReverseFind(_T('\\'));
	if (nRetFind == -1)
	{
		WiiLog(_T("Error: ReverseFind"));
		return TRUE;
	}

	CString strConfigFile = strModuleFile.Left(nRetFind + 1) + AfxGetAppName() + _T(".ini");
	if (!m_Param.ReadIniFile(strConfigFile))
	{
		WiiLog(_T("Error: ReadIniFile"));
	}
	else
	{
		WiiLog(kSystemLogType, _T("Parameters:%s"),m_Param.GetDetailInfo());
	}

	InitializeCriticalSectionAndSpinCount(&m_csInputFile, 0x400);
	return TRUE;
}




// CMpgWatcherDoc 序列化

void CMpgWatcherDoc::Serialize(CArchive& ar)
{
	reinterpret_cast<CEditView*>(m_viewList.GetHead())->SerializeRaw(ar);
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CMpgWatcherDoc 诊断

#ifdef _DEBUG
void CMpgWatcherDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMpgWatcherDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMpgWatcherDoc 命令

BOOL CMpgWatcherDoc::SaveModified()
{
	// TODO: Add your specialized code here and/or call the base class

	return TRUE;
}

void CMpgWatcherDoc::OnWatcherStart()
{
	// TODO: Add your command handler code here
	if (!CommonFunction::EnsureDir(m_Param.w_strOutputDirectory))
	{
		WiiLog(_T("输出路径不存在：%s"), m_Param.w_strOutputDirectory);
		return;
	}

	if (m_bRunning)
	{
		WiiLog(_T("正在监控."));
		return;
	}

	m_bRunning = true;

	m_hWatcherThread = CreateThread(NULL,
		NULL,
		(LPTHREAD_START_ROUTINE)WatcherThread,
		this,
		NULL,
		NULL);
	if (!m_hWatcherThread)
	{
		WiiLog(_T("文件监控线程创建失败。"));
		m_bRunning = false;
		return;
	}

	m_hConvertThread = CreateThread(NULL,
							NULL,
							(LPTHREAD_START_ROUTINE)ConvertThread,
							this,
							NULL,
							NULL);
	if (!m_hConvertThread)
	{
		WiiLog(_T("文件转换线程创建失败。"));
		m_bRunning = false;
		return;
	}
}

void CMpgWatcherDoc::OnWatcherStop()
{
	// TODO: Add your command handler code here
	if (m_bRunning)
	{
		m_bRunning = false;
		WiiLog(_T("等待转换线程退出..."));
		if (WaitForSingleObject(m_hConvertThread, WAIT_WATCHER_THREAD_EXIT))
		{
			WiiLog(_T("强制退出转换线程。"));
			TerminateThread(m_hConvertThread, -1);
		}
		m_hConvertThread = NULL;

		WiiLog(_T("等待监控线程退出..."));
		if (WaitForSingleObject(m_hConvertThread, WAIT_WATCHER_THREAD_EXIT))
		{
			WiiLog(_T("强制退出监控线程。"));
			TerminateThread(m_hConvertThread, -1);
		}
		m_hConvertThread = NULL;
	}
	WiiLog(kNormalLogType, _T("线程已停止。"));
}

UINT WINAPI CMpgWatcherDoc::ConvertThread( LPVOID pContext )
{
	CMpgWatcherDoc* pDoc = (CMpgWatcherDoc*)pContext;
	CParam& EnvParam = pDoc->m_Param;
	CRITICAL_SECTION& csInputFiles = pDoc->m_csInputFile;
	while(pDoc->m_bRunning)
	{
		if (!pDoc->m_dqInputFiles.size())
		{
			continue;
		}

		CString strCmd;
		CString strInput;
		CString strOutput;

		EnterCriticalSection(&csInputFiles);
		strInput = pDoc->m_dqInputFiles.front();
		pDoc->m_dqInputFiles.pop_front();
		LeaveCriticalSection(&csInputFiles);

		CString strFileNameNoExt;
		strFileNameNoExt = CommonFunction::GetFileNameByPathNoExt(strInput);
		strOutput.Format(_T("%s\\%s.wav"),
						EnvParam.w_strOutputDirectory,
						strFileNameNoExt);
		
		strCmd.Format(_T("%s %s %s"),
						EnvParam.w_strConverter,
						strInput,
						strOutput);
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
			WiiLog(_T("%s创建转换进程失败"), strInput);
		}
		else
		{
			WaitForSingleObject(pi.hProcess, INFINITE);
			DWORD dwProcessExitCode = 0;
			BOOL bRet = GetExitCodeProcess(pi.hProcess, &dwProcessExitCode);
			if (dwProcessExitCode && dwProcessExitCode != 0xc0000005)
			{
				WiiLog(_T("%s转换失败：0x%x"), strInput, dwProcessExitCode);
			}
			else
			{
				WiiLog(kNormalLogType, _T("%s转换完成"), strInput);

				// write interface ini files
				// not complete
				CString strStartTime;
				CString strEndTime;
				strStartTime = _T("2003-04-10 09:20:03");
				strEndTime = _T("2003-04-10 09:25:18");
				CString strInterfaceBegin;
				CString strInterfaceEnd;
				strInterfaceBegin.Format(_T("1；0；%s；%s；100；291806；800001；%s；%s；03793655239；02162340347；0；电话；正常；；"),
					strStartTime,
					strEndTime,
					strOutput,
					strFileNameNoExt);
				strInterfaceEnd.Format(_T("1；1；%s；%s；100；291806；800001；%s；%s；03793655239；02162340347；0；电话；正常；；"),
					strStartTime,
					strEndTime,
					strOutput,
					strFileNameNoExt);
				CommonFunction::WriteLineToTxt(EnvParam.w_strInterfaceFile, strInterfaceBegin);
				CommonFunction::WriteLineToTxt(EnvParam.w_strInterfaceFile, strInterfaceEnd);
			}
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
		}
	}
	return 0;
}

UINT CMpgWatcherDoc::WatcherThread(LPVOID pContext)
{
	CMpgWatcherDoc* pDoc = (CMpgWatcherDoc*) pContext;
	CString strDir;
	strDir = pDoc->m_Param.w_strWatcherDirectory;

	HANDLE hDir = NULL;
	hDir = CreateFile(strDir, 
		GENERIC_READ, 
		FILE_SHARE_READ || FILE_SHARE_WRITE || FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS,
		NULL);

	if (INVALID_HANDLE_VALUE == hDir)
	{
		WiiLog(L"CreateFile error: %s\n", strDir);
	}

	int nMaxFileLen = MAX_FILE_NAME_LENGTH;
	wchar_t* strFileNameW = new wchar_t[nMaxFileLen];

	int nBufferSize = 1000;
	DWORD nInfoRetByte = 0;

	char* pBuffer = new char[nBufferSize];

	nInfoRetByte = sizeof(FILE_NOTIFY_INFORMATION);

	while(pDoc->m_bRunning &&
		ReadDirectoryChangesW(hDir,
								pBuffer,
								nBufferSize - 2,
								false,
								FILE_NOTIFY_CHANGE_FILE_NAME, 
								&nInfoRetByte,
								NULL, 
								NULL))
	{
		EnterCriticalSection(&pDoc->m_csInputFile);
		char* pNext = pBuffer;
		FILE_NOTIFY_INFORMATION* pCurrent = (FILE_NOTIFY_INFORMATION*)pNext;
		do
		{

			if (FILE_ACTION_ADDED == pCurrent->Action )
			{
				memset(strFileNameW, 0, nMaxFileLen * sizeof(wchar_t));
				memcpy(strFileNameW, 
					pCurrent->FileName, 
					pCurrent->FileNameLength);

				CString strInput;
				strInput.Format(_T("%s\\%s"), 
							pDoc->m_Param.w_strWatcherDirectory, 
							strFileNameW);
				pDoc->m_dqInputFiles.push_back(strInput);

				pNext += pCurrent->NextEntryOffset;
				pCurrent = (FILE_NOTIFY_INFORMATION*)pNext;
			}
		} while (pCurrent->NextEntryOffset);
		LeaveCriticalSection(&pDoc->m_csInputFile);
	}

	delete pBuffer;
	delete strFileNameW;

	return 0;
}
