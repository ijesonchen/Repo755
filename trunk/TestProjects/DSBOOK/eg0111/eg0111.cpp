/*
��������к�
*/

#include <stdio.h>

int MaxSubseqSumN3(int* pa, int n)
{
	int maxsum = 0;
	for (int i = 0; i < n; ++i)	// i ��������˵�
	{
		for (int j = i; j < n; ++j) // j �������Ҷ˵�
		{
			int sum = 0;
			for (int k = i; k < j; ++k)
			{
				sum += pa[k];
			}
			if (maxsum < sum)
			{
				maxsum = sum;
			}
		}
	}
	return maxsum;
}

/*
N3�Ż�k��ѭ���õ�
*/
int MaxSubseqSumN2(int* pa, int n)
{

	int maxsum = 0;
	for (int i = 0; i < n; ++i)
	{
		int sum = 0;
		for (int j = i; j < n; ++j)
		{
			sum += pa[j];
			if (maxsum < sum)
			{
				maxsum = sum;
			}
		}
	}
	return maxsum;
}

/************************************************************************/
/* 
���η���
��Ϊ����������

**********|**********
������ֵ	�Ҳ����ֵ
     ������ֵ
���У�˫�����ֵ���Ϸֽ⣬Ϊ1��ʱ������Ϊ���ֵ��
������ֵ�����м俪ʼ�����󲻶��ۼӣ���������ֵ��ͬ���õ���������ֵ��
			�Ӷ��ó�������ֵ�����󣬿��ң����ߺͣ�

*/
/************************************************************************/
int MaxSubseqSumLogn(int* pa, int n)
{
	if (n == 1)
	{
		return pa[0];
	}
	int nmax = 0;
	int half = n / 2;
	int nl = MaxSubseqSumLogn(pa, half);
	int nr = MaxSubseqSumLogn(pa + half, n - half);
	nmax = nl > nr ? nl : nr;

	int* p = pa + half - 1;
	int nl2 = *p;
	int t = *p;
	--p;
	while (p >= pa)
	{
		t += *p;
		if (t > nl2)
		{
			nl2 = t;
		}
		--p;
	}
	if (nl2 > nmax)
	{
		nmax = nl2;
	}

	p = pa + half;
	int nr2 = *p;
	t = *p;
	++p;
	while (p - pa < n)
	{
		t += *p;
		if (t > nr2)
		{
			nr2 = t;
		}
		++p;
	}
	if (nr2 > nmax)
	{
		nmax = nr2;
	}
	if (nl2 + nr2 > nmax)
	{
		nmax = nl2 + nr2;
	}
	return nmax;
}

/*
����������sum�Ϳ�ʹ���к�sub����
*/
int MaxSubseqSumN(int* pa, int n)
{
	int sum = 0;
	int sub = 0;
	for (int ii = 0; ii < n; ++ii)
	{
		sum += pa[ii];
		if (sum > sub)
		{
			sub = sum;
		}
		else if (sum < 0)
		{
			sum = 0;
		}
	}
	return sub;
}

int main(void)
{
	int pa[] = {-1, -2, 0, 1, 2, -5, 1, 2, 3, -5, 2}; // ==6
	int n = sizeof(pa) / sizeof(int);
	int subsum1 = 0;
	int subsum2 = 0;
	int subsum3 = 0;
	int subsum4 = 0;
	subsum1 = MaxSubseqSumN3(pa, n);
	subsum2 = MaxSubseqSumN2(pa, n);
	subsum3 = MaxSubseqSumLogn(pa, n);
	subsum4 = MaxSubseqSumN(pa, n);
	return 0;
}