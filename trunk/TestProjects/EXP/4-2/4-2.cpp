#include <iostream>
#include <stdio.h>
#include <string.h>
/*
输入：

输入首先给出正整数N（<=105），随后N行，每行给出卫星观测到的一棵树的种类名称。
种类名称由不超过30个英文字母和空格组成（大小写不区分）。

输出：
按字典序递增输出各种树的种类名称及其所占总数的百分比，其间以空格分隔，精确到小数点后4位。

使用树
插入时按字典顺序，左侧为大。前序输出

经验：
读取一行使用gets
如果上一行使用scanf，则注意上一行的\n可能被gets读取为空行.
scanf("%s", szBuf)只会读取一个单词而非整行。

*/

const unsigned MAX_NAME_LEN = 31;
int cnt = 0;

struct BinNode
{
	char name[MAX_NAME_LEN];
	int cnt;
	BinNode* left;
	BinNode* right;

	~BinNode()
	{
		delete left;
		left = NULL;
		delete right;
		right = NULL;
	}

	BinNode(const char* pName)
		: cnt(1)
		, left(0)
		, right(0)
	{
		size_t nLen = strlen(pName);
		if (nLen >= MAX_NAME_LEN)
		{
			printf("length error: %s", pName);
		}
		memcpy(name, pName, nLen + 1);
	}
};

BinNode* Insert(BinNode* pNode, const char* pName)
{
	if (!pNode)
	{
		pNode = new BinNode(pName);
		return pNode;
	}
	int nCmp = strcmp(pName, pNode->name);
	if (nCmp == 0)
	{
		++pNode->cnt;
	}
	else if (nCmp < 0)
	{
		if (pNode->left)
		{
			Insert(pNode->left, pName);
		}
		else
		{
			pNode->left = new BinNode(pName);
		}
	}
	else
	{
		if (pNode->right)
		{
			Insert(pNode->right, pName);
		}
		else
		{
			pNode->right = new BinNode(pName);
		}
	}
	return pNode;
}

void PreOrder(BinNode* pTree)
{
	if (!pTree)
	{
		return;
	}
	PreOrder(pTree->left);
	float f = (float)pTree->cnt * 100 / cnt;
	printf("%s %.4f%%\n", pTree->name, f);
	PreOrder(pTree->right);
}

int main(void)
{
	char pName[MAX_NAME_LEN] = {0};
	scanf("%d\n", &cnt);
	BinNode* pTree = NULL;
	for (int ii = 0; ii < cnt; ++ii)
	{
//		scanf("%s\n", pName); 
		gets(pName);
		pTree = Insert(pTree, pName);
	}

	PreOrder(pTree);

	return 0;
}