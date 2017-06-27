#include "StdAfx.h"
#include "AlgCommon.h"

#include <iostream>
using namespace std;

namespace AlgCommon
{
	// 1-7 递归计算n!
	int Factorial(int n)
	{
		if (n<=1)
		{
			return 1;
		}
		else
		{
			return n * Factorial(n - 1);
		}
	}

	// 1-8 累加a[0: n-1]
	template<class T>
	T Sum(T a[], int n)
	{
		T tsum = 0;
		for (int ii = 0; ii < n; ++ii)
		{
			tsum += a[ii];
		}
		return tsum;
	}

	// 1-9 递归计算a[0: n-1]
	template<class T>
	T Rsum(T a[], int n)
	{
		if (n > 0)
		{
			return Rsum(a, n-1) + a[n-1];
		}
		else
		{
			return 0;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 函数模板的显式实例化
	template int Rsum<int>(int * const,int);

	// 1-10 递归排列字串，list[k:m]的排列
	template<class T>
	void Perm(T list[], int k, int m)
	{
		if (k == m)
		{
			// output
			for (int ii = 0; ii <= m; ++ii)
			{
				cout<<list[ii];
			}
			cout<<endl;
		}
		else
		{
			// x * Perm(k+1, n)
			for (int ii = k; ii <= m; ++ii)
			{
				Swap(list[k], list[ii]);
				Perm(list, k + 1, m);
				Swap(list[k], list[ii]);
			}			
		}
	}

	// 1-11 交换两个值
	template<class T>
	inline void Swap(T& a, T& b)
	{
		T c = a;
		a = b;
		b = c;
	}


	// ex. 1.2 4)1 Fibonacci Sequence: 0 1 1 2 3 5 8 13 21...
	int Fibonacci(int n)
	{
		int nSeqValue = 0;
		if (n == 0)
		{
			nSeqValue = 0;
			return nSeqValue;
		}
		else if (n == 1)
		{
			nSeqValue = 1;
			return nSeqValue;
		}
		else
		{
			return Fibonacci(n - 1) + Fibonacci(n - 2);
		}
	}

	// ex 1.2.4 3)非递归计算斐波那契数列
	int Fibonacci(int n, std::vector<int>& vtFibonacci)
	{
		if (n < 0)
		{
			return -1;
		}
		switch (n)
		{
		case 0:
			vtFibonacci.push_back(0);
			break;
		case 1:
			vtFibonacci.push_back(0);
			vtFibonacci.push_back(1);
			break;
		default:
			vtFibonacci.push_back(0);
			vtFibonacci.push_back(1);
			for (int ii = 2; ii < n; ++ii)
			{
				vtFibonacci.push_back(vtFibonacci[ii - 1] + vtFibonacci[ii - 2]);
			}
			break;
		}
		return vtFibonacci.back();
	}


	// ex 1.2 5 All subset: sub set of list[k:m]
	template <typename T>
	void SubSet(T list[], int k, int m)
	{
		if (k == m)
		{
			cout<<"[] ";
		}
		else
		{
			for (int ii = k; ii <= m; ++ii)
			{
				Swap(list[ii], list[k]);
				SubSet(list, k + 1, m);
				Swap(list[ii], list[k]);
			}
		}
	}

	template void SubSet<char>(char* const, int k, int m);
}

