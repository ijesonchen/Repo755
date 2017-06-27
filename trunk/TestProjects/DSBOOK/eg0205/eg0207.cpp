#include <iostream>
using namespace std;

// 基于排序求中位数

int Median(int* p, int n)
{
	int m = 0;

	// 由大到小排列 
	for (int ii = 0; ii < n; ++ii)
	{
		m = p[ii];
		int k = ii;
		for (int jj = ii + 1; jj < n; ++jj)
		{
			if (m < p[jj])
			{
				m = p[jj]; 
				k = jj;
			}
		}
		p[k] = p[ii];
		p[ii] = m;
	}

	return p[(n-1)/2]; // 0-based
}

void eg0207(void)
{
	int a[] = {7,5,3,9,8,4,2,6,1,0};
	int x = Median(a, sizeof(a) / sizeof(int));
}