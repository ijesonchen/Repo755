#include <stdio.h>

const int MAX_NODE = 50;
char szPreOrder[MAX_NODE] = {0};
char szMidOrder[MAX_NODE] = {0};
int nNode = 0;


struct Node
{
	char data;
	Node* left;
	Node* right;

	Node(): data(0), left(0), right(0){};

	~Node()
	{
		if (left) {	delete left; left = 0;}
		if (right) { delete right; right = 0;}
	}
};


/*
	递归为：前序、中序 -> 树。
	前序、中序性质：前序首元素为根元素
	前序：根，左树元素，右树元素
	中序：左树元素，根，右树元素
	其中前序、中序的左树元素数量相等。右树元素亦然。
*/
Node* RestoreTree(char* ppre, char* pin, int n)
{
	if (!ppre || !n)
	{
		return NULL;
	}
	Node* pNode = new Node();
	const char data = *ppre;
	pNode->data = data;
	// find data in pin
	// for pin: left nodes: ii, right nodes: n - 1 - ii
	int ii = 0;
	for (; ii < n; ++ii)
	{
		if (data == pin[ii])
		{
			break;
		}
	}
	if (ii && ii == n)
	{
		// should not get there
		printf("error");
	}

	pNode->left = RestoreTree(ppre + 1, pin, ii);
	pNode->right = RestoreTree(ppre + ii + 1, pin + ii + 1, n - ii - 1);

	return pNode;
}

int TreeHight(Node* pTree)
{
	if (!pTree)
	{
		return 0;
	}
	int n1 = TreeHight(pTree->left);
	int n2 = TreeHight(pTree->right);
	
	return 1 + (n1 > n2 ? n1 : n2);
}


int main(void)
{
	// read data
	scanf("%d\n", &nNode);
	for (int ii = 0; ii < nNode; ++ii)
	{
		scanf("%c", &szPreOrder[ii]);
	}
	getchar();
	for (int ii = 0; ii < nNode; ++ii)
	{
		scanf("%c", &szMidOrder[ii]);
	}

	Node* pTree = 0;

	pTree = RestoreTree(szPreOrder, szMidOrder, nNode);

	printf("%d", TreeHight(pTree));

	delete pTree;

	return 0;
	
}