// TinyTest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "TinyTest.h"

#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Ψһ��Ӧ�ó������

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// ��ʼ�� MFC ����ʧ��ʱ��ʾ����
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: ���Ĵ�������Է���������Ҫ
		_tprintf(_T("����: MFC ��ʼ��ʧ��\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: �ڴ˴�ΪӦ�ó������Ϊ��д���롣
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
