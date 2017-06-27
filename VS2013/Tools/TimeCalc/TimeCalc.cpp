// TimeCalc.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "TimeCalc.h"
#include <iostream>
#include <limits>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

#undef max

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

	const time_t tmsecpermin = 60;
	const time_t tmminperhour = 60;
	const time_t tmhourperday = 24;

	const time_t tmmin = 60;
	const time_t tmsecperhour = 3600;
	const time_t tmsecperday = tmsecperhour * 24;
	const time_t tmsecperwk = tmsecperday * 7;
	const time_t tmsecpermon = tmsecperday * 30;
	const time_t tmsecperyear = tmsecperday * 365;

	while (true)
	{
		cout << "please input time_t: " << endl;
		time_t tInput = 0;
		cin >> tInput;
		if (cin.eof())
		{
			break;
		}

		if (cin.fail())
		{
			cout << "bad input" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}
		
		time_t tNow = time(nullptr);
		time_t tspan = tNow - tInput;
		cout << "input   : " << ctime(&tInput) ;
		cout << "current : " << ctime(&tNow) ;

		int nsec = tspan % tmsecpermin;
		int nmin = tspan / tmsecpermin % tmminperhour;
		int nhour = tspan / tmsecperhour % tmhourperday;
		int ntotalday = (int)tspan / tmsecperday;
		int nwk = 0;
		int nmon = 0;
		int nyear = 0;

		cout << "span: ";
		if (ntotalday) cout << ntotalday << " days ";
		cout << nhour << ":" << nmin << ":" << nsec << endl;		
	}

	cout << "exit." << endl;

	return nRetCode;
}
