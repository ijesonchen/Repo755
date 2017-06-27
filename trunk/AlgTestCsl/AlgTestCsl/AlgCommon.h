#pragma once

#include <vector>

namespace AlgCommon
{
	// 1-7 �ݹ����n!
	int Factorial(int n);

	// 1-8 �ۼ�a[0: n-1]
	template<class T>
	T Sum(T a[], int n);

	// 1-9 �ݹ����a[0: n-1]
	template<class T>
	T Rsum(T a[], int n);

	// 1-10 �ݹ������ִ���list[k:m]������
	template<class T>
	void Perm(T list[], int k, int m);

	// 1-11 ��������ֵ
	template<class T>
	inline void Swap(T& a, T& b);

	// ex. 1.2 4)1 Fibonacci Sequence: 0 1 1 2 3 5 8 13 21...
	int Fibonacci(int n);

	// ex 1.2.4 3)�ǵݹ����쳲���������
	int Fibonacci(int n, std::vector<int>& vtFibonacci);

	//////////////////////////////////////////////////////////////////////////
	// ex 1.2 5 All subset: sub set of list[k:m]
	// !!!!!!!!! wrong !!!!!!!!!!!!!
	template <typename T>
	void SubSet(T list[], int k, int m);

	//////////////////////////////////////////////////////////////////////////
	// ����ݹ��㷨
	
	// 1-12 Ϊ��ά�������洢�ռ�
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

	// 1-14 �ͷ�Make2DArray����Ŀռ�
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

	// ex 1.7 �ı�һά�����С
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
