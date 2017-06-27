/*
7. Reverse Integer Add to List
Description  Submission  Solutions
Total Accepted: 214247
Total Submissions: 899497
Difficulty: Easy
Contributors: Admin
Reverse digits of an integer.

Example1: x = 123, return 321
Example2: x = -123, return -321

click to show spoilers.

Have you thought about this?
Here are some good questions to ask before coding. Bonus points for you if you have already thought through this!

If the integer's last digit is 0, what should the output be? ie, cases such as 10, 100.

Did you notice that the reversed integer might overflow? Assume the input is a 32-bit integer, then the reverse of 1000000003 overflows. How should you handle such cases?

For the purpose of this problem, assume that your function returns 0 when the reversed integer overflows.

Note:
The input is assumed to be a 32-bit signed integer. Your function should return 0 when the reversed integer overflows.

Subscribe to see which companies asked this question.

*/

/*
将整数的十进制形式翻转
如 123 -> 321 ,  -123 -> -321
假设输入为32为有符号整形。0或翻转溢出输出0

Approach #1 转换为字串
特殊处理0
将整数转换为字串，翻转为字串，特殊处理'-'，转换为长整形
判断溢出并输出

Approach #2 手写十进制转换，特殊处理

Approach #3
2的基础上 数字数组代替字串
同时由于数字本身带符号，可以省略符号判断

Approach #99
3的基础上 用数字，直接原地转换（leetcode某答案）

*/

#include <iostream>
#include <sstream>
#include <vector>
#include <cstdint>
#include <algorithm>

using namespace std;


std::string i2a(std::int64_t i)
{
	if (!i)
	{
		return "0";
	}
	string s;
	bool neg = false;
	if (i < 0)
	{
		neg = true;
		i = -i;
	}
	while (i)
	{
		s += (i % 10) + '0';
		i /= 10;
	}
	reverse(s.begin(), s.end());
	if (neg)
	{
		s.insert(0, "-");
	}
	return s;
}

// return 0 if overflow
std::int32_t a2i(const std::string& s)
{
	bool neg = false;
	auto beg = s.begin();
	if (*beg == '-')
	{
		neg = true;
		++beg;
	}
	int64_t x = 0;
	while (beg != s.end())
	{
		x += (*beg - '0') * pow(10, s.end() - beg - 1);
		++beg;
	}
	if (neg)
	{
		x = -x;
	}
	if (x > 0x7FFFFFFF || x < static_cast<int32_t>(0x80000000))
	{
		x = 0;
	}
	return static_cast<int32_t>(x);
}

class Solution {
public:
	int reverse(int x) {

	}
};

class Solution1 {
public:
	int reverse(int x) {
		stringstream ss;
		ss << x;
		string s = ss.str();
		auto beg = s.begin();
		auto end = s.end() - 1;
		if (x < 0)
		{
			++beg;
		}
		while (beg < end)
		{
			swap(*beg, *end);
			++beg;
			--end;
		}

		int a = 0;
		stringstream ss1;
		ss1 << s;
		ss1 >> a;
		long long b = 0;
		stringstream ss2;
		ss2 << s;
		ss2 >> b;
		return a == b ? a : 0;
	}
};

class Solution2 {
public:
	int reverse(int x) {
		auto s = i2a(x);
		auto beg = s.begin();
		if (x < 0)
		{
			++beg;
		}
		std::reverse(beg, s.end());
		return a2i(s);
	}
};

class Solution3 {
public:
	int reverse(int x) {
		vector<char> v;
		while (x)
		{
			v.push_back(x % 10);
			x /= 10;
		}
		int64_t y = 0;
		auto p = v.begin();
		while (p != v.end())
		{
			y += (*p) * pow(10, v.end() - p - 1);
			++p;
		}
		if (y > 0x7FFFFFFF || y < static_cast<int32_t>(0x80000000))
		{
			y = 0;
		}
		return y;
	}
};

int64_t pow10(int n)
{
	int64_t x = 1;
	while (n--)
	{
		x *= 10;
	}
	return x;
}

