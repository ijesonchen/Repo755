
// Hzmv2PcmMT_DlgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Hzmv2PcmMT_Dlg.h"
#include "Hzmv2PcmMT_DlgDlg.h"
#include "VoiceRecTask.h"

#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CHzmv2PcmMT_DlgDlg dialog




CHzmv2PcmMT_DlgDlg::CHzmv2PcmMT_DlgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHzmv2PcmMT_DlgDlg::IDD, pParent)
	, w_bCoInited(false)
	, w_bProcessQuit(false)
	, m_bList1(true)
	, m_pListA(NULL)
	, m_pListB(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (S_OK != hr)
	{
		::MessageBox(NULL, _T("COINIT ERROR"), _T(""), MB_OK);
	}
	else
	{
		w_bCoInited = true;
	}
}

void CHzmv2PcmMT_DlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSG_LIST1, m_List1);
	DDX_Control(pDX, IDC_MSG_LIST2, m_List2);
}

BEGIN_MESSAGE_MAP(CHzmv2PcmMT_DlgDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_STARTPROC, &CHzmv2PcmMT_DlgDlg::OnBnClickedStartproc)
END_MESSAGE_MAP()


// CHzmv2PcmMT_DlgDlg message handlers

BOOL CHzmv2PcmMT_DlgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	m_pListA = &m_List1;
	m_pListB = &m_List2;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CHzmv2PcmMT_DlgDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CHzmv2PcmMT_DlgDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHzmv2PcmMT_DlgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CHzmv2PcmMT_DlgDlg::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	if (w_bCoInited)
	{
		CoUninitialize();
		w_bCoInited = false;
	}

	CDialog::PostNcDestroy();
}

void CHzmv2PcmMT_DlgDlg::OnBnClickedStartproc()
{
	// TODO: Add your control notification handler code here
// 	static int n = 0;
// 	CString str1;
// 	str1.Format(_T("str %d"), n++);
// 	WriteMessage(str1);
// 
// 	return;

	//枚举文件
	CString strSrcDir = INPUT_DIR;
	CString strDstDir = OUTPUT_DIR;
	if (!EnumSrcFiles(strSrcDir, _T("*.*")))
	{
		CString str;
		str.Format(_T("%s为空目录"), strSrcDir);
		WriteMessage(str);
		return;
	}
	//检查输出目录是否存在
	if (!CheckOutputDirSilent(strDstDir))
	{
		CString str;
		str.Format(_T("%s为空目录"), strDstDir);
		WriteMessage(str);
		return;
	}

	CSingleLock lock(&w_csTasks, TRUE);
	for (std::deque<CString>::iterator it = w_dqStrSrcFiles.begin(); it != w_dqStrSrcFiles.end(); ++it)
	{
		CSharePtr<ITask> spTask(new CVoiceRecTask(*it));
		w_dqSpTasks.push_back(spTask);
	}
	size_t nCurDqSize = w_dqSpTasks.size();
	lock.Unlock();
	CString str;
	str.Format(_T("主线程--已添加预处理任务，当前队列有%d个任务"), nCurDqSize);
	WriteMessage(str);


	int nCurThreadID = 0;
	unsigned int nCurThread = (unsigned)(w_dqSpWinThread.size());
	if (nCurThread > 0)
	{
		nCurThreadID = w_dqSpTrainThread.back()->w_nID;
	}

	if (MAX_THREAD_COUNT > nCurThread)
	{
		InterlockedExchange(&w_nRunningThread, MAX_THREAD_COUNT);
		for (unsigned int i = nCurThread; i < MAX_THREAD_COUNT; ++i)
		{
			CWorkerThread *pTrain = new CWorkerThread(this, ++nCurThreadID);
			w_dqSpTrainThread.push_back(pTrain);
			CWinThread *pWinThread = AfxBeginThread(CWorkerThread::TrainThreadFun, pTrain);
			pWinThread->m_bAutoDelete = FALSE;
			w_dqSpWinThread.push_back(pWinThread);
		}
	}
	else
	{
		for (unsigned int i = MAX_THREAD_COUNT; i < nCurThread; ++i)
		{
			w_dqSpTrainThread[i]->SetExitFlag(TRUE);
		}
	}
}

