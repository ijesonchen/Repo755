#include "stdafx.h"
#include <vector>
#include <iostream>

/*
���ۣ�
emplace_back���ȡback()���ã��������δ����β����
   
*/

using namespace std;

void TestEmplace(void)
{
	vector<int> vt;
	for (int ii = 0; ii < 10; ++ii)
	{
		vt.emplace_back();
		vt.back() = ii;
	}

	vt.emplace_back();
	auto& v1 = vt.back();
	vt.emplace_back();
	auto& v2 = vt.back();

	v1 = 13;
	v2 = 14;

}