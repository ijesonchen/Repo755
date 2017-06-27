#include <string>
#include <iostream>
#include <memory>

void TestBag(void);
void TestStatck(void);

// Dijkstra双栈算术表达式求值
void DijkstraEvaluate(void);

// 出入栈序列是否可能
void ex010301(void);

// 中序表达式补全左括号
void ex010309(void);

// 中序表达式转换为后序表达式 InfixToPostfix
void ex010310(void);

// 计算后序表达式 EvaluatePostFix
void ex010311(void);

//////////////////////////////////////////////////////////////////////////
// 节点操作
void ex0103LinkOperation(void);

template<typename T> class BagArray;
template<typename T> class BagList;

//////////////////////////////////////////////////////////////////////////
// Bag with array
template<typename T>
class BagArray
{
public:
	BagArray()
		: pItems(nullptr)
	{
		const int init = 1;
		pItems = new T[init];
		cap = init;
	};

	~BagArray()
	{
		if (pItems)
		{
			delete[] pItems;
		}
	}

	void add(const T& item)
	{	
		pItems[cnt++] = item;
		if (cnt >= cap)
		{
			auto newcap = cap * 2;
			auto newp = new T[newcap];
			for (int ii = 0; ii < cnt; ++ii)
			{
				newp[ii] = pItems[ii];
			}
			delete[] pItems;
			pItems = newp;
			cap = newcap;
		}
	}

	bool isEmpty(void) const
	{
		return cnt == 0;
	}

	int size(void) const
	{
		return cnt;
	}

	int capacity(void) const
	{
		return cap;
	}

	void print(void)
	{
		cout << cnt << " items: " << endl;
		for (int ii = 0; ii < cnt; ++ii)
		{
			cout << pItems[ii] << " ";
		}
		cout << endl;
	}
private:
	T* pItems;
	int cap = 0;
	int cnt = 0;
};

//////////////////////////////////////////////////////////////////////////
// Bag with list
template<typename T>
class BagList
{
public:
	BagList()
		: cnt(0)
	{
		pHdr = new Node();
		pCur = pHdr;
	}

	~BagList()
	{
		auto pa = pHdr;
		auto pb = pa->next;
		delete pa;
		while (pa = pb)
		{
			pb = pa->next;
			delete pa;
		}
	}

	void add(const T& item)
	{
		pCur->next = new Node();
		pCur = pCur->next;
		pCur->data = item;
		++cnt;
	}

	void print(void)
	{
		cout << cnt << " items: " << endl;
		auto p = pHdr->next;
		while (p)
		{
			cout << p->data << " ";
			p = p->next;
		}
		cout << endl;
	}

	bool isEmpty(void) const
	{
		return cnt == 0;
	}

	int size(void) const
	{
		return cnt;
	}

	int capacity(void) const
	{
		return cnt;
	}
private:
	struct Node
	{
		T data;
		Node* next = nullptr;
	};
	
	Node* pHdr;
	Node* pCur;

	int cnt;
};

//////////////////////////////////////////////////////////////////////////
// BagListInt, new item insert to hdr
class BagListInt
{
public:
	BagListInt() 
		: pHdr(nullptr)
		, cnt(0)
	{};

	~BagListInt()
	{
		auto pa = pHdr;
		auto pb = pa->next;
		delete pa;
		while (pa = pb)
		{
			pb = pa->next;
			delete pa;
		}
	}
	
	void add(const int& item)
	{
		auto p = new Node();
		p->data = item;
		p->next = pHdr;
		pHdr = p;
		++cnt;
	}

	void print(void)
	{
		std::cout << cnt << " items: " << std::endl;
		auto p = pHdr;
		while (p)
		{
			std::cout << p->data << " ";
			p = p->next;
		}
		std::cout << std::endl;
	}

	bool isEmpty(void) const
	{
		return cnt == 0;
	}

	int size(void) const
	{
		return cnt;
	}

	int capacity(void) const
	{
		return cnt;
	}
private:
	struct Node
	{
		int data;
		Node* next = nullptr;
	};

	Node* pHdr;
	int cnt;
};

//////////////////////////////////////////////////////////////////////////
// linked stack
template<typename T>
class LinkedStack
{
public:
	LinkedStack()
		: pHdr(nullptr)
		, cnt(0)
	{
	}

	~LinkedStack()
	{
		if (!pHdr)
		{
			return;
		}
		auto pa = pHdr;
		auto pb = pa->next;
		delete pa;
		while (pa = pb)
		{
			pb = pa->next;
			delete pa;
		}
	}

	void push(const T& item)
	{
		auto p = new Node();
		p->data = item;
		p->next = pHdr;
		pHdr = p;
		++cnt;
	}

	T pop()
	{
		if (!pHdr)
		{
			cout << "empty!";
			throw "stack under flow";
		}
		auto d = pHdr->data;
		auto p = pHdr->next;
		delete  pHdr;
		pHdr = p;
		--cnt;
		return d;
	}

	void print(void)
	{
		std::cout << cnt << " items: " << std::endl;
		auto p = pHdr;
		while (p)
		{
			std::cout << p->data << " ";
			p = p->next;
		}
		std::cout << std::endl;
	}

	bool isEmpty(void) const
	{
		return cnt == 0;
	}

	int size(void) const
	{
		return cnt;
	}
private:
	struct Node
	{
		T data;
		Node* next = nullptr;
	};

	Node* pHdr;
	int cnt;
};