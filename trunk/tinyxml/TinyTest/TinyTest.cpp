// TinyTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "TinyTest.h"

#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// 初始化 MFC 并在失败时显示错误
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: MFC 初始化失败\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: 在此处为应用程序的行为编写代码。
	}

	mainTinyXml();

	char** pparg = new char*[argc];
	CStringA* strArgs = new CStringA[argc];
	for (int ii = 0; ii < argc; ++ii)
	{
		CString wstrArg = argv[ii];
		strArgs[ii] = wstrArg;
		pparg[ii] = (char*)strArgs[ii].GetString();
	}

	mainTinyXml2org(argc, (const char**)pparg);

	delete[] pparg;
	delete[] strArgs;

	return nRetCode;
}
