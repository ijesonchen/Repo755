#include <stdio.h>
#include <assert.h>
#include <stack>
#include <Windows.h>

struct HanoiQuest
{
	char n; // tower hight
	char s; // source
	char t; // temp
	char d; // destination
	HanoiQuest(char ncount = 0, char a = 0, char b = 0, char c = 0)
	: n(ncount)
	, s(a)
	, t(b)
	, d(c)
	{}
};

const int MAX_QUEST_COUNT = 1024;
class HanoiQuestStack
{
protected:
	HanoiQuest pQuests[MAX_QUEST_COUNT]; 
	int nQuest;
public:
	HanoiQuestStack(void)
	{
		memset(pQuests, 0, sizeof(HanoiQuest) * MAX_QUEST_COUNT);
		nQuest = 0;
	}

	void push(HanoiQuest& quest)
	{
		if (nQuest >= MAX_QUEST_COUNT)
		{
			// stack full
			printf("stack full");
			return;
		}
		pQuests[nQuest] = quest;
		++nQuest;
	}
	
	void pop(void)
	{
		--nQuest;
	}

	int size(void)
	{
		return nQuest;
	}

	HanoiQuest top(void)
	{
		return pQuests[nQuest - 1];
	}

};

void HanoiStack(int n, char a, char b, char c)
{
	if (n < 1)
	{
		return;
	}
	HanoiQuestStack stQuests;

	stQuests.push(HanoiQuest(n, a, b, c));

	while (stQuests.size())
	{
		HanoiQuest hanoiQuest = stQuests.top();
		stQuests.pop();

		int nHanoi = hanoiQuest.n;
		char s = hanoiQuest.s;
		char d = hanoiQuest.d;
		char t = hanoiQuest.t;

		if (nHanoi == 1)
		{
//			__asm NOP;
			printf("%c -> %c\n", hanoiQuest.s, hanoiQuest.d);
		}
		else
		{
			stQuests.push(HanoiQuest(nHanoi - 1, t, s, d));
			stQuests.push(HanoiQuest(1, s, t, d));
			stQuests.push(HanoiQuest(nHanoi - 1, s, d, t));
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
template<class T>
class HanoiTower
{
protected:
	char idx;
	std::stack<T> stDatas;
public:
	// 1 based.
	HanoiTower(char ch, int nDataCount = 0)
		: idx (ch)
	{
		for (int ii = nDataCount; ii > 0; --ii)
		{
			Push(ii);
		}
	}

	bool Push(T& newData)
	{
		if (stDatas.size() && 
			(stDatas.top() <= newData))
		{
			assert(false);
			return false;
		}
		stDatas.push(newData);
		return true;
	}

	bool Pop(T& popData)
	{
		if (stDatas.size())
		{
			popData = stDatas.top();
			stDatas.pop();
			return true;
		}
		return false;
	}

	size_t Size(void)
	{
		return stDatas.size();
	}

	bool MoveTo(HanoiTower& another)
	{
		if (!stDatas.size())
		{
			assert(false);
			return false;
		}
		if (another.stDatas.size() &&
			(stDatas.top() >= another.stDatas.top()))
		{
			assert(false);
			return false;
		}
		another.Push(stDatas.top());
		stDatas.pop();
		printf("%c -> %c\n", idx, another.idx);
		return true;
	}

	std::string MoveString(HanoiTower& another)
	{
		if (!stDatas.size())
		{
			assert(false);
			return false;
		}
		if (another.stDatas.size() &&
			(stDatas.top() >= another.stDatas.top()))
		{
			assert(false);
			return false;
		}
		another.Push(stDatas.top());
		stDatas.pop();
		std::string strMove << idx << " -> " << another.idx;
		return strMove;
	}

	void Swap(HanoiTower& another)
	{
		HanoiTower temp = another;
		another = *this;
		*this = temp;
	}
};
// move a -> c , use b as temp.
template<class T>
void HanoiRecur(int nMove, HanoiTower<T>& a, HanoiTower<T>& b, HanoiTower<T>& c)
{
	if (nMove <= 0)
	{
		assert(false);
	}
	else if (nMove == 1)
	{
		a.MoveTo(c);
	}
	else
	{
		HanoiRecur(nMove - 1, a, c, b);
		a.MoveTo(c);
		HanoiRecur(nMove - 1, b, a, c);
	}
}

void HanoiRecurInt(int n, char a, char b, char c)
{
	if (n <= 0)
	{
		return;
	}
	else if (n == 1)
	{
//		__asm NOP;
		printf("%c -> %c\n", a, c);
	}
	else
	{
		HanoiRecurInt(n - 1, a, c, b);
		HanoiRecurInt(1, a, b, c);
		HanoiRecurInt(n - 1, b, a, c);
	}
}

int main(void)
{
	int nTowerHight = 0;
	scanf("%d", &nTowerHight);

	DWORD dw1  = GetTickCount();
	HanoiRecurInt(nTowerHight, 'a', 'b', 'c');
	dw1 = GetTickCount() - dw1;

	DWORD dw2  = GetTickCount();
	HanoiStack(nTowerHight, 'a', 'b', 'c');
	dw2 = GetTickCount() - dw2;

	printf("***%u %u", dw1, dw2);

	return 0;
}