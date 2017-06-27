#include <stdio.h>

int nTotalStudent = 0;
int nTotalGroup = 0;

#define MAX_STUDENT 30001

int pData[MAX_STUDENT];

int GetHeaderID(int id)
{
	int n = id;

	while (pData[n] > 0)
	{
		n = pData[n];
	}
	return n;
}

int GetMax(void)
{
	int nMemberCount = 0;
	for (int ii = 1; ii <= nTotalStudent; ii++)
	{
		if (nMemberCount >= pData[ii])
		{
			nMemberCount = pData[ii];
		}
	}

	return -nMemberCount;
}

int Combine(int head1, int head2)
{
	if (pData[head1] < pData[head2])
	{
		pData[head1] += pData[head2];
		pData[head2] = head1;
		return head1;
	}
	else
	{
		pData[head2] += pData[head1];
		pData[head1] = head2;
		return head2;
	}
}

void ReadOneClub(void)
{
	int n = 0;
	int id = 0;
	scanf("%d %d", &n, &id);
	int head1 = GetHeaderID(id);
	for (int ii = 1; ii < n; ++ii)
	{
		scanf("%d", &id);
		int head2 = GetHeaderID(id);
		if (head1 != head2)
		{
			head1 = Combine(head1, head2);
		}
	}
	
}

int main(void)
{
	scanf("%d %d", &nTotalStudent, &nTotalGroup);

	// init array
	for (int ii = 1; ii <= nTotalStudent; ++ii)
	{
		pData[ii] = -1;
	}

	// read one group
	for (int ig = 0; ig < nTotalGroup; ++ig)
	{
		// read group member count & first member
		ReadOneClub();
	}

	printf("%d", GetMax());


	return 0;
}