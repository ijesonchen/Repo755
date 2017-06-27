/*
Given an array of integers, return indices of the two numbers such that they add up to a specific target.

You may assume that each input would have exactly one solution.

Example:
Given nums = [2, 7, 11, 15], target = 9,

Because nums[0] + nums[1] = 2 + 7 = 9,
return [0, 1].
UPDATE (2016/2/13):
The return format had been changed to zero-based indices. Please read the above updated description carefully.

*/

/*
题目假设每个输入必定含有结果，可以去掉很多判断

Approach #1 暴力枚举 遍历枚举
T: o(n^2)
S: o(1)


Approach #2 二分查找
1. 新数组：排序，二分查找	o(nlogn)
2. 根据值，在原始数组中找到下标 o(n)
	注意相同值的话，需要重新查找

T: o(nlogn)
S: o(n)

Approach #3 哈希map
key-value: a[i], i
T: o(n)
S: o(n)

Approach #4 一次哈希

*/

#include <vector>
#include <algorithm>
#include <iostream>
#include <unordered_map>

using namespace std;

class Solution {
public:
	vector<int> twoSum(vector<int>& nums, int target) {
	}
};

class Solution1 {
public:
	vector<int> twoSum(vector<int>& nums, int target) {
		for (auto it1 = nums.begin(); it1 != nums.end(); ++it1)
		{
			for (auto it2 = it1 + 1; it2 != nums.end(); ++it2)
			{
				if (*it1 + *it2 == target)
				{
					return{ (int)(it1 - nums.begin()), (int)(it2 - nums.begin()) };
				}
			}
		}
		// for error
		return{ 0, 0 };
	}
};

class Solution2 {
public:
	vector<int> twoSum(vector<int>& nums, int target) {
		auto bFound = false;
		auto arr = nums;
		sort(arr.begin(), arr.end());
		int n1 = 0;
		int n2 = 0;
		for (auto it = arr.begin(); it != arr.end(); ++it)
		{
			auto dst = target - *it;
			auto p = lower_bound(it + 1, arr.end(), dst);
			if (p != arr.end() && !(dst < *p))
			{
				n1 = *it;
				n2 = *p;
				bFound = true;
				break;
			}
		}
		if (!bFound)
		{
			return{ 0, 0 };
		}
		auto p1 = find(nums.begin(), nums.end(), n1);
		auto p2 = find(nums.begin(), nums.end(), n2);
		if (p1 == p2)
		{
			p2 = find(p1 + 1, nums.end(), n2);
		}
		if (p1 < p2)
			return{ (int)(p1 - nums.begin()), (int)(p2 - nums.begin()) };
		else
			return{ (int)(p2 - nums.begin()), (int)(p1 - nums.begin()) };
	}
};

class Solution3 {
public:
	vector<int> twoSum(vector<int>& nums, int target) {
		// k-v: num, idx
		unordered_map<int, int> mapnums(nums.size());
		for (auto item = nums.begin(); item != nums.end(); ++item)
		{
			mapnums.insert(make_pair(*item, int(item - nums.begin())));
		}
		for (auto item : mapnums)
		{
			auto num = item.first;
			auto idx = item.second;
			const auto itTarget = mapnums.find(target - item.first);
			if (itTarget != mapnums.end())
			{
				auto val = itTarget->first;
				// found. self?
				if (val == item.first)
				{
					auto it1 = find(nums.begin(), nums.end(), val);
					auto it2 = find(it1 + 1, nums.end(), val);
					return{ (int)(it1 - nums.begin()), (int)(it2 - nums.begin()) };
				}
			}
			else
			{
				return
					item.second < itTarget->second  ?
					vector<int>{ item.second, itTarget->second } :
					vector<int>{ itTarget->second, item.second };
			}
		}
		// for error
		return{ 0, 0 };
	}
};

void Leet001()
{
	vector<int> nums{ 2, 7, 11, 15 };
	int target = 9;
	vector<int> res{ 0, 1 };

	if (res != Solution1().twoSum(nums, target))
	{
		cout << "sln 1 failed." << endl;
	}
	if (res != Solution2().twoSum(nums, target))
	{
		cout << "sln 2 failed." << endl;
	}
}