#include <stdio.h>


#define MAX_VALUE 31
int pNumbers[MAX_VALUE] = {0};

int g_nSum = 0;
int g_nOutput = 0;
int g_bNew = 1;

void PrintResult(void)
{
	if (g_bNew)
	{
		g_bNew = 0;
	}
	else
	{
		if (g_nOutput == 4)
		{
			printf("\n");
			g_nOutput = 0;
		}
		else
		{
			printf(";");
		}
	}
	printf("%d=%d", g_nSum, *pNumbers);
	int nn = 1;
	while (pNumbers[nn])
	{
		printf("+%d", pNumbers[nn++]);
	}
	g_nOutput++;
}

void Decomp(int nSum, int nStart, int* pSeq)
{
	if (nSum <= 0)
	{
		*pSeq = 0;
		PrintResult();
		return;
	}
	else if (nSum == nStart)
	{
		*pSeq = nStart;
		pSeq[1] = 0;
		PrintResult();
		return;
	}
	else
	{
		for (int ii = nStart; ii <= nSum; ++ii)
		{
			*pSeq = ii;
			Decomp(nSum - ii, ii, pSeq + 1);
		}
	}
}

int main(void)
{
	scanf("%d", &g_nSum);
	Decomp(g_nSum, 1, pNumbers);

	return 0;
}