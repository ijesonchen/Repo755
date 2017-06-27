/*
9. Palindrome Number

Determine whether an integer is a palindrome. Do this without extra space.

click to show spoilers.

Some hints:
Could negative integers be palindromes? (ie, -1)

If you are thinking of converting the integer to string, note the restriction of using extra space.

You could also try reversing an integer. However, if you have solved the problem "Reverse Integer", you know that the reversed integer might overflow. How would you handle such case?

There is a more generic way of solving this problem.


*/

/*
回文数字判断
负数不应该作为回文

Approach #1 
reverse x to y
x == y 则为回文



*/

#include <iostream>
#include <sstream>
#include <vector>
#include <cstdint>
#include <algorithm>

using namespace std;

class Solution {
public:
	bool isPalindrome(int x) {

	}
};

/*
11507 / 11507 test cases passed.
Status: Accepted
Runtime: 182 ms
*/
class Solution1 {
public:
	bool isPalindrome(int x) {
		// reverse x
		if (x < 0)
		{
			return false;
		}
		int y = x;
		int rev = 0;
		while (y)
		{
			rev = rev * 10 + y % 10;
			y /= 10;
		}
		// 不需判断溢出，溢出则表示非回文
		return rev == x;
	}
};

void Leet009()
{
	// 2147483647, -2147483648
	vector<int> src = { 0, 123, 121, -3, 123321, 12321 };
	vector<bool> dst = { 1, 0, 1, 0, 1, 1 };

	for (size_t ii = 0, ni = src.size(); ii < ni; ++ii)
	{
		auto x = Solution1().isPalindrome(src[ii]);
		if (dst[ii] != x)
		{
			cout << "error: " << src[ii] << " to " << dst[ii] << " with " << x << endl;
		}
	}
	cout << "done" << endl;
}