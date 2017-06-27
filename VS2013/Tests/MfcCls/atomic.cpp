#include "stdafx.h"
#include <iostream>
#include <atomic>
#include <thread>
#include <sstream>
#include <vector>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

atomic_ullong g_counter;
atomic_bool g_run;

void Thread(int idx, int nCount)
{
	while (!g_run)
	{
		continue;
	}
	for (int ii = 0; ii < nCount; ++ii)
	{
		g_counter++;
		stringstream ss;
		ss << idx << "  - " << g_counter << endl;
		cout << ss.str();
		Sleep(1);
	}

	stringstream ss;
	ss << idx << "  - exit..." << endl;
	cout << ss.str();
}

void AutomicTest(void)
{
	g_run = false;
	g_counter = 0;
	int nthread = 20;
	int nloop = 20;
	
	vector<thread> threads;
	for (int ii = 0; ii < nthread; ++ii)
	{
		thread th(Thread, ii, nloop);
		threads.push_back(std::move(th));
	}
	g_run = true;
	for (int ii = 0; ii < nthread; ++ii)
	{
		threads[ii].join();
	}

	cout << "AutomicTest finish..." << endl;

}