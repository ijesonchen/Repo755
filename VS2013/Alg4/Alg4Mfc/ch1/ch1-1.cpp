#include "ch1-1.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

//////////////////////////////////////////////////////////////////////////
// 二分搜索
// key在v中的偏移量，0基
int Rank(const int key, const vector<int>& v)
{
	if (key < v.front())
	{
		// mid == v.begin() && key < val will cause exception: v.begin() - 1
		return -1;
	}
	auto lo = v.begin();
	auto hi = v.end() - 1;
	while (lo <= hi)
	{
		auto mid = lo + (hi - lo) / 2;
		auto val = *mid;
		if (key == val)
		{
			return mid - v.begin();
		}
		if (key < val)
		{
			hi = mid - 1;
		}
		else
		{
			lo = mid + 1;
		}
	}
	return -1;
}

// rank with index
int Rank2(const int key, const vector<int>& v)
{
	int lo = 0;
	int hi = v.size() - 1;
	while (lo <= hi)
	{
		auto mid = lo + (hi - lo) / 2;
		auto val = v[mid];
		if (key == val)
		{
			return mid;
		}
		if (key < val)
		{
			hi = mid - 1;
		}
		else
		{
			lo = mid + 1;
		}
	}
	return -1;
}

void testRank(void)
{
	vector<int> v;
	for (int ii = 100; ii; --ii)
	{
		v.push_back(ii - 1);
	}
	sort(v.begin(), v.end());

	vector<int> v2;
	for (int ii = -3; ii < 103; ++ii)
	{
		v2.push_back(ii);
	}
	int k = 0;
	for (auto val : v2)
	{
		cout << "rank val " << val << endl;
		k = Rank2(val, v);
		if (k < 0 ||k != val)
		{
			cout << "not found val " << val << endl;
		}
	}
}

vector<int> ReadInts(const std::string& filename)
{
	vector<int> v;
	fstream f;
	f.open(filename, ios::in);
	if (!f)
	{
		return v;
	}
	int x = 0;
	while (f >> x)
	{
		v.push_back(x);
	}
	return v;
}

vector<int> BinarySearch(const std::string& w, const std::string& t)
{
	auto vw = ReadInts(w);
	std::sort(vw.begin(), vw.end());
	auto vt = ReadInts(t);
	vector<int> vRet;
	for (auto v : vt)
	{
		if (Rank2(v, vw) < 0)
		{
			vRet.push_back(v);
		}
	}
	size_t ni = min(vRet.size(), 6U);
	for (size_t ii = 0; ii < ni; ++ii)
	{
		cout << vRet[ii] << endl;
	}
	return vRet;
}

void egBinarySearch(void)
{
	testRank();
	// 50, 99, 13
	// 3 items
	std::string tinyW = "tinyW.txt";
	std::string tinyT = "tinyT.txt";
	cout << tinyW << " check with " << tinyT << endl;
	auto v1 = BinarySearch(tinyW, tinyT);
	cout << v1.size() << " items" << endl;

	vector<int> v11 = { 50, 99, 13 };
	if (v1 == v11)
	{
		cout << "check passed." << endl;
	}

	// 499569 984875 295754 207807 140925 161828
	// 367966 items
	std::string largeW = "largeW.txt";
	std::string largeT = "largeT.txt";
	cout << largeW << " check with " << largeT << endl;
	auto v2 = BinarySearch(largeW, largeT);
	cout << v2.size() << " items" << endl;
	
	size_t v2cnt = 367966;
	vector<int> v22 = { 499569, 984875, 295754, 207807, 140925, 161828 };
	vector<int> v23(v2.begin(), v2.begin() + 6);
	if (v2.size() == v2cnt && v22 == v23)
	{
		cout << "check passed." << endl;
	}
}

// 1.1.9 正整数 N -> 二进制 string
std::string ex0109f(int n)
{
	string s;
	while (n)
	{
		s = (n % 2 ? "1" : "0") + s;
		n /= 2;
	}
	return s;
}

void ex0109(void)
{
	for (int ii = 0; ii < 128; ++ii)
	{
		cout << ex0109f(ii) << endl;
	}
}

// 1.1.11 打印二位bool数组
void ex0111f(bool* p, int m, int n)
{
	// title
	cout << " ";
	for (int ii = 0; ii < n; ++ii)
	{
		cout << ii;
	}
	cout << endl;
	for (int im = 0; im < m; ++im)
	{
		cout << im;
		for (int in = 0; in < n; ++in)
		{
			if (*(p + im * n + in))
			{
				cout << "*";
			}
			else
			{
				cout << " ";
			}
		}
		cout << endl;
	}
}

void ex0111(void)
{
	const int m = 5;
	const int n = 10;
	bool p[m][n];
	for (int im = 0; im < m; ++im)
	{
		for (int in = 0; in < n; ++in)
		{
			p[im][in] = im * in % 2 ? true : false;
		}
	}

	ex0111f(p[0], m, n);
}


// 1.1.13 MxN数组的转置
void ex0113f(int* p, int m, int n)
{
	for (int in = 0; in < n; ++in)
	{
		for (int im = 0; im < m; ++im)
		{
			cout << *(p + im * n + in) << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void ex0113(void)
{
	const int m = 5;
	const int n = 10;
	int p[m][n];
	int c = -1;
	for (int im = 0; im < m; ++im)
	{
		for (int in = 0; in < n; ++in)
		{
			p[im][in] = ++c;
		}
	}

	ex0113f(p[0], m, n);
}


// 1.1.27 二项分布的递归次数
long long binomial_cnt = 0;
double binomial(int n, int k, double p)
{
	++binomial_cnt;
	if (n == 0 && k == 0)
		return 1.0;
	if (n < 0 || k < 0)
		return 0.0;
	return (1.0 - p) * binomial(n - 1, k, p) + p * binomial(n - 1, k - 1, p);
}

void ex0127(void)
{
	cout << binomial(1, 1, 0.25) << endl;
	cout << binomial_cnt << endl;
}