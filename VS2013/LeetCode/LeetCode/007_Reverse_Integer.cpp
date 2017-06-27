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


*/

#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

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


void Leet007()
{
	// 2147483647, -2147483648
	vector<int> src = { 0, 123, -123, 2147483647, (int)0x80000000 };
	vector<int> dst = { 0, 321, -321, 0, 0 };

	for (size_t ii = 0, ni = src.size(); ii < ni; ++ii)
	{
		auto x = Solution1().reverse(src[ii]);
		if (dst[ii] != x)
		{
			cout << "error: " << src[ii] << " to " << dst[ii] << " with " << x << endl;
		}
	}
}