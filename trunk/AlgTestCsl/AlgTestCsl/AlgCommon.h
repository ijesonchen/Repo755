#pragma once

#include <vector>

namespace AlgCommon
{
	// 1-7 递归计算n!
	int Factorial(int n);

	// 1-8 累加a[0: n-1]
	template<class T>
	T Sum(T a[], int n);

	// 1-9 递归计算a[0: n-1]
	template<class T>
	T Rsum(T a[], int n);

	// 1-10 递归排列字串，list[k:m]的排列
	template<class T>
	void Perm(T list[], int k, int m);

	// 1-11 交换两个值
	template<class T>
	inline void Swap(T& a, T& b);

	// ex. 1.2 4)1 Fibonacci Sequence: 0 1 1 2 3 5 8 13 21...
	int Fibonacci(int n);

	// ex 1.2.4 3)非递归计算斐波那契数列
	int Fibonacci(int n, std::vector<int>& vtFibonacci);

	//////////////////////////////////////////////////////////////////////////
	// ex 1.2 5 All subset: sub set of list[k:m]
	// !!!!!!!!! wrong !!!!!!!!!!!!!
	template <typename T>
	void SubSet(T list[], int k, int m);

	//////////////////////////////////////////////////////////////////////////
	// 经典递归算法
	
	// 1-12 为二维数组分配存储空间
	template <class T>
	bool Make2DArray(T**& x, int rows, int cols)
	{
		try
		{
			x = new T*[rows];
			for (int ii = 0; ii < rows; ++ii)
			{
				x[ii] = new T[cols];
			}

			return true;
		}
		catch (...)
		{
			return false;
		}
	}

	// 1-14 释放Make2DArray分配的空间
	template <class T>
	void Delete2DArray(T**& x, int rows)
	{
		for (int ii = 0; ii < rows; ++ii)
		{
			delete[] x[ii];
		}
		delete[] x;
		x = NULL;
	}

	// ex 1.7 改变一维数组大小
	template <class T>
	void ChangeArraySize(T*& x, int fromSize, int toSize)
	{
		if (fromSize == toSize)
		{
			return;
		}
		T* toArray = new T[toSize];
		int nCopy = (fromSize >= toSize) ? 
					toSize : fromSize;
		memcpy(toArray, x, nCopy * sizeof(T));
		delete x;
		x = toArray;
	}

};
