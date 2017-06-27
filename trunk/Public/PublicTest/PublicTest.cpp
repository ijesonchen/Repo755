// PublicTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "PublicTest.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "..\Public\PublicFunc.h"
using namespace PublicFunc;

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

	CString str(_T("asdf你好sfi测试"));
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
