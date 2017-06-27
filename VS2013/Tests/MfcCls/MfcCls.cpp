// MfcCls.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "MfcCls.h"
#include "test.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

//////////////////////////////////////////////////////////////////////////
//
const CString karrChnLangNames[]
{
	_T("汉语"),
		_T("英语"),
		_T("日语"),
		_T("俄语"),
		_T("哈萨克"),
		_T("德语"),
		_T("沪语"),
		_T("法语"),
		_T("粤语"),
		_T("维语"),
		_T("藏语"),
		_T("闽南"),
		_T("韩语"),
		_T("柯尔兹"),
};

const int knMaxChnLangIdx = sizeof(karrChnLangNames) / sizeof(CString);

struct SS
{
	enum SymbolType
	{
		langNone = 0x00000000,
		//适用语种
		langMand = 0x00000001,
		langEngl = 0x00000002,
		langJapa = 0x00000004,
		langRuss = 0x00000008,
		langHasc = 0x00000010,
		langGerm = 0x00000020,
		langHuyu = 0x00000040,
		langFren = 0x00000080,
		langYueh = 0x00000100,
		langWewr = 0x00000200,
		langZang = 0x00000400,
		langMinn = 0x00000800,
		langKore = 0x00001000,
		langKerz = 0x00002000,

		//符号类型，音节，音素，带声调音节，词语
		typePhoneme = 0x01000000,//00000001...
		typeSyllable = 0x02000000,//00000010...
		typeTuneSyl = 0x82000000,//10000010...第一个bit表示是否带声调
		typeWord = 0x04000000,//00000100...
		typeNoise = 0x08000000,//00001000...
		//音素类型，辅音，元音，鼻音，静音
		typeConsonant = 0x00010000,
		typeVowel = 0x00020000,
		typeRhonchus = 0x00040000,
		typeSilence = 0x00080000,
		typeSukun = 0x00100000,
	};
};

CString GetLangChnName(int nLangType)
{
	switch (nLangType)
	{
	case SS::langMand:
		return _T("汉语");
	case SS::langEngl:
		return _T("英语");
	case SS::langJapa:
		return _T("日语");
	case SS::langRuss:
		return _T("俄语");
	case SS::langHasc:
		return _T("哈萨克");
	case SS::langGerm:
		return _T("德语");
	case SS::langHuyu:
		return _T("沪语");
	case SS::langFren:
		return _T("法语");
	case SS::langYueh:
		return _T("粤语");
	case SS::langWewr:
		return _T("维语");
	case SS::langZang:
		return _T("藏语");
	case SS::langMinn:
		return _T("闽南");
	case SS::langKore:
		return _T("韩语");
	case SS::langKerz:
		return _T("柯尔兹");
	default:
		return _T("未知");
	}
}


CString GetLangChnName2(int nLangType)
{
	int idx = -1;
	while (nLangType)
	{
		nLangType >>= 1;
		idx++;
	}
	if (idx >= 0 && idx < knMaxChnLangIdx)
	{
		return karrChnLangNames[idx];
	}
	return _T("未知");
}

void p(int nLangType)
{
	static int idx = -1;
	++idx;
	wcout << idx << L"-" << nLangType << L":" 
		<< GetLangChnName(nLangType).GetString() << L" - " 
		<< GetLangChnName2(nLangType).GetString() << endl;
}

void TestLang(void)
{
// 	p(SS::langMand);
// 	p(SS::langEngl);
// 	p(SS::langJapa);
// 	p(SS::langRuss);
// 	p(SS::langHasc);
// 	p(SS::langGerm);
// 	p(SS::langHuyu);
// 	p(SS::langFren);
// 	p(SS::langYueh);
// 	p(SS::langWewr);
// 	p(SS::langZang);
// 	p(SS::langMinn);
// 	p(SS::langKore);
// 	p(SS::langKerz);
	p(5);
}



int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// 初始化 MFC 并在失败时显示错误
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO:  更改错误代码以符合您的需要
			_tprintf(_T("错误:  MFC 初始化失败\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO:  在此处为应用程序的行为编写代码。
		}
	}
	else
	{
		// TODO:  更改错误代码以符合您的需要
		_tprintf(_T("错误:  GetModuleHandle 失败\n"));
		nRetCode = 1;
	}
// 	setlocale(LC_ALL, "chs");
// 	TestLang();
	
	TestAll();

	cout << endl << "enter to exit..." << endl;
	getchar();
	cout << endl << "exited..." << endl;
	return nRetCode;
}
