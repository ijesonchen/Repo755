// WebServiceDlgDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WebServiceDlg.h"
#include "WebServiceDlgDlg.h"

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


// CWebServiceDlgDlg �Ի���




CWebServiceDlgDlg::CWebServiceDlgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWebServiceDlgDlg::IDD, pParent)
	, m_service(_T(""))
	, m_addr(_T(""))
	, m_recid(_T(""))
	, m_clueid(_T(""))
	, m_user(_T(""))
	, m_filepath(_T(""))
	, m_result(_T(""))
	, m_method(_T(""))
	, m_count(0)
	, m_isDynamic(FALSE)
	, m_isObj(false)
	, m_isString(false)
	, m_isDataSet(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWebServiceDlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SRVNAME, m_service);
	DDX_Text(pDX, IDC_SRVADDR, m_addr);
	DDX_Text(pDX, IDC_REGID, m_recid);
	DDX_Text(pDX, IDC_CLUEID, m_clueid);
	DDX_Text(pDX, IDC_USER, m_user);
	DDX_Text(pDX, IDC_FILEPATH, m_filepath);
	DDX_Text(pDX, IDC_RESULT, m_result);
	DDX_Text(pDX, IDC_METHOD, m_method);
	DDX_Text(pDX, IDC_COUNT, m_count);
	DDX_Check(pDX, IDC_CHECK1, m_isDynamic);
	DDX_Check(pDX, IDC_RADIO1, m_isObj);
	DDX_Check(pDX, IDC_RADIO2, m_isString);
	DDX_Check(pDX, IDC_RADIO3, m_isDataSet);
}

BEGIN_MESSAGE_MAP(CWebServiceDlgDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CWebServiceDlgDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CWebServiceDlgDlg ��Ϣ�������

BOOL CWebServiceDlgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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

void CWebServiceDlgDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CWebServiceDlgDlg::OnPaint()
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
HCURSOR CWebServiceDlgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
#using <System.dll>
#using <System.XML.dll>
#using <System.Web.Services.dll>
#using <System.Data.dll>
using namespace System;
using namespace System::IO;
using namespace System::Net;
using namespace System::Reflection;
using namespace System::CodeDom;
using namespace System::CodeDom::Compiler;
using namespace System::Web::Services;
using namespace System::Web::Services::Description;
using namespace System::Web::Services::Protocols;
using namespace System::Xml::Serialization;

// �ִ�ת��
using namespace System::Runtime::InteropServices;

// DataSet
using namespace System::Data;

void CWebServiceDlgDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	String^ paraUrl = gcnew String(m_addr.Trim().GetString());
	String^ paraService = gcnew String(m_service.Trim().GetString());
	String^ paraMethod = gcnew String(m_method.Trim().GetString());

	String^ recid = gcnew String(m_recid.Trim().GetString());
	String^ clueid = gcnew String(m_clueid.Trim().GetString());
	String^ user = gcnew String(m_user.Trim().GetString());
	String^ filepath = gcnew String(m_filepath.Trim().GetString());
	String^ ftppath = gcnew String("");
	String^ username = gcnew String("");
	String^ password = gcnew String("");
	String^ msg = gcnew String("");


	array<Object^>^ paraObj8 = gcnew array<Object^>(8);
	paraObj8[0] = recid;
	paraObj8[1] = clueid;
	paraObj8[2] = user;
	paraObj8[3] = filepath;
	paraObj8[4] = ftppath;
	paraObj8[5] = username;
	paraObj8[6] = password;
	paraObj8[7] = msg;

// 	String^ url = gcnew String("http://www.webxml.com.cn/WebServices/TrainTimeWebService.asmx");
// 	String^ servicename = gcnew String("TrainTimeWebService");
//	String^ paraMethod = gcnew String("add_material");
	int nParaCount = 8;
	if (m_isDynamic)
	{
		if (m_count <= 0 || m_count > 8)
		{
			MessageBox(L"������������ȷ");
			return;
		}
		nParaCount = m_count;
	}


	array<Object^>^ paraObjs = gcnew array<Object^>(nParaCount);

	for (int ii = 0; ii < nParaCount; ++ii)
	{
		paraObjs[ii] = paraObj8[ii];
	}

	Object^ objResult;
	// 1. ʹ�� WebClient ���� WSDL ��Ϣ��
	WebClient^ web = gcnew WebClient();
	Stream^ stream = web->OpenRead(paraUrl + "?WSDL");

	// 2. �����͸�ʽ�� WSDL �ĵ���
	ServiceDescription^ description = ServiceDescription::Read(stream);

	// 3. �����ͻ��˴�������ࡣ
	ServiceDescriptionImporter^ importer = gcnew ServiceDescriptionImporter();

	importer->ProtocolName = "Soap"; // ָ������Э�顣
	importer->Style = ServiceDescriptionImportStyle::Client; // ���ɿͻ��˴���
	importer->CodeGenerationOptions = 
		CodeGenerationOptions::GenerateProperties | CodeGenerationOptions::GenerateNewAsync;

	importer->AddServiceDescription(description, nullptr, nullptr); // ��� WSDL �ĵ���

	// 4. ʹ�� CodeDom ����ͻ��˴����ࡣ
	CodeNamespace^ nmspace = gcnew CodeNamespace(); // Ϊ��������������ռ䣬ȱʡΪȫ�ֿռ䡣
	CodeCompileUnit^ unit = gcnew CodeCompileUnit();
	unit->Namespaces->Add(nmspace);

	ServiceDescriptionImportWarnings^ warning = importer->Import(nmspace, unit);
	CodeDomProvider^ provider = CodeDomProvider::CreateProvider("CSharp");

	CompilerParameters^ parameter = gcnew CompilerParameters();
	parameter->GenerateExecutable = false;
	parameter->GenerateInMemory = true;
	parameter->ReferencedAssemblies->Add("System.dll");
	parameter->ReferencedAssemblies->Add("System.XML.dll");
	parameter->ReferencedAssemblies->Add("System.Web.Services.dll");
	parameter->ReferencedAssemblies->Add("System.Data.dll");

	CompilerResults^ result = provider->CompileAssemblyFromDom(parameter, unit);

	// 5. ʹ�� Reflection ���� WebService��
	if (!result->Errors->HasErrors)
	{
		Assembly^ asmb = result->CompiledAssembly;
		Type^ t = asmb->GetType(paraService); // �����ǰ��Ϊ����������������ռ䣬�˴���Ҫ�������ռ���ӵ�����ǰ�档

		Object^ o = Activator::CreateInstance(t);
		MethodInfo^ method = t->GetMethod(paraMethod);

		objResult = method->Invoke(o, paraObjs);
		String^ strResult;

		if (m_isDynamic)
		{
			if (m_isObj)
			{
				strResult = objResult->ToString();

			}
			if (m_isString)
			{
				strResult = (String^)objResult;
			}
			if (m_isDataSet)
			{
				DataSet^ ds = (DataSet^)objResult;
				strResult = ds->GetXml();
			}	
		}
		else
		{
			Boolean b = (Boolean)objResult;
			strResult = String::Format("return: {0} \r\nftppath: {1}\r\nuser: {2}\r\npw: {3}\r\nmsg: {4}", b, ftppath, username, password, msg);
		}

		const wchar_t* chars = (const wchar_t*)(Marshal::StringToHGlobalUni(strResult)).ToPointer();
		m_result = chars;
		Marshal::FreeHGlobal(IntPtr((void*)chars));
		UpdateData(FALSE);	
	}

}
