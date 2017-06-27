// MfcCls.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "MfcCls.h"
#include "test.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ψһ��Ӧ�ó������

CWinApp theApp;

using namespace std;

//////////////////////////////////////////////////////////////////////////
//
const CString karrChnLangNames[]
{
	_T("����"),
		_T("Ӣ��"),
		_T("����"),
		_T("����"),
		_T("������"),
		_T("����"),
		_T("����"),
		_T("����"),
		_T("����"),
		_T("ά��"),
		_T("����"),
		_T("����"),
		_T("����"),
		_T("�¶���"),
};

const int knMaxChnLangIdx = sizeof(karrChnLangNames) / sizeof(CString);

struct SS
{
	enum SymbolType
	{
		langNone = 0x00000000,
		//��������
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

		//�������ͣ����ڣ����أ����������ڣ�����
		typePhoneme = 0x01000000,//00000001...
		typeSyllable = 0x02000000,//00000010...
		typeTuneSyl = 0x82000000,//10000010...��һ��bit��ʾ�Ƿ������
		typeWord = 0x04000000,//00000100...
		typeNoise = 0x08000000,//00001000...
		//�������ͣ�������Ԫ��������������
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
		return _T("����");
	case SS::langEngl:
		return _T("Ӣ��");
	case SS::langJapa:
		return _T("����");
	case SS::langRuss:
		return _T("����");
	case SS::langHasc:
		return _T("������");
	case SS::langGerm:
		return _T("����");
	case SS::langHuyu:
		return _T("����");
	case SS::langFren:
		return _T("����");
	case SS::langYueh:
		return _T("����");
	case SS::langWewr:
		return _T("ά��");
	case SS::langZang:
		return _T("����");
	case SS::langMinn:
		return _T("����");
	case SS::langKore:
		return _T("����");
	case SS::langKerz:
		return _T("�¶���");
	default:
		return _T("δ֪");
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
	return _T("δ֪");
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
		// ��ʼ�� MFC ����ʧ��ʱ��ʾ����
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO:  ���Ĵ�������Է���������Ҫ
			_tprintf(_T("����:  MFC ��ʼ��ʧ��\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO:  �ڴ˴�ΪӦ�ó������Ϊ��д���롣
		}
	}
	else
	{
		// TODO:  ���Ĵ�������Է���������Ҫ
		_tprintf(_T("����:  GetModuleHandle ʧ��\n"));
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
