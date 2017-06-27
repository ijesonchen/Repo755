#include <stdio.h>
#include <queue>

int main(void)
{
	int nCount = 0;
	scanf("%d", &nCount);
	std::queue<int> qa;
	std::queue<int> qb;
	int nClient = 0;
	for (int ii = 0; ii < nCount; ++ii)
	{
		scanf("%d", &nClient);
		if (nClient & 1)
		{
			qa.push(nClient);
		}
		else
		{
			qb.push(nClient);
		}
	}
	
	// process
	int nOutput = 0;
	while (qa.size() || qb.size())
	{
		if (qa.size())
		{
			printf("%d", qa.front());
			++nOutput;
			if (nOutput < nCount)
			{
				printf(" ");
			}
			qa.pop();
		}
		if (qa.size())
		{
			printf("%d", qa.front());
			++nOutput;
			if (nOutput < nCount)
			{
				printf(" ");
			}
			qa.pop();
		}
		if (qb.size())
		{
			printf("%d", qb.front());
			++nOutput;
			if (nOutput < nCount)
			{
				printf(" ");
			}
			qb.pop();
		}
	}

	return 0;
}