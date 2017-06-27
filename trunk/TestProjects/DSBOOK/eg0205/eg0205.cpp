#include <iostream>
using namespace std;

#include <afx.h>
#define new DEBUG_NEW


struct Node 
{
	int data;
	Node* next;

	Node(): data(-1), next(0){};

	~Node()
	{
		if (next)
		{
			delete next;
			next = 0;
		}
	}
};

Node* Create(int nlen)
{
	if (nlen <= 0)
	{
		return 0;
	}
	Node* root = new Node();
	root->data = 0;
	Node* p = root;
	for (int ii = 1; ii < nlen; ++ii)
	{
		p->next = new Node();
		p = p->next;
		p->data = ii;
	}

	return root;
}

void Print(Node* p)
{
	while (p)
	{
		cout << p->data << " ";
		p = p->next;
	}
	cout << endl;
}

// 单向链表翻转

Node* Reverse(Node* p)
{
	Node* pp = NULL;
	Node* pn = NULL;

	while (p)
	{
		pn = p->next;
		p->next = pp;
		pp = p;
		p = pn;
	}
	return pp;
}

void eg0205(void)
{
	Node* p =  Create(10);
	Print(p);

	p = Reverse(p);
	Print(p);

	delete p;
}