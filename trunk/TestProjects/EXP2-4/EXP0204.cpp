#include <stdio.h>

struct Node
{
	int data;
	Node* pNext;

	Node(void) : data(0), pNext(0) {};
	Node(int nData): data(nData), pNext(0) {};

	Node& operator =(int nData)
	{
		data = nData;
		return *this;
	}
};

int main(void)
{
	Node* pLink = new Node();

	int nDataCount = 0;
	int nNewData = 0;

	scanf("%d %d", &nDataCount, &nNewData);

	Node* pNode = pLink;
	for (int ii  = 0; ii < nDataCount; ++ii)
	{
		int nData = 0;
		scanf("%d" , &nData);
		pNode->pNext = new Node(nData);
		pNode = pNode->pNext;
	}

	pNode = pLink;
	for (int ii = 0; ii < nDataCount; ++ii)
	{
		if (pNode->pNext)
		{
			if (pNode->pNext->data >= nNewData)
			{
				break;
			}
			pNode = pNode->pNext;
		}
	}
	Node* pNewNode = new Node(nNewData);
	pNewNode->pNext = pNode->pNext;
	pNode->pNext = pNewNode;

	pNode = pLink->pNext;
	if (pNode)
	{
		printf("%d", pNode->data);
	}

	while (pNode->pNext)
	{
		pNode = pNode->pNext;
		printf(" %d", pNode->data);
	}


	return 0;
}