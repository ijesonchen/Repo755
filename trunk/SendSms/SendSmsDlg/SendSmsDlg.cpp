
// SendSmsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SendSms.h"
#include "SendSmsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CSendSmsDlg �Ի���




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


// CSendSmsDlg ��Ϣ�������

BOOL CSendSmsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	SetDlgItemTextA(m_hWnd, IDC_SMSDll, "sms.dll");
	m_hSmsDll = NULL;
	Sms_Connect = NULL;
	Sms_Disconn = NULL;
	Sms_Send = NULL;
	Sms_Receive = NULL;
	Sms_Delete = NULL;

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSendSmsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
// 	strtmp="//�Ϻ�Ѹ����Ϣ�������޹�˾,��ַwww.xunsai.com//";
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
		ShowMsg("δѡ�ж˿�");
		return;
	}
	m_cCommCombo.GetLBText(nIndex, strCom);
	if (strCom.IsEmpty())
	{
		ShowMsg("δѡ�ж˿�");
		return;
	}
	strCom.Delete(0, 3);

	ShowMsg("��������...");
	CStringA szUEType[255];
	int nPort;
	nPort = _wtoi(strCom);
	// ��һ������������ "//�Ϻ�Ѹ����Ϣ�������޹�˾,��ַwww.xunsai.com//" 
	if (1 == Sms_Connect("//�Ϻ�Ѹ����Ϣ�������޹�˾,��ַwww.xunsai.com//", nPort, 19200, szUEType))
	{
		strMsg.Format("���ӳɹ� �˿�:%d �豸����:%s", nPort, *szUEType);
		ShowMsg(strMsg);
	}
	else
	{
		strMsg.Format("����ʧ�� �˿�:%d", nPort);
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
	strMsg.Format("���� %s: %s", strMobileNumber, strSmsContent);
	ShowMsg(strMsg);

	if (1 == Sms_Send(strMobileNumber.GetBuffer(), strSmsContent.GetBuffer()))
	{
		ShowMsg("���ͳɹ�");
	}
	else
	{
		ShowMsg("����ʧ��");
	}
}

void CSendSmsDlg::OnBnClickedReceiveButton()
{
	// TODO: Add your control notification handler code here
	ShowMsg("���ڶ�ȡ");
	CStringA strRecv;
	CStringA strBuffer[1024];
	
	// 0��δ�����ţ�1���Ѷ����ţ�2���������ţ�3���ѷ����ţ�4��ȫ������
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
	ShowMsg("����ɾ��");
	Sms_Delete(strIndexA.GetBuffer());
	ShowMsg("ɾ�����");
}

void CSendSmsDlg::OnBnClickedDisconnectionButton()
{
	// TODO: Add your control notification handler code here
	ShowMsg("���ڶϿ�����");
	Sms_Disconn();
	ShowMsg("�ѶϿ�");
}

void CSendSmsDlg::OnBnClickedInitButton()
{
	// TODO: Add your control notification handler code here

	// �������ô��ں�
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
			ShowMsg("������DLL�ļ���");
			throw -1;
		}
		m_hSmsDll = LoadLibrary(strDll);
		if (!m_hSmsDll)
		{
			ShowMsg("����DLLʧ��");
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
		m_cMsgList.InsertString(0, _T("��ʼ�����"));
	}
	catch (int x)
	{
		CString strError;
		strError.Format(_T("��ʼ��ʧ�ܡ��������:%d"), x);
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

// ��List�ؼ���һ�в�����Ϣ
int CSendSmsDlg::ShowMsg( CStringA szMsgA )
{
	CString str;
	str = szMsgA;
	m_cMsgList.InsertString(0, str);
	return 0;
}
