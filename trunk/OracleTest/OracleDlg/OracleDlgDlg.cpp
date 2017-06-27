
// OracleDlgDlg.cpp : implementation file
//


#include "stdafx.h"
#include "OracleDlg.h"
#include "OracleDlgDlg.h"

#include "CommonFunction.h"
using namespace CommonFunction;

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


// COracleDlgDlg dialog




COracleDlgDlg::COracleDlgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COracleDlgDlg::IDD, pParent)
	, m_pConnection(NULL)
	, m_pCmd(NULL)
	, m_pRecord(NULL)
	, m_nVocTaskIndex(0)
	, m_bConnected(false)
	, m_bTaskRead(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COracleDlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COracleDlgDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CONNECTDB, &COracleDlgDlg::OnBnClickedConnectdb)
	ON_BN_CLICKED(IDC_READ_TASK_FILE, &COracleDlgDlg::OnBnClickedReadTaskFile)
	ON_BN_CLICKED(IDC_WRITE_DB, &COracleDlgDlg::OnBnClickedWriteDb)
	ON_BN_CLICKED(IDC_SKIP_TASK, &COracleDlgDlg::OnBnClickedSkipTask)
END_MESSAGE_MAP()


// COracleDlgDlg message handlers

BOOL COracleDlgDlg::OnInitDialog()
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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void COracleDlgDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void COracleDlgDlg::OnPaint()
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
HCURSOR COracleDlgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void COracleDlgDlg::OnBnClickedConnectdb()
{
	// TODO: Add your control notification handler code here
	CoInitialize(NULL);
	_RecordsetPtr pRet;

	if (FAILED(m_pConnection.CreateInstance(__uuidof(Connection))))
	{
		MessageBox(_T("m_pConnection"));
	}
	if (FAILED(m_pCmd.CreateInstance(__uuidof(Command))))
	{
		MessageBox(_T("m_pCmd"));
	}		
	if (FAILED(m_pRecord.CreateInstance(__uuidof(Recordset))))
	{
		MessageBox(_T("m_pRecord"));
	}
	CString strConn = _T("Provider=OraOLEDB.Oracle.1;Persist Security Info=True;\
						 Data Source=\
						 (DESCRIPTION = \
						 (ADDRESS = (PROTOCOL = TCP)(HOST = dellserver1)(PORT = 1521) )\
						 (CONNECT_DATA = (SERVER = DEDICATED)(SERVICE_NAME = test1) )\
						 )");
	CString strUser = _T("scott");
	CString strPwd = _T("aA123456aA");
	CString strInsert; 
	CString strSelect = _T("select * from tblvoicetest");

	_variant_t vRet;

	try
	{
		m_pConnection->Open(strConn.AllocSysString(), strUser.AllocSysString(), strPwd.AllocSysString(), adModeUnknown);

		m_pCmd->ActiveConnection = m_pConnection;
		
		m_bConnected = true;
		SetDlgItemText(IDC_MSG_INFO, _T("ConnOK"));
	}
	catch (_com_error &e)
	{
		wchar_t errmsg[500];
		wsprintf(errmsg, _T("ErrorNo: 0x%x\nError Message:%s\nError Source:%s\nError Description:%s\n"), 
			e.Error(), 
			e.ErrorMessage(), 
			(LPCTSTR)e.Source(), 
			(LPCTSTR)e.Description());
		SetDlgItemText(IDC_MSG_INFO, errmsg);
	//	MessageBox(errmsg);
	}
}

void COracleDlgDlg::OnBnClickedReadTaskFile()
{
	// TODO: Add your control notification handler code here
	if (m_bTaskRead)
	{
		MessageBox(_T("Task already read."));
		return;
	}
	CString strListSub;
	strListSub = _T("..\\debug\\00listsub.txt");
	CString strListNoSub;
	strListNoSub = _T("..\\debug\\00listnosub.txt");

	CString strTmp;

	VocTaskInfo vocFileInfo;

	int n = 0;
	CFile file;
	if (file.Open(strListSub, CFile::shareDenyWrite))
	{
		while (ReadLineFromTxt(file, strTmp))
		{
			vocFileInfo.nFileSize = GetFileSize(strTmp);
			vocFileInfo.strName = GetFileNameByPathNoExt(strTmp);
			vocFileInfo.strExt = GetFileExt(strTmp);
			vocFileInfo.strDir = GetDirNameByPath(ParentDir(strTmp));
			m_dqVocTask.push_back(vocFileInfo);
		}
		file.Close();
	}

	CString strFilePathNoSub;
	strFilePathNoSub = _T("\\\\dellserver1\\e$\\特种产品部语音备份-57Test\\");
	CString strFullNameNoSub;
	if (file.Open(strListNoSub, CFile::shareDenyWrite))
	{
		while (ReadLineFromTxt(file, strTmp))
		{
			vocFileInfo.nFileSize = GetFileSize(strFilePathNoSub + strTmp);
			vocFileInfo.strName = GetFileNameNoExt(strTmp);
			vocFileInfo.strExt = GetFileExt(strTmp);
			vocFileInfo.strDir = _T("");
			m_dqVocTask.push_back(vocFileInfo);
		}
		file.Close();
	}
}

