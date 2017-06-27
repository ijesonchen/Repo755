#include "stdafx.h"
#include <iostream>
#include <deque>
#include <thread>
#include <list>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int nSleep = 1000;
int maxSec = 5;

class A
{
public:
	A()
	{
		cout << "ctor a" << endl;
	};

	A(A&)
	{
		cout << "copy a" << endl;
	}
	A(A&&)
	{
		cout << "move a" << endl;
	}
	A& operator=(A&&)
	{
		cout << "move assign a" << endl;
	}
	A& operator=(A&)
	{
		cout << "copy assign a" << endl;
	}

	~A()
	{
		cout << "dtor a" << endl;
	}
};

void ThreadTest(int idx, int r, A& a)
{
	auto nSec = r % 5 + 1;
//	int nSec = 5;
	cout << "enter thread " << idx << " " << std::this_thread::get_id();
	cout << " sleep " << nSec << " seconds " << endl;
//	Sleep(nSleep * nSec);
	cout << "leave thread " << idx << endl;
}

void TestThreadCntr(void)
{
	list<thread> ths;

	auto pred = [](thread& th){return !th.joinable(); };
	int nLoop = 10;
	int idx = 0;
	while (nLoop--)
	{
		ths.remove_if(pred);
		A a;
		thread th(ThreadTest, ++idx, rand(), std::move(a));
		auto id = th.get_id();
		cout << "id " << id << endl;
		cout << "th " << th.native_handle() << endl;
		ths.push_back(std::move(th));
		Sleep(nSleep);
	}

	for (auto& th : ths)
	{
		th.join();
	}
} 