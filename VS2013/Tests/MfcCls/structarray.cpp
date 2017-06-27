#include "stdafx.h"
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

enum Type
{
	Type0 = 0,
	TypeA,
	TypeB,
	TypeC,
	TypeD,
};

struct TestMap
{
	Type t;
	string a;
	string b;
};

bool operator==(const TestMap& m, const Type t1)
{
	if (m.t == t1)
	{
		return true;
	}
	return false;
}

const TestMap testMap[] =
{
	{ TypeA, "a", "1" },
	{ TypeB, "b", "2" },
	{ TypeC, "c", "3" },
	{ TypeD, "d", "4" },
};

TestMap FindT(int n)
{
	const Type t = (Type)n;

	TestMap m = { Type0, "δ֪", "unknown" };
	auto p = find(begin(testMap), end(testMap), t);
	if (p == end(testMap))
	{
		return m;
	}
	return *p;
}

void TestStructArray(void)
{
	for (int ii = 0; ii < 7; ++ii)
	{
		cout << FindT(ii).t << ", " << FindT(ii).a << ", " << FindT(ii).b << endl;
	}

	cout << "end";
}