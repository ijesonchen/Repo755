#include "ch1.h"
#include "..\Pub\pub.h"
#include <vector>
#include <algorithm>

using namespace std;


/*
1. sort array a. N*logN
2. for i in a, find pos of -i in a (lower_bound performs binary search). logN
3. if a[pos] == -i, found target

N*logN
*/
int CountTwoSumZero(const std::string& filename)
{
	auto nums = ReadIntFromFile(filename);
	sort(nums.begin(), nums.end());

	int cnt = 0;
// 	for (int ii = 0, ni = int(nums.size());
// 		ii < ni; ++ii)
// 	{
// 		auto k = Rank2(-nums[ii], nums);
// 		if (k > ii)
// 		{
// 			++cnt;
// 		}
// 	}

	for (auto it = nums.begin(); it != nums.end(); ++it)
	{
		auto res = lower_bound(it + 1, nums.end(), -*it);
		if (res != nums.end())
		{
			if (*res == -*it)
			{
				++cnt;
			}
		}
	}
	return cnt;
}

void FastTwoSum(void)
{
	vector<string> files = {
		"D:\\devdata\\algs4-data\\1Kints.txt",
		"D:\\devdata\\algs4-data\\2Kints.txt",
		"D:\\devdata\\algs4-data\\4Kints.txt",
		"D:\\devdata\\algs4-data\\8Kints.txt",
// 		"D:\\devdata\\algs4-data\\16Kints.txt",
// 		"D:\\devdata\\algs4-data\\32Kints.txt",
//		"D:\\devdata\\algs4-data\\1Mints.txt"
	};

	vector<int> r = { 1, 2, 3, 19};

	vector<int> o;

	for (auto fn : files)
	{
		o.push_back(CountTwoSumZero(fn));
	}

	CheckResult(r, o);
}


/*
1. sort array a. N*logN
2. for i, j in a, find k == - (i + j) with lower_bound (see TwoSumZero). N*N*logN
N*N*logN
*/
int CountThreeSumZero(const std::string& filename)
{
	auto nums = ReadIntFromFile(filename);
	sort(nums.begin(), nums.end());

	int cnt = 0;

	for (auto ii = nums.begin(); ii != nums.end() - 2; ++ii)
	{
		for (auto ij = ii + 1; ij != nums.end() - 1; ++ij)
		{
			auto nk = - *ii - *ij;
			auto ik = lower_bound(ij + 1, nums.end(), nk);
			if (ik != nums.end())
			{
				if (*ik == nk)
				{
					++cnt;
				}
			}
		}
	}

	return cnt;
}

int CountSlowThreeSumZero(const std::string& filename)
{
	auto nums = ReadIntFromFile(filename);
	int cnt = 0;
	for (auto ii = nums.begin(); ii != nums.end() - 2; ++ii)
	{
		for (auto ij = ii + 1; ij != nums.end() - 1; ++ij)
		{
			for (auto ik = ij + 1; ik != nums.end(); ++ik)
			{
				if (*ii + *ij + *ik == 0)
				{
					++cnt;
				}
			}
		}
	}
	return cnt;
}

void ThreeSum(void)
{
	vector<string> files = {
		"D:\\devdata\\algs4-data\\1Kints.txt",
		"D:\\devdata\\algs4-data\\2Kints.txt",
		"D:\\devdata\\algs4-data\\4Kints.txt",
		"D:\\devdata\\algs4-data\\8Kints.txt",
		// 		"D:\\devdata\\algs4-data\\16Kints.txt",
		// 		"D:\\devdata\\algs4-data\\32Kints.txt",
		//		"D:\\devdata\\algs4-data\\1Mints.txt"
	};

	vector<int> r = { 70, 528, 4039, 32074,
		//		255181, 2052358 
	};

	vector<int> o;

	for (auto fn : files)
	{
		o.push_back(CountSlowThreeSumZero(fn));
	}

	CheckResult(r, o);
}

void FastThreeSum(void)
{
	vector<string> files = {
		"D:\\devdata\\algs4-data\\1Kints.txt",
		"D:\\devdata\\algs4-data\\2Kints.txt",
		"D:\\devdata\\algs4-data\\4Kints.txt",
		"D:\\devdata\\algs4-data\\8Kints.txt",
// 		"D:\\devdata\\algs4-data\\16Kints.txt",
// 		"D:\\devdata\\algs4-data\\32Kints.txt",
//		"D:\\devdata\\algs4-data\\1Mints.txt"
	};

	vector<int> r = { 70, 528, 4039, 32074, 
//		255181, 2052358 
	};

	vector<int> o;

	for (auto fn : files)
	{
		o.push_back(CountThreeSumZero(fn));
	}

	CheckResult(r, o);
}