void CHzmv2PcmMT_DlgDlg::WriteMessage(CString strMsg)
{
	int nListCount = m_pListA->GetCount();
	int nMaxMsg = MAX_MESSAGE_IN_LIST;
	if (nListCount >= nMaxMsg)
	{
		// switch A B list pointer
		m_pListB->ResetContent();
		if (m_bList1)
		{
			m_pListA = &m_List2;
			m_pListB = &m_List1;
			m_bList1 = false;
		}
		else
		{
			m_pListA = &m_List1;
			m_pListB = &m_List2;
			m_bList1 = true;
		}
	}
	if (WRITE_LATEST_TOP)
	{
		m_pListA->InsertString(0, strMsg);
	}
	else
	{
		m_pListA->AddString(strMsg);
	}
}

bool CHzmv2PcmMT_DlgDlg::EnumSrcFiles(const CString& strFileDir, const CString strFilter)
{
	struct SFileNameAndLen
	{
		CString w_strFileName;
		DWORD w_nFileLen;
	};
	struct SGetFileLen
	{
		DWORD operator()(const CString& strFile)
		{
			WIN32_FILE_ATTRIBUTE_DATA fileAttributeData;
			::GetFileAttributesEx(strFile, GetFileExInfoStandard, &fileAttributeData);
			return fileAttributeData.nFileSizeLow;
		}
	};
	struct SEnumDir
	{
		void operator()(const CString& strFileDir, const CString strFilter, std::deque<SFileNameAndLen>& dqFileNameAndLen)
		{
			CFileFind finder;
			CString strFind;
			strFind = strFileDir + _T("\\") + strFilter;
			BOOL bFind = finder.FindFile(strFind);
			while (bFind)
			{
				bFind = finder.FindNextFile();
				if(finder.IsDots())
				{
					continue;
				}
				if (finder.IsDirectory())
				{
					CString strSubDir = finder.GetFilePath();
					SEnumDir()(strSubDir, strFilter, dqFileNameAndLen);
				}
				else
				{
					SFileNameAndLen fileNameLen;
					fileNameLen.w_strFileName = finder.GetFilePath();
					fileNameLen.w_nFileLen = SGetFileLen()(fileNameLen.w_strFileName);
					dqFileNameAndLen.push_back(fileNameLen);
				}
			}
			finder.Close();
		}
	};
	struct SSFileNameAndLenLager
	{
		bool operator()(const SFileNameAndLen& a, const SFileNameAndLen& b)
		{
			return a.w_nFileLen > b.w_nFileLen;
		}
	};

	//遍历目录
	std::deque<SFileNameAndLen> dqFileNameAndLen;
	SEnumDir()(strFileDir, strFilter, dqFileNameAndLen);
	if (dqFileNameAndLen.empty())
	{
		return false;
	}
	//排序
//	std::sort(dqFileNameAndLen.begin(), dqFileNameAndLen.end(), SSFileNameAndLenLager());
	//回写
	w_dqStrSrcFiles.clear();
	for (std::deque<SFileNameAndLen>::iterator it = dqFileNameAndLen.begin(); it != dqFileNameAndLen.end(); ++it)
	{
		w_dqStrSrcFiles.push_back(it->w_strFileName);
	}
	return true;
}

bool CHzmv2PcmMT_DlgDlg::CheckOutputDirSilent(const CString& strOutputDir)
{
	DWORD dwRet = ::GetFileAttributes(strOutputDir);
	if ( dwRet == INVALID_FILE_ATTRIBUTES || (dwRet & FILE_ATTRIBUTE_DIRECTORY) == 0) 
	{
		if (ERROR_SUCCESS != ::SHCreateDirectoryEx(NULL, strOutputDir, NULL))//创建目录
		{
			CString str;
			str.Format(_T("目录%s创建失败"), strOutputDir);
			WriteMessage(str);
			return false;
		}
	}
	return true;
}