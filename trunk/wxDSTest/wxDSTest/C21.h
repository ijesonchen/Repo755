#ifndef C21_h__
#define C21_h__
// main wxWidgets header file
#include <wx/wx.h>

typedef int ElemType;

class SqList
{
public:
	SqList(void){ InitData(); }

	SqList(const ElemType* pElem, unsigned nElem);

	~SqList(){ Clear(); }

	unsigned Length(void){ return m_nElem; }

	int Get(unsigned nIndex, ElemType& elem);

	// 0-based index of elem. -1 if not found.
	int Locate(const ElemType& elem);

	// 0-based index, insert before nIndex, or add to tail if nIndex is next to last node.
	int Insert(const unsigned nIndex, const ElemType& elem);

	// 0-based index, pElem to retrieve node value if not NULL
	int Delete(const unsigned nIndex, ElemType* const pElem);

	// clear data.
	void Clear(void);

protected:
	ElemType* m_pElem;
	// count of element. last_elem_index + 1.
	unsigned m_nElem;
	unsigned m_nCapacity;

	void InitData(void);
	void ExtendCapacity(void);
private:
};

void SqListTest(void);

// ������lb�д��ڵ�la�в����ڵ�Ԫ�ز��뵽la�� **vvvv**
void CombineList(SqList& la, SqList& lb);

// �ϲ�����la, lb��lc��,lc��Ȼ����,��ȥ��
void MergeList1(SqList& la, SqList& lb, SqList& lc);

// �ϲ�����la, lb��lc��,lc��Ȼ����,��ȥ��
void MergeList_review(SqList& la, SqList& lb, SqList& lc);

//////////////////////////////////////////////////////////////////////////
// std functions
// diff: 
// to simplify code: while loop condition include all exception, body do repeat work.
// �ϲ�����la, lb��lc��,lc��Ȼ����,��ȥ��,���Ӵ���
void MergeList1_std(SqList& la, SqList& lb, SqList& lc);

// �ϲ�����la, lb��lc��,lc��Ȼ����ȥ���ظ���
void MergeList2(SqList& la, SqList& lb, SqList& lc);



//////////////////////////////////////////////////////////////////////////
// class LkList

// LkList* pLkList. ��һ��ָ�뱾��Ϊͷָ�룬���洢���ݡ���pLkList->m_eData�����壬pLkList->m_pNextΪ������
class LkList
{
public:
	LkList(void){ InitData(); }

	LkList(const ElemType* pElem, unsigned nElem);

	~LkList(){ Clear(); }

	// total elem count from current node.
	unsigned Length(void);

	// 0-based index of elem. current node is 0  **oooo**
	int Get(unsigned nIndex, ElemType& elem);
	// 0-based index of elem. -1 if not found.
	int Locate(const ElemType& elem);

	// 0-based index, insert before nIndex, or add to tail if nIndex is next to last node.
	int Insert(const unsigned nIndex, const ElemType& elem);


	// 0-based index, insert before nIndex, or add to tail if nIndex is next to last node.
	int Insert2(const unsigned nIndex, const ElemType& elem);

	// 0-based index, pElem to retrieve node value if not NULL
	int Delete(const unsigned nIndex, ElemType* const pElem);

	// 0-based index, pElem to retrieve node value if not NULL
	int Delete2(const unsigned nIndex, ElemType* const pElem);

	void PrintData(void);

	// clear all data, free memory
	void Clear(void);

	//////////////////////////////////////////////////////////////////////////
	// std functions
	// diff: 
	// 1. when locating, step until end then check if reached required node.
	// 2. processing exception first, then follow right procedure (return 0 or true last).
public:

	// 0-based index of elem. current node is 0  **oooo**
	int Get_std(unsigned nIndex, ElemType& elem);

	// 0-based index, insert before nIndex, or add to tail if nIndex is next to last node.
	int Insert_std(const unsigned nIndex, const ElemType& elem);

	// 0-based index, pElem to retrieve node value if not NULL
	int Delete_std(const unsigned nIndex, ElemType* const pElem);

	LkList* m_pNext;
	ElemType m_eData;

protected:
	void InitData(void);
private:
};

void LkListTest(void);

// ��lsa, lsb���շǵݼ��ϲ���lsc,����ģʽ
int MergeLinkList(LkList& lsa, LkList& lsb, LkList& lsc);

// ��lsa, lsb���շǵݼ��ϲ���lsc,�ڵ���뷽ʽ
// �����lsa, lsbΪ��
int MergeLinkList_InsNode(LkList& lsa, LkList& lsb, LkList& lsc);

#endif // C21_h__