void COracleDlgDlg::OnBnClickedWriteDb()
{
	// TODO: Add your control notification handler code here
	if (!m_bConnected)
	{
		MessageBox(_T("not connected."));
		return;
	}
	BOOL bTrans = false;
	int nCount = 0;
	nCount = GetDlgItemInt(IDC_WRITE_COUNT, &bTrans, true);
	if (0 == nCount)
	{
		nCount = 100;
	}
	else if (nCount < 0)
	{
		nCount = m_dqVocTask.size() - m_nVocTaskIndex;
	}
	if (m_nVocTaskIndex >= m_dqVocTask.size())
	{
		MessageBox(_T("no task left."));
		return;
	}
	if (m_nVocTaskIndex + nCount >= m_dqVocTask.size())
	{			
		SetDlgItemText(IDC_MSG_INFO, _T("no enough tasks."));
	}
	CString strInsert;
	int ii = 0;
	BSTR bs;
	for (ii = m_nVocTaskIndex; ii < m_nVocTaskIndex + nCount; ii++)
	{
		if (ii >= m_dqVocTask.size())
		{
			SetDlgItemText(IDC_MSG_INFO, _T("no task left."));
			return;
		}
		VocTaskInfo task = m_dqVocTask[ii];
		if (task.strDir.IsEmpty())
		{
			strInsert.Format(_T("INSERT INTO TBLVOICETEST (SN, FILESIZE, FILEEXT) values ('%s', %d, '%s')"),
				task.strName, task.nFileSize, task.strExt);
		}
		else
		{
			strInsert.Format(_T("INSERT INTO TBLVOICETEST (SN, FILESIZE, FILEPATH, FILEEXT) values ('%s', %d, '%s', '%s')"),
				task.strName, task.nFileSize, task.strDir, task.strExt);
		}

		try
		{
			bs = strInsert.AllocSysString();
			if (bs)
			{
				m_pCmd->CommandText = bs;
				m_pCmd->Execute(NULL, NULL, adCmdText);
				SysFreeString(bs);
				bs = NULL;
			}
		}
		catch (CMemoryException* e)
		{
			wchar_t errmsg[1000];
			wsprintf(errmsg, _T("%d memexcp: %s"), ii, strInsert);
			SetDlgItemText(IDC_MSG, errmsg);

		}
		catch (CFileException* e)
		{
			wchar_t errmsg[1000];
			wsprintf(errmsg, _T("%d fileexcp: %s"), ii, strInsert);
			SetDlgItemText(IDC_MSG, errmsg);
		}
		catch (CException* e)
		{
			wchar_t errmsg[1000];
			wsprintf(errmsg, _T("%d genexcp: %s"), ii, strInsert);
			SetDlgItemText(IDC_MSG, errmsg);
		}
		catch (_com_error& e)
		{
			e.ErrorMessage();
			wchar_t errmsg[1000];
			wsprintf(errmsg, _T("INDEX: %d ERROR: %s SQL: %s"), ii, e.ErrorMessage(), strInsert);
			SetDlgItemText(IDC_MSG, errmsg);
		}
	}
	m_nVocTaskIndex = ii;
	MessageBox(_T("Write Finished."));
}

void COracleDlgDlg::OnBnClickedSkipTask()
{
	// TODO: Add your control notification handler code here
	BOOL bTrans = false;
	int nCount = 0;
	nCount = GetDlgItemInt(IDC_SKIP_COUNT, &bTrans, true);
	if (0 == nCount)
	{
		nCount = 100;
	}
	m_nVocTaskIndex += nCount;
	wchar_t errmsg[500];
	wsprintf(errmsg, _T("INDEX: %d SKIPED: %d"), m_nVocTaskIndex, nCount);
	SetDlgItemText(IDC_MSG_INFO, errmsg);
}
