// ZipTest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "ZipTest.h"

#include "..\..\Public\DebugNew.h"

#include "ZipFunc.h"
using namespace ZipFunc;


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

	// all unicode filename stored in UTF-8 and will restore when uncompress.
	nRetCode = ZipTestMain(argc, argv, envp);

	printf("finished.\n");
	return nRetCode;
}
