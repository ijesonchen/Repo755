// DataAlg.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "DataAlg.h"

#include "..\..\Public\DebugNew.h"

#include "Functions.h"


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

	char szTest[] = "abcde";

	permulation(szTest, 0, strlen(szTest) - 1);

	return nRetCode;
}
