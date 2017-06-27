// WebServiceDlgDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WebServiceDlg.h"
#include "WebServiceDlgDlg.h"

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


// CWebServiceDlgDlg 对话框




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


// CWebServiceDlgDlg 消息处理程序

BOOL CWebServiceDlgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CWebServiceDlgDlg::OnPaint()
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

// 字串转换
using namespace System::Runtime::InteropServices;

// DataSet
using namespace System::Data;

void CWebServiceDlgDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
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
			MessageBox(L"参数个数不正确");
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
	// 1. 使用 WebClient 下载 WSDL 信息。
	WebClient^ web = gcnew WebClient();
	Stream^ stream = web->OpenRead(paraUrl + "?WSDL");

	// 2. 创建和格式化 WSDL 文档。
	ServiceDescription^ description = ServiceDescription::Read(stream);

	// 3. 创建客户端代理代理类。
	ServiceDescriptionImporter^ importer = gcnew ServiceDescriptionImporter();

	importer->ProtocolName = "Soap"; // 指定访问协议。
	importer->Style = ServiceDescriptionImportStyle::Client; // 生成客户端代理。
	importer->CodeGenerationOptions = 
		CodeGenerationOptions::GenerateProperties | CodeGenerationOptions::GenerateNewAsync;

	importer->AddServiceDescription(description, nullptr, nullptr); // 添加 WSDL 文档。

	// 4. 使用 CodeDom 编译客户端代理类。
	CodeNamespace^ nmspace = gcnew CodeNamespace(); // 为代理类添加命名空间，缺省为全局空间。
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

	// 5. 使用 Reflection 调用 WebService。
	if (!result->Errors->HasErrors)
	{
		Assembly^ asmb = result->CompiledAssembly;
		Type^ t = asmb->GetType(paraService); // 如果在前面为代理类添加了命名空间，此处需要将命名空间添加到类型前面。

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
