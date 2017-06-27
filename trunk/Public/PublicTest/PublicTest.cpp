// PublicTest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "PublicTest.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "..\Public\PublicFunc.h"
using namespace PublicFunc;

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

	CString str(_T("asdf���sfi����"));
	int nRet = 0;
	nRet = StringLength1G(str.GetString());

	int nUtf8Len = 1000;
	char* pUtf8 = new char[nUtf8Len];
	nRet = UnicodeToUtf8(str.GetString(), pUtf8, nUtf8Len);

	int nUniLen = 1000;
	wchar_t* pUni = new wchar_t[nUniLen];
	nRet = Utf8ToUnicode(pUtf8, pUni, nUniLen);

	nRet = str.Compare(pUni);

	wstring wstr = str.GetString();
	string u8 = EncodeUtf8(wstr);
	wstring wstr2 = DecodeUtf8(u8);

	if (wstr != wstr)
	{
		return -3;
	}

	return nRetCode;
}
