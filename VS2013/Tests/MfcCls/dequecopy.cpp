#include "stdafx.h"
#include <iostream>
#include <deque>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CTest
{
public:
	CTest()
	{
		cout << "default ctor " << endl;
	};


	CTest(const CTest& rhs)
	{
		cout << "copy ctor " << endl;
		*this = rhs;
	}

	~CTest()
	{
		cout << "destructor " << endl;
		delete p;
	};

	CTest& operator=(const CTest& rhs)
	{
		cout << "assign" << endl;
		if (this == &rhs)
		{
			return *this;
		}
		Clear();
		if (rhs.p && rhs.n)
		{
			p = new char[rhs.n];
			n = rhs.n;
			memcpy(p, rhs.p, n);
		}
		idx = rhs.idx;
		return *this;
	};

	void Clear(void)
	{
		delete p;
		p = nullptr;
		n = 0;
		idx = -1;
	}

	char* p = nullptr;
	int n = 0;
	int idx = -1;
};

void TestDequeCopy(void)
{
	const int dqCnt = 1;
	const int plen = 100;
	deque<CTest> dq1;
	for (int ii = 0; ii < dqCnt; ++ii)
	{
		CTest ctest;
		ctest.idx = ii;
		ctest.n = plen;
		ctest.p = new char[ctest.n];
		for (int jj = 0; jj < plen; ++jj)
		{
			ctest.p[jj] = (char)jj;
		}
		dq1.push_back(ctest);
	}

	deque<CTest> dq2;
	dq2 = dq1;

	return;
}