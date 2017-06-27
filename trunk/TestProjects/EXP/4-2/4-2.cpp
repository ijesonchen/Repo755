#include <iostream>
#include <stdio.h>
#include <string.h>
/*
���룺

�������ȸ���������N��<=105�������N�У�ÿ�и������ǹ۲⵽��һ�������������ơ�
���������ɲ�����30��Ӣ����ĸ�Ϳո���ɣ���Сд�����֣���

�����
���ֵ������������������������Ƽ�����ռ�����İٷֱȣ�����Կո�ָ�����ȷ��С�����4λ��

ʹ����
����ʱ���ֵ�˳�����Ϊ��ǰ�����

���飺
��ȡһ��ʹ��gets
�����һ��ʹ��scanf����ע����һ�е�\n���ܱ�gets��ȡΪ����.
scanf("%s", szBuf)ֻ���ȡһ�����ʶ������С�

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