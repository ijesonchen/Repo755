#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAXN 100000
#define MAXS 30

typedef struct TreeNode *BinTree;
struct TreeNode
{
	char Data[MAXS+1];
	int Cnt;
	BinTree Left;
	BinTree Right;
};

BinTree Insert( BinTree T, char *Name )
{ /* 将树种Name插入二叉搜索树T，或累计已存在树种 */
	int cmp;

	if ( !T ) { /* 建立第1个结点 */
		T = (BinTree)malloc(sizeof(struct TreeNode));
		strcpy(T->Data, Name);
		T->Cnt = 1;
		T->Left = T->Right = NULL;
	}
	else { /* 插入 */
		cmp = strcmp(Name, T->Data); /* 比较名称 */
		if ( cmp<0 )
			T->Left = Insert(T->Left, Name);
		else if ( cmp>0 )
			T->Right = Insert(T->Right, Name);
		else  T->Cnt++; /* 树种存在，计数 */
	}
	return T;
}

void Output( BinTree T, int N )
{
	if (!T) return; /* 递归终止条件 */
	Output(T->Left, N); /* 输出左子树 */
	printf("%s %.4f%c\n", T->Data, (double)T->Cnt/(double)N*100.0, '%');
	Output(T->Right, N); /* 输出右子树 */
}

int main()
{
	int N, i;
	char Name[MAXS+1];
	BinTree T = NULL;

	scanf("%d\n", &N);
	for (i=0; i<N; i++) {
		gets(Name);
		T = Insert(T, Name);
	}
	Output(T, N);

	return 0;
}
