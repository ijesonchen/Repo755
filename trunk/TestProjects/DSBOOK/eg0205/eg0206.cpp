#include <iostream>
using namespace std;

// 求 100 - 200 间的所有素数 

void prime1(void)
{
	for (int ii = 100; ii <= 200; ++ii)
	{
		bool bis = true;
		for (int jj = 2; jj < ii; ++jj)
		{
			if (ii % jj == 0)
			{
				bis = false;
				break;
			}
		}
		if (bis)
		{
			cout << ii << " ";
		}
	}
	cout << endl;
}

void prime2(void)
{
	for (int ii = 100; ii <= 200; ++ii)
	{
		int jj = 2;
		while (jj < ii && ii % jj)
		{
			++jj;
		}
		if (jj == ii)
		{
			cout << ii << " ";
		}
	}
	cout << endl;
}

void eg0206(void)
{
	prime1();
	prime2();
}