
// SendSmsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SendSms.h"
#include "SendSmsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CSendSmsDlg 对话框




CSendSmsDlg::CSendSmsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSendSmsDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSendSmsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSG_LIST, m_cMsgList);
	DDX_Control(pDX, IDC_COMM_COMBO, m_cCommCombo);
}

BEGIN_MESSAGE_MAP(CSendSmsDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(IDC_Connection_Button, &CSendSmsDlg::OnBnClickedConnectionButton)
	ON_BN_CLICKED(IDC_Send_Button, &CSendSmsDlg::OnBnClickedSendButton)
	ON_BN_CLICKED(IDC_Receive_Button, &CSendSmsDlg::OnBnClickedReceiveButton)
	ON_BN_CLICKED(IDC_Delete_Button, &CSendSmsDlg::OnBnClickedDeleteButton)
	ON_BN_CLICKED(IDC_Disconnection_Button, &CSendSmsDlg::OnBnClickedDisconnectionButton)
	ON_BN_CLICKED(IDC_INIT_BUTTON, &CSendSmsDlg::OnBnClickedInitButton)
END_MESSAGE_MAP()


// CSendSmsDlg 消息处理程序

BOOL CSendSmsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SetDlgItemTextA(m_hWnd, IDC_SMSDll, "sms.dll");
	m_hSmsDll = NULL;
	Sms_Connect = NULL;
	Sms_Disconn = NULL;
	Sms_Send = NULL;
	Sms_Receive = NULL;
	Sms_Delete = NULL;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSendSmsDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSendSmsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSendSmsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSendSmsDlg::OnBnClickedConnectionButton()
{

// 	HINSTANCE hinstDLL; 
// 	hinstDLL=LoadLibraryA("sms.dll");
// 	CStringA strText;
// 	CStringA strtmp;
// 	CStringA tmp[256];
// 	strtmp="//上海迅赛信息技术有限公司,网址www.xunsai.com//";
// 	typedef int(_stdcall *Sms_Connection)(CStringA CopyRight,int Com_Port,int Com_BaudRate,CStringA *Mobile_Type);
// 	Sms_Connection Proc;
// 	Proc = (Sms_Connection)GetProcAddress(hinstDLL,"Sms_Connection");
// 	int iValue = Proc(strtmp, 6,19200,tmp);
// 	if (iValue == 1)
// 	{
// 	} else if (iValue == 0) 
// 	{
// 	}
	// TODO: Add your control notification handler code here
	CStringA strMsg;
	// get com port
	CString strCom;
	int nIndex = m_cCommCombo.GetCurSel();
	if (CB_ERR  == nIndex)
	{
		ShowMsg("未选中端口");
		return;
	}
	m_cCommCombo.GetLBText(nIndex, strCom);
	if (strCom.IsEmpty())
	{
		ShowMsg("未选中端口");
		return;
	}
	strCom.Delete(0, 3);

	ShowMsg("正在连接...");
	CStringA szUEType[255];
	int nPort;
	nPort = _wtoi(strCom);
	// 第一个参数必须是 "//上海迅赛信息技术有限公司,网址www.xunsai.com//" 
	if (1 == Sms_Connect("//上海迅赛信息技术有限公司,网址www.xunsai.com//", nPort, 19200, szUEType))
	{
		strMsg.Format("连接成功 端口:%d 设备类型:%s", nPort, *szUEType);
		ShowMsg(strMsg);
	}
	else
	{
		strMsg.Format("连接失败 端口:%d", nPort);
		ShowMsg(strMsg);
	}
}

void CSendSmsDlg::OnBnClickedSendButton()
{
	// TODO: Add your control notification handler code here
	CStringA strMobileNumber;
	CStringA strSmsContent;
	CString strTmp;
	GetDlgItemText(IDC_SMSTel, strTmp);
	strMobileNumber = strTmp;
	GetDlgItemText(IDC_SMSCon, strTmp);
	strSmsContent = strTmp;

	CStringA strMsg;
	strMsg.Format("发送 %s: %s", strMobileNumber, strSmsContent);
	ShowMsg(strMsg);

	if (1 == Sms_Send(strMobileNumber.GetBuffer(), strSmsContent.GetBuffer()))
	{
		ShowMsg("发送成功");
	}
	else
	{
		ShowMsg("发送失败");
	}
}

void CSendSmsDlg::OnBnClickedReceiveButton()
{
	// TODO: Add your control notification handler code here
	ShowMsg("正在读取");
	CStringA strRecv;
	CStringA strBuffer[1024];
	
	// 0：未读短信；1：已读短信；2：待发短信；3：已发短信；4：全部短信
	char* smsType = "4";
	Sms_Receive(smsType, strBuffer);
	strRecv.Format("%s", *strBuffer);
	SetDlgItemTextA(m_hWnd, IDC_SMSReceived, strRecv);
	ShowMsg(strRecv);
}

void CSendSmsDlg::OnBnClickedDeleteButton()
{
	// TODO: Add your control notification handler code here
	CString strIndex;
	GetDlgItemText(IDC_Index, strIndex);
	CStringA strIndexA;
	strIndexA = strIndex;
	ShowMsg("正在删除");
	Sms_Delete(strIndexA.GetBuffer());
	ShowMsg("删除完成");
}

void CSendSmsDlg::OnBnClickedDisconnectionButton()
{
	// TODO: Add your control notification handler code here
	ShowMsg("正在断开连接");
	Sms_Disconn();
	ShowMsg("已断开");
}

void CSendSmsDlg::OnBnClickedInitButton()
{
	// TODO: Add your control notification handler code here

	// 遍历可用串口号
	m_cCommCombo.ResetContent();
	int nCommMax = 255;
	CString strCommPort;
	HANDLE hComm = NULL;
	for (int ii = 0; ii < nCommMax; ii++)
	{
		strCommPort.Format(_T("COM%d"), ii);
		hComm = CreateFile(strCommPort, 
							GENERIC_READ | GENERIC_WRITE, 
							0,
							NULL,
							OPEN_EXISTING,
							0,
							NULL);
		if (INVALID_HANDLE_VALUE != hComm)
		{
			// found comm
			m_cCommCombo.AddString(strCommPort);
			CloseHandle(hComm);
		}
	} // for
	m_cCommCombo.SetCurSel(0);


	// load DLL lib
	CString strDll;
	GetDlgItemText(IDC_SMSDll, strDll);
	try
	{
		if (strDll.IsEmpty())
		{
			ShowMsg("请输入DLL文件名");
			throw -1;
		}
		m_hSmsDll = LoadLibrary(strDll);
		if (!m_hSmsDll)
		{
			ShowMsg("载入DLL失败");
			throw -2;
		}

		Sms_Connect = (pFuncSms_Connection)GetProcAddress(m_hSmsDll, "Sms_Connection");
		Sms_Disconn = (pFuncSms_Disconnection)GetProcAddress(m_hSmsDll, "Sms_Disconnection");
		Sms_Send	= (pFuncSms_Send)GetProcAddress(m_hSmsDll, "Sms_Send");
		Sms_Receive = (pFuncSms_Receive)GetProcAddress(m_hSmsDll, "Sms_Receive");
		Sms_Delete	= (pFuncSms_Delete)GetProcAddress(m_hSmsDll, "Sms_Delete");

		if (!(Sms_Connect && Sms_Disconn && Sms_Send && Sms_Receive && Sms_Delete))
		{
			throw -3;
		}
		m_cMsgList.InsertString(0, _T("初始化完成"));
	}
	catch (int x)
	{
		CString strError;
		strError.Format(_T("初始化失败。错误代号:%d"), x);
		m_cMsgList.InsertString(0, strError);

		// reset 
		if (m_hSmsDll)
		{
			CloseHandle(m_hSmsDll);
			m_hSmsDll = NULL;
		}
		Sms_Connect = NULL;
		Sms_Disconn = NULL;
		Sms_Send = NULL;
		Sms_Receive = NULL;
		Sms_Delete = NULL;
	}
}

// 在List控件第一行插入消息
int CSendSmsDlg::ShowMsg( CStringA szMsgA )
{
	CString str;
	str = szMsgA;
	m_cMsgList.InsertString(0, str);
	return 0;
}