class Solution4 {
public:
	int reverse(int x) {
		char v[10];// = { 0 };
		int idx = 0;
		while (x)
		{
			v[idx++] = x % 10;
			x /= 10;
		}
		int64_t y = 0;
		for (int ii = 0; ii < idx; ++ii)
		{
			auto t = pow10(idx - ii - 1);
			y += (v[ii]) * t;
		}
		if (y > 0x7FFFFFFF || y < static_cast<int32_t>(0x80000000))
		{
			y = 0;
		}
		return y;
	}
};

// 15ms
class Solution5 {
public:
	int reverse(int x) {
		int64_t ans = 0;
		while (x)
		{
			ans = ans * 10 + x % 10;
			x /= 10;
		}

		if (ans > 0x7FFFFFFF || ans < static_cast<int32_t>(0x80000000))
		{
			ans = 0;
		}
		return ans;
	}
};

// 19ms
class Solution99 {
public:
	int reverse(int x) {
		int ans = 0;
		while (x) {
			int temp = ans * 10 + x % 10;
			if (temp / 10 != ans)
				return 0;
			ans = temp;
			x /= 10;
		}

		return ans;
	}
};

void Leet007()
{
// 	char x = '0';
// 	char y = x + 1;
// 	auto s = i2a(123);
// 	s = i2a(-123);
// 	s = i2a(0);
// 	auto t = a2i("123");
// 	t = a2i("-123");
// 	t = a2i("0");
// 	auto a = pow10(0);
// 	a = pow10(1);
// 
// 	a = pow10(5);
	// 2147483647, -2147483648
	vector<int> src = { 0, 123, -123, 2147483647, (int)0x80000000 };
	vector<int> dst = { 0, 321, -321, 0, 0 };

// 	for (size_t ii = 0, ni = src.size(); ii < ni; ++ii)
// 	{
// 		auto x = Solution1().reverse(src[ii]);
// 		if (dst[ii] != x)
// 		{
// 			cout << "error: " << src[ii] << " to " << dst[ii] << " with " << x << endl;
// 		}
// 	}
// 	for (size_t ii = 0, ni = src.size(); ii < ni; ++ii)
// 	{
// 		auto x = Solution2().reverse(src[ii]);
// 		if (dst[ii] != x)
// 		{
// 			cout << "error: " << src[ii] << " to " << dst[ii] << " with " << x << endl;
// 		}
// 	}
// 	for (size_t ii = 0, ni = src.size(); ii < ni; ++ii)
// 	{
// 		auto x = Solution3().reverse(src[ii]);
// 		if (dst[ii] != x)
// 		{
// 			cout << "error: " << src[ii] << " to " << dst[ii] << " with " << x << endl;
// 		}
// 	}
// 	for (size_t ii = 0, ni = src.size(); ii < ni; ++ii)
// 	{
// 		auto x = Solution4().reverse(src[ii]);
// 		if (dst[ii] != x)
// 		{
// 			cout << "error: " << src[ii] << " to " << dst[ii] << " with " << x << endl;
// 		}
// 	}
	for (size_t ii = 0, ni = src.size(); ii < ni; ++ii)
	{
		auto x = Solution5().reverse(src[ii]);
		if (dst[ii] != x)
		{
			cout << "error: " << src[ii] << " to " << dst[ii] << " with " << x << endl;
		}
	}
// 	for (size_t ii = 0, ni = src.size(); ii < ni; ++ii)
// 	{
// 		auto x = Solution99().reverse(src[ii]);
// 		if (dst[ii] != x)
// 		{
// 			cout << "error: " << src[ii] << " to " << dst[ii] << " with " << x << endl;
// 		}
// 	}
//	cout << "done" << endl;

// 	int x[] = { 0, 123, -123, 2147483647, (int)0x80000000 };
// 	Solution4().reverse(x[0]);
// 	Solution4().reverse(x[1]);
// 	Solution4().reverse(x[2]);
// 	Solution4().reverse(x[3]);
// 	Solution4().reverse(x[4]);
}