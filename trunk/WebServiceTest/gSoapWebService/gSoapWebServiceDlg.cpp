
// gSoapWebServiceDlg.cpp : ʵ���ļ�
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


// CgSoapWebServiceDlg �Ի���




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


// CgSoapWebServiceDlg ��Ϣ�������

BOOL CgSoapWebServiceDlg::OnInitDialog()
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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CgSoapWebServiceDlg::OnPaint()
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
HCURSOR CgSoapWebServiceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// �� �ַ�תUTF8 
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

// UTF8 ת���ַ� 
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
// 	// \\192.168.1.10\c$\����Ŀ¼\123\123
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
			MessageBox(L"����·����\\\\��ͷ����������");
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
					strtemp.Format(_T("��������ʧ��: %s ��ȡ��ϸ������Ϣʧ��"), strDstPath.GetString());
					MessageBox(strtemp);
				}
				else
				{
					CString strtemp;
					strtemp.Format(_T("��������ʧ��: %s. ����%u %s - %s"), strDstPath.GetString(), dwRet2, szName, szMsg);
					MessageBox(strtemp);
				}
			}
			break;

		default:
			{
				CString strtemp;
				strtemp.Format(_T("��������ʧ��: %s %u"), strDstPath.GetString(), dwRet);
				MessageBox(strtemp);
			}
			break;
		}
	}
}

void CgSoapWebServiceDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	// prepare file

	CString strFileName = w_strFileName;

	CFile file;
	if (!file.Open(strFileName, CFile::modeCreate | CFile::modeWrite))
	{
		MessageBox(L"������ʱ�ļ�ʧ�ܣ��޷��ϴ�");
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
		strErr.Format(_T("WebService����ʧ�ܣ�����: %d"), nRet);
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
		msg.Format(_T("WebService����ʧ��: %s"), wstr.c_str());
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
		MessageBox(L"�ϴ��ļ�����");
	}
}
