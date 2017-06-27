
// gSoapWebServiceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "gSoapWebService.h"
#include "gSoapWebServiceDlg.h"

#include <Winnetwk.h>
#pragma comment(lib, "Mpr.lib")

#include "FTP/FtpClient.h"
#include "gSOAP/soapH.h"
#include "gSOAP/ws_USCOREmaterialSoap.nsmap"

using namespace std;

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


// CgSoapWebServiceDlg 对话框




CgSoapWebServiceDlg::CgSoapWebServiceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CgSoapWebServiceDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CgSoapWebServiceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_REGID, w_strRecid);
	DDX_Text(pDX, IDC_CLUEID, w_strClueid);
	DDX_Text(pDX, IDC_USER, w_strUser);
	DDX_Text(pDX, IDC_FILEPATH, w_strFileName);
	DDX_Text(pDX, IDC_RESULT, w_strRemark);
}

BEGIN_MESSAGE_MAP(CgSoapWebServiceDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CgSoapWebServiceDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CgSoapWebServiceDlg 消息处理程序

BOOL CgSoapWebServiceDlg::OnInitDialog()
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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CgSoapWebServiceDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CgSoapWebServiceDlg::OnPaint()
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
HCURSOR CgSoapWebServiceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// 宽 字符转UTF8 
string EncodeUtf8(const wchar_t* p) 
{ 
	wstring in(p);
	string s(in.length()*3+1,' '); 
	size_t len = ::WideCharToMultiByte(CP_UTF8, 0, 
		in.c_str(), in.length(), 
		&s[0], s.length(), 
		NULL, NULL); 
	s.resize(len); 
	return s; 
}

// UTF8 转宽字符 
wstring DecodeUtf8(const char* p) 
{ 
	string in(p);
	wstring s(in.length(), L' '); 
	size_t len = ::MultiByteToWideChar(CP_UTF8, 0, 
		in.c_str(), in.length(), 
		&s[0], s.length()); 
	s.resize(len); 
	return s; 
}

// bool ParseFtpPath(const CString& strFullPath, CString& strServer, CString& strRemotPath)
// {
// 	// \\192.168.1.10\c$\测试目录\123\123
// 	// check
// 	if (0 != strFullPath)
// 	{
// 	}
// }

void CgSoapWebServiceDlg::ConnectShare(const CString& strUserName, 
				  const CString& strPassword,
				  const CString& strDstPath)
{
	if (!strUserName.IsEmpty() && !strDstPath.IsEmpty() && !strPassword.IsEmpty())
	{
		if (strDstPath.Find(_T("\\\\")) != 0)
		{
			MessageBox(L"本地路径非\\\\开头，无需连接");
			return;
		}

		TCHAR tszNetPath[MAX_PATH + 1] = {0};
		int nPathLen = strDstPath.GetLength();
		if (nPathLen > MAX_PATH)
		{
			nPathLen = MAX_PATH;
		}
		memcpy(tszNetPath, strDstPath.GetString(), nPathLen * sizeof(TCHAR));

		NETRESOURCE netRes;
		netRes.dwType = RESOURCETYPE_DISK;
		netRes.lpLocalName = NULL;
		netRes.lpRemoteName = tszNetPath;
		netRes.lpProvider = NULL;

		DWORD dwRet = WNetAddConnection2(&netRes, strPassword.GetString(), strUserName.GetString(), CONNECT_UPDATE_PROFILE);

		switch (dwRet)
		{
		case NO_ERROR:
			break;

		case ERROR_SESSION_CREDENTIAL_CONFLICT:
			break;

		case ERROR_EXTENDED_ERROR:
			{
				DWORD dwRet2 = 0;
				const DWORD nBufLen = 1024;
				TCHAR szMsg[nBufLen] = {0};
				TCHAR szName[nBufLen] = {0};
				dwRet = WNetGetLastError(&dwRet2, szMsg, nBufLen, szName, nBufLen);
				if (NO_ERROR != dwRet)
				{
					CString strtemp;
					strtemp.Format(_T("建立链接失败: %s 获取详细错误信息失败"), strDstPath.GetString());
					MessageBox(strtemp);
				}
				else
				{
					CString strtemp;
					strtemp.Format(_T("建立链接失败: %s. 代码%u %s - %s"), strDstPath.GetString(), dwRet2, szName, szMsg);
					MessageBox(strtemp);
				}
			}
			break;

		default:
			{
				CString strtemp;
				strtemp.Format(_T("建立链接失败: %s %u"), strDstPath.GetString(), dwRet);
				MessageBox(strtemp);
			}
			break;
		}
	}
}

void CgSoapWebServiceDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	// prepare file

	CString strFileName = w_strFileName;

	CFile file;
	if (!file.Open(strFileName, CFile::modeCreate | CFile::modeWrite))
	{
		MessageBox(L"创建临时文件失败，无法上传");
		return;
	}

	CStringA strA;
	strA = w_strRemark;
	file.Write(strA.GetString(), strA.GetLength());

	file.Close();

	// web service call

	int nRet = 0;

	string strRecid = EncodeUtf8(w_strRecid.GetString());
	string strClueid = EncodeUtf8(w_strClueid.GetString());
	string strUser = EncodeUtf8(w_strUser.GetString());
	string strLocalFile = EncodeUtf8(w_strFileName.GetString());

	struct soap *soap = soap_new1(SOAP_C_UTFSTRING); // alloc 'soap' engine context
	_ns1__add_USCOREmaterial* pmaterial = soap_new_set__ns1__add_USCOREmaterial(soap, 
		(char*)strRecid.c_str(), (char*)strClueid.c_str(), (char*)strUser.c_str(), (char*)strLocalFile.c_str());
	_ns1__add_USCOREmaterialResponse resp;
	nRet = soap_call___ns1__add_USCOREmaterial(soap, 0, 0, pmaterial, resp);

	if (nRet != SOAP_OK)
	{
		CString strErr;
		strErr.Format(_T("WebService调用失败，代码: %d"), nRet);
		MessageBox(strErr);
		soap_destroy(soap); // dealloc serialization data
		soap_end(soap);     // dealloc temp data
		soap_free(soap);    // dealloc 'soap' engine context
		return;
	}

	wstring wstr;
	if (!resp.add_USCOREmaterialResult)
	{
		wstr = DecodeUtf8(resp.strMsg);
		CString msg;
		msg.Format(_T("WebService请求失败: %s"), wstr.c_str());
		MessageBox(msg);

		soap_destroy(soap); // dealloc serialization data
		soap_end(soap);     // dealloc temp data
		soap_free(soap);    // dealloc 'soap' engine context
		return;
	}
	CString strFtpPath;
	CString strFtpUser;
	CString strFtpPwd;
	CString strMsg;

	wstr = DecodeUtf8(resp.strFtpPath ? resp.strFtpPath : "");
	strFtpPath = wstr.c_str();
	wstr = DecodeUtf8(resp.strUserName ? resp.strUserName : "");
	strFtpUser = wstr.c_str();
	wstr = DecodeUtf8(resp.strPassword ? resp.strPassword : "");
	strFtpPwd  = wstr.c_str();
	wstr = DecodeUtf8(resp.strMsg ? resp.strMsg : "");
	strMsg  = wstr.c_str();

	// test
	if (false)
	{
		strFtpPath = L"\\\\dellserver\\d$\\ftproot\\a";
		strFtpUser = L"administrator";
		strFtpPwd  = L"111111";
	}

	w_strRemark.Format(_T(" strFtpPath %s \r\n strFtpUser %s \r\n strFtpPwd %s \r\n strMsg %s"),
		strFtpPath, strFtpUser, strFtpPwd, strMsg);
	UpdateData(FALSE);

	soap_destroy(soap); // dealloc serialization data
	soap_end(soap);     // dealloc temp data
	soap_free(soap);    // dealloc 'soap' engine context

	// ftp
	CString strRemoteFile;
	strRemoteFile.Format(_T("%s\\%s"), strFtpPath, w_strFileName);

	// connect
	ConnectShare(strFtpUser, strFtpPwd, strFtpPath);

	if (!CopyFile(strFileName, strRemoteFile, TRUE))
	{
		MessageBox(L"上传文件出错");
	}
}
