/*

秦九昭法求多项式和

f(x)=sigma(ai*x^i)

*/

#include <Windows.h>

// 直接求解多项式和
int PolySumDirect(int* pa, int x, int n)
{
	int sum = 0;
	for (int i = 0; i < n; ++i)
	{
		int xi = 1;
		for (int j = 0; j < i; ++j)
		{
			xi *= x;
		}
		sum += pa[i] * xi;
	}
	return sum;
}

// 秦九昭法求多项式和
int PolySumQjz(int* pa, int x, int n)
{
	int sum = 0;
	int xi = 1;
	for (int i = 0; i < n; ++i)
	{
		sum += pa[i] * xi;
		xi *= x;
	}
	return sum;
}

#include <stdio.h>

int main(void)
{
	const int n = 30;
	int pa[n];
	for (int i = 0; i < n; ++i)
	{
		pa[i] = i + 1;
	}
	int x = 3;
	int loop = 500000;
	int sum1 = 0;
	int sum2 = 0;
	DWORD dw1 = GetTickCount();
	for (int i = 0; i < loop; ++i)
	{
		sum1 = PolySumDirect(pa, x, sizeof(pa) / sizeof(int));
	}
	dw1 = GetTickCount() - dw1;
	DWORD dw2 = GetTickCount();
	for (int i = 0; i < loop; ++i)
	{
		sum2 = PolySumDirect(pa, x, sizeof(pa) / sizeof(int));
	}
	dw2 = GetTickCount() - dw2;
	bool bequal = (sum1 == sum2);
	if (bequal)
	{
		printf("right.\n");
	}
	else
	{
		printf("error.\n");
	}
	return 0;
}