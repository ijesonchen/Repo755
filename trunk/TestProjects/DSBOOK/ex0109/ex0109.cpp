/*
x^Nʱ�临�Ӷ�Ϊo(lgN)���㷨
˼·��
���˷�ת�����ݼӺ�
����ǰ��ƽ�������x��
1,2,4,8,...�η�
Ȼ��n�ֽ�Ϊ�����ݵĺ�
�����Կ�����n��Ϊ����λ�Ķ����Ʊ�ʾ
x^n���Ӧ������ֵ�ĳ˻�

��kΪ2^k >= n����Сֵ

�������x^i (i:1->k)
����Եõ� n = sigma(a*i)
����a=0/1, i:1->k

powern2:
��N��ʾ�ɶ����ƣ�N=sigma(2^i)������N�ĵ�iλΪ1
�ӵ�λ����λai����ʾx^��2^i��
��ӦλΪ1�������ai����ӦΪx^(sigma(2^i)),����N�ĵ�iλΪ1
��x^n

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