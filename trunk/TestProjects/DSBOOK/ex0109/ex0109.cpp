/*
x^N时间复杂度为o(lgN)的算法
思路：
将乘法转化成幂加和
利用前项平方，获得x的
1,2,4,8,...次方
然后将n分解为上述幂的和
即可以看做将n作为上述位的二进制表示
x^n则对应上述项值的乘积

令k为2^k >= n的最小值

依次求出x^i (i:1->k)
则可以得到 n = sigma(a*i)
其中a=0/1, i:1->k

powern2:
将N表示成二进制，N=sigma(2^i)，其中N的第i位为1
从低位到高位ai，表示x^（2^i）
相应位为1，则乘以ai，相应为x^(sigma(2^i)),其中N的第i位为1
即x^n

*/

#include <stdio.h>

//#define AUTO_TEST

unsigned long long  powern1( int n, int x ) 
{

	int k = 0;
	// 2^k <= N
	int m = n;
	while (m / 2)
	{
		m = m / 2;
		++k;
	}
	printf("k = %d\n", k);
	unsigned long long *pk = new unsigned long long[k+1];
	int *pn = new int[k + 1];

	// pk[i] is x^(i+1)
	pk[0] = x;
	pn[0] = 1;
	for (int i = 1; i <= k; ++i)
	{
		pk[i] = pk[i - 1] * pk[i - 1];
		pn[i] = pn[i - 1] * 2;
	}

	unsigned long long sum = 1;
	m = n;
	for (int i = k; i >= 0; --i)
	{
		if (m >= pn[i])
		{
			m -= pn[i];
			sum *= pk[i];
		}
	}
	return sum;
}


unsigned long long powern2( int n, int x )
{
	unsigned long long t = x;
	unsigned long long sum = 1;
	while (n)
	{
		if (n & 1)
		{
			sum *= t;
		}
		t *= t;
		n >>= 1;
	}
	return sum;
}

int main(void)
{

#ifdef AUTO_TEST
	int x = 2;
for (int ii = 1; ii <= 64; ++ii)
{
	int n = ii;
	printf("n: %d\t", n);
#else
	int x = 0;
	int n = 0;
	printf("input x n:\n");
	scanf("%d %d", &x, &n);
#endif // AUTO_TEST

	unsigned long long sum = powern2(n, x);

#ifdef AUTO_TEST
	printf("%llu\n", sum);
}
#else
	printf("x^n = %llu\n", sum);
#endif // AUTO_TEST

	return 0;
}