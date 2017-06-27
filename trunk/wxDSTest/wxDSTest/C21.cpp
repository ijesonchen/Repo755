#include "C21.h"

//////////////////////////////////////////////////////////////////////////
// class SqList

SqList::SqList(const ElemType* pElem, unsigned nElem)
{
	InitData();
	m_nCapacity = nElem * 2;
	m_pElem = new ElemType[m_nCapacity];
	memset(m_pElem, NULL, m_nCapacity * sizeof(ElemType));
	memcpy(m_pElem, pElem, nElem * sizeof(ElemType));
	m_nElem = nElem;
}

int SqList::Get(unsigned nIndex, ElemType& elem)
{
	elem = 0;
	if (nIndex >= m_nElem)
	{
		return -1;
	}
	elem = m_pElem[nIndex];
	return 0;
};

// 0-based index of elem. -1 if not found.
int SqList::Locate(const ElemType& elem)
{
	for (unsigned ii = 0; ii < m_nElem; ++ii)
	{
		if (elem == m_pElem[ii])
		{
			return ii;
		}
	}
	return -1;
};

// 0-based index, insert before nIndex, or add to tail if nIndex is next to last node.
int SqList::Insert(const unsigned nIndex, const ElemType& elem)
{
	if (nIndex > m_nElem)
	{
		return -1;
	}
	// check capacity
	if (m_nElem >= m_nCapacity)
	{
		ExtendCapacity();
	}
	if (m_nElem >= m_nCapacity)
	{
		// out of memory
		return -2;
	}

	// move data
	for (unsigned ii = m_nElem; ii > nIndex; --ii)
	{
		m_pElem[ii] = m_pElem[ii - 1];
	}

	// insert data
	m_pElem[nIndex] = elem;
	++m_nElem;
	return 0;
};

// 0-based index, pElem to retrieve node value if not NULL
int SqList::Delete(const unsigned nIndex, ElemType* const pElem)
{
	// check param
	if (nIndex >= m_nElem)
	{
		return -1;
	}

	if (pElem)
	{
		*pElem = m_pElem[nIndex];
	}

	--m_nElem;
	// move data
	for (unsigned ii = nIndex; ii < m_nElem; ++ii)
	{
		m_pElem[ii] = m_pElem[ii + 1];
	}
	// reset last node data
	m_pElem[m_nElem] = 0;
	return 0;
}


// protect members
void SqList::InitData(void)
{
	m_pElem = NULL;
	m_nElem = 0;
	m_nCapacity = 0;
}

void SqList::Clear(void)
{
	m_nElem = 0;
	m_nCapacity = 0;
	delete m_pElem;
	m_pElem = NULL;
}

void SqList::ExtendCapacity(void)
{
	if (!m_nCapacity)
	{
		++m_nCapacity;
	}
	m_nCapacity *= 2;
	ElemType* pElem = new ElemType[m_nCapacity];
	memset(pElem, NULL, m_nCapacity * sizeof(ElemType));
	memcpy(pElem, m_pElem, m_nElem * sizeof(ElemType));
	delete m_pElem;
	m_pElem = pElem;
}


//////////////////////////////////////////////////////////////////////////
// functions
const ElemType pElem1[] = {1,3,5,7,9};
const ElemType pElem2[] = {2,4,6,8,10,12,14,16};

void SqListTest(void)
{
	// list member test
	SqList ls(pElem1, sizeof(pElem1) / sizeof(ElemType));

	ElemType elem = 0;
	int nRet = 0;
	nRet = ls.Get(3, elem);
	elem = 10;
	nRet = ls.Insert(5, elem);
	nRet = ls.Delete(5, &elem);
	elem = 5;
	nRet = ls.Insert(0, elem);
	elem = 6;
	nRet = ls.Insert(4, elem);
	elem = 7;
	nRet = ls.Insert(7, elem);
	elem = 5;
	nRet = ls.Insert(0, elem);
	elem = 6;
	nRet = ls.Insert(4, elem);
	elem = 7;
	nRet = ls.Insert(7, elem);

	nRet = ls.Length();
	nRet = ls.Locate(elem);
	elem = 3;
	nRet = ls.Locate(elem);

	SqList ls1(pElem1, 5);

	SqList ls2(pElem2, sizeof(pElem2) / sizeof(ElemType));

	CombineList(ls1, ls2);

	SqList ls3(pElem1, sizeof(pElem1) / sizeof(ElemType));
	SqList ls4;

	MergeList1_std(ls2, ls3, ls4);

	SqList ls5;
	ls5.Clear();
	MergeList_review(ls2, ls3, ls5);
	SqList ls6;
	SqList ls7;
	MergeList_review(ls2, ls6, ls7);
}


// 将所有lb中存在但la中不存在的元素插入到la中 **vvvv**
void CombineList(SqList& la, SqList& lb)
{
	unsigned lenLa = la.Length();
	unsigned lenLb = lb.Length();
	for (unsigned ii = 0, iCount = lenLb;
		ii < iCount; ++ii)
	{
		ElemType e;
		lb.Get(ii, e);
		if (-1 == la.Locate(e))
		{
			la.Insert(lenLa++, e);
		}
	}
}


// 合并有序la, lb到lc中,lc仍然有序,不去重
void MergeList1(SqList& la, SqList& lb, SqList& lc)
{
	unsigned lenLa = la.Length();
	unsigned lenLb = lb.Length();
	unsigned ia = 0;
	unsigned ib = 0;
	unsigned ic = 0;

	ElemType ea = 0;
	ElemType eb = 0;

	bool bRuning = true;

	while (bRuning)		// 简洁：使用 ia < lenLa && ib < lenLb作为条件
	{
		if (ia >= lenLa)
		{
			break;
		}
		la.Get(ia, ea);
		if (ib >= lenLb)
		{
			break;
		}
		lb.Get(ib, eb);

		if (ea <= eb)
		{
			lc.Insert(ic++, ea);
			++ia;
		}
		else
		{
			lc.Insert(ic++, eb);
			++ib;
		}
	}

	if (ia >= lenLa)		// 简洁：分别对la, lb使用while循环insert剩余元素
	{
		for (; ib < lenLb;)
		{
			lb.Get(ib++, eb);
			lc.Insert(ic++, eb);
		}
	}
	else if (ib >= lenLb)
	{
		for (; ia < lenLa;)
		{
			la.Get(ia++, ea);
			lc.Insert(ic++, ea);
		}
	}
}


// 合并有序la, lb到lc中,lc仍然有序,不去重
void MergeList_review(SqList& la, SqList& lb, SqList& lc)
{
/*
	na;
	nb;
	while (ia < na && ib < nb)
	{
		ea = la.Get(ia);
		eb = lb.Get(ib);
		if (ea <= eb)
			lc.Add(ea);
			++ia;
		else
			lc.Add(eb);
			++eb;
	}

	lc.Add(la.rest)
	lc.Add(lb.rest)

*/


	unsigned na = la.Length();
	unsigned nb = lb.Length();

	unsigned ia = 0;
	unsigned ib = 0;
	ElemType ea = 0;
	ElemType eb = 0;

	unsigned ic = 0;

	if (na == 0)
	{
		// copy lb to lc and return.
		while (ib < nb)
		{
			lc.Insert(ic++, eb);
			if (lb.Get(ib++, eb))
			{
				break;
			}
		}
		return;
	}
	else if (nb == 0)
	{
		// copy la to lc and return.
		while (ia < na)
		{
			if (la.Get(ia++, ea))
			{
				break;
			}
			lc.Insert(ic++, ea);
		}

		return;
	}


	if (la.Get(ia, ea))
	{
		// error.
		ia = na;
	}
	if (lb.Get(ib, eb))
	{
		ib = nb;
	}

	while (ia < na && ib < nb) 
	{
		if (ea <= eb)
		{
			lc.Insert(ic++, ea);
			if (la.Get(++ia, ea))
			{
				// error.
				ia = na;
			}
		}
		else
		{
			lc.Insert(ic++, eb);
			if (lb.Get(++ib, eb))
			{
				ib = nb;
			}
		}
	}
	
	while (ia < na)
	{
		if (la.Get(ia++, ea))
		{
			break;
		}
		lc.Insert(ic++, ea);
	}
	while (ib < nb)
	{
		lc.Insert(ic++, eb);
		if (lb.Get(ib++, eb))
		{
			break;
		}
	}
}

// 合并有序la, lb到lc中,lc仍然有序,不去重,例子代码
void MergeList1_std(SqList& la, SqList& lb, SqList& lc)
{
	// diff: to simplify code: while loop condition include all exception, body do repeat work.
	unsigned lenLa = la.Length();
	unsigned lenLb = lb.Length();
	unsigned ia = 0;
	unsigned ib = 0;
	unsigned ic = 0;

	ElemType ea = 0;
	ElemType eb = 0;

	bool bRuning = true;

	while ((ia < lenLa) && (ib < lenLb))
	{
		la.Get(ia, ea);
		lb.Get(ib, eb);

		if (ea <= eb)
		{
			lc.Insert(ic++, ea);
			++ia;
		}
		else
		{
			lc.Insert(ic++, eb);
			++ib;
		}
	}

	while (ia < lenLa)
	{
		la.Get(ia++, ea);
		lc.Insert(ic++, ea);
	}

	while (ib < lenLb)
	{
		lb.Get(ib++, eb);
		lc.Insert(ic++, eb);
	}
}

// 合并有序la, lb到lc中,lc仍然有序，去除重复项
void MergeList2(SqList& la, SqList& lb, SqList& lc)
{
	unsigned lenLa = la.Length();
	unsigned lenLb = lb.Length();
	unsigned ia = 0;
	unsigned ib = 0;
	unsigned ic = 0;

	ElemType ea = 0;
	ElemType eb = 0;

	bool bRuning = true;

	while (bRuning)
	{
		if (ia >= lenLa)
		{
			break;
		}
		la.Get(ia, ea);
		if (ib >= lenLb)
		{
			break;
		}
		lb.Get(ib, eb);

		if (ea <= eb)
		{
			lc.Insert(ic++, ea);
			++ia;
		}
		else
		{
			lc.Insert(ic++, eb);
			++ib;
		}

		if (ea == eb)
		{
			++ib;
		}
	}
	if (ia >= lenLa)
	{
		for (; ib < lenLb;)
		{
			lb.Get(ib++, eb);
			lc.Insert(ic++, eb);
		}
	}
	else if (ib >= lenLb)
	{
		for (; ia < lenLa;)
		{
			la.Get(ia++, ea);
			lc.Insert(ic++, ea);
		}
	}
}



//////////////////////////////////////////////////////////////////////////
// class LkList

LkList::LkList(const ElemType* pElem, unsigned nElem)
{
	if (!pElem || !nElem)
	{
		return;
	}
	m_eData = 0;
	m_pNext = NULL;

	unsigned ii = 0;
	LkList* pNext = this;

	while (ii < nElem)
	{
		pNext->m_pNext = new LkList();
		pNext = pNext->m_pNext;

		pNext->m_pNext = NULL;
		pNext->m_eData = pElem[ii++];
	}
}

void LkList::InitData(void)
{
	m_pNext = NULL;
	m_eData = 0;
}

// clear all data, free memory
void LkList::Clear(void)
{
	delete m_pNext;
	m_pNext = NULL;
}

// total elem count from current node.
unsigned LkList::Length(void)
{
	unsigned nCount = 0;
	LkList* pNext = m_pNext;
	while (pNext)
	{
		++nCount;
		pNext = pNext->m_pNext;
	}
	return nCount;
}

// 0-based index of elem. current node is 0  **oooo**
int LkList::Get(unsigned nIndex, ElemType& elem)
{
	elem = 0;
	unsigned ii = 0;
	LkList* pNext = m_pNext;

	while (pNext)
	{
		// found
		if (nIndex == ii)
		{
			elem = pNext->m_eData;
			return 0;
		}
		// not found, next
		pNext = pNext->m_pNext;
		++ii;
	}

	return -1;
}

// 0-based index of elem. current node is 0  **oooo**
int LkList::Get_std(unsigned nIndex, ElemType& elem)
{
	// diff: check while searching -> search first, then check.
	elem = 0;
	unsigned ii = 0;
	LkList* pNext = m_pNext;

	while (pNext && ii < nIndex)
	{
		// not found, next
		pNext = pNext->m_pNext;
		++ii;
	}

	if (!pNext || nIndex > ii)	// ?? impossible for nIndex > ii
	{
		return -1;
	}

	elem = pNext->m_eData;
	return 0;
}

// 0-based index of elem. -1 if not found.
int LkList::Locate(const ElemType& elem)
{
	int ii = 0;
	LkList* pNext = m_pNext;

	while (pNext)
	{
		// found
		if (elem == pNext->m_eData)
		{
			return ii;
		}
		// not found, next
		pNext = pNext->m_pNext;
		++ii;
	}

	return -1;
}

// 0-based index, insert before nIndex, or add to tail if nIndex is next to last node.
int LkList::Insert(const unsigned nIndex, const ElemType& elem)
{
	unsigned ii = 0;
	LkList* pNext = m_pNext;
	LkList* pPrev = this;
	while (pNext)
	{
		// found, return
		if (nIndex == ii)
		{
			pPrev->m_pNext = new LkList();
			pPrev->m_pNext->m_eData = elem;
			pPrev->m_pNext->m_pNext = pNext;
			return 0;
		}
		pPrev = pNext;	// save prev node for tail
		// not found, next
		pNext = pNext->m_pNext;
		++ii;
	}

	// tail node
	if (nIndex == ii)
	{
		pPrev->m_pNext = new LkList();
		pPrev->m_pNext->m_eData = elem;
		pPrev->m_pNext->m_pNext = NULL;
		return 0;
	}

	return -1;
}


// 0-based index, insert before nIndex, or add to tail if nIndex is next to last node.
int LkList::Insert_std(const unsigned nIndex, const ElemType& elem)
{
	// diff: check while searching -> search first, then check.
	unsigned ii = 0;
	LkList* pParent = this;

	// stepping
	while (pParent->m_pNext && ii < nIndex)
	{
		pParent = pParent->m_pNext;	
		++ii;
	}

	// check
	if (nIndex > ii ||	// ?? impossible
		nIndex != ii ||	// get node (include tail)
		(pParent->m_pNext == NULL && nIndex < ii))	// not append to tail, ?? included in nIndex != ii
	{
		return -1;
	}

	LkList* pNew = new LkList();
	pNew->m_eData = elem;
	pNew->m_pNext = pParent->m_pNext;
	pParent->m_pNext = pNew;

	return 0;
}

// 0-based index, insert before nIndex, or add to tail if nIndex is next to last node.
int LkList::Insert2(const unsigned nIndex, const ElemType& elem)
{
	unsigned ii = 0;
	LkList* p = this;
	while (p->m_pNext)
	{
		// p->m_pNext found, insert before 
		if (nIndex == ii)
		{
			LkList* pTemp = p->m_pNext;
			p->m_pNext = new LkList();
			p->m_pNext->m_eData = elem;
			p->m_pNext->m_pNext = pTemp;
			return 0;
		}
		p = p->m_pNext;
		++ii;
	}

	// tail node
	if (nIndex == ii)
	{
		p->m_pNext = new LkList();
		p->m_pNext->m_eData = elem;
		p->m_pNext->m_pNext = NULL;
		return 0;
	}

	return -1;
}

// 0-based index, pElem to retrieve node value if not NULL
int LkList::Delete(const unsigned nIndex, ElemType* const pElem)
{
	if (pElem)
	{
		// reset output
		*pElem = 0;
	}

	// step to nIndex
	unsigned ii = 0;
	LkList* pPrev = this;
	LkList* pNext = m_pNext;
	while (pNext)
	{
		if (nIndex == ii)
		{
			// pNext found;
			// save
			if (pElem)
			{
				*pElem = pNext->m_eData;
			}
			// remove node
			pPrev->m_pNext = pNext->m_pNext;
			pNext->m_pNext = NULL;
			// release memory
			delete pNext;
			return 0;
		}
		pPrev = pNext;
		pNext = pNext->m_pNext;
		++ii;
	}
	// not found.
	return -1;
}

// 0-based index, pElem to retrieve node value if not NULL
int LkList::Delete_std(const unsigned nIndex, ElemType* const pElem)
{
	// diff: check while searching -> search first, then check.

	if (pElem)
	{
		// reset output
		*pElem = 0;
	}

	unsigned ii = 0;
	LkList* pParent = this;

	// step...
	while (pParent->m_pNext && ii < nIndex)
	{
		pParent = pParent->m_pNext;
		++ii;
	}

	// check if achieve
	if (ii > nIndex ||
		ii != nIndex ||
		!pParent->m_pNext)
	{
		return -1;
	}

	// found
	LkList* pFound = pParent->m_pNext;

	// save
	if (pElem)
	{
		*pElem = pFound->m_eData;
	}

	// remove
	pParent->m_pNext = pFound->m_pNext;
	pFound->m_pNext = NULL;

	// free memory
	delete pFound;

	return 0;
}


// 0-based index, pElem to retrieve node value if not NULL
int LkList::Delete2(const unsigned nIndex, ElemType* const pElem)
{
	// locate to index
	unsigned ii = 0;
	LkList* pPrev = this;
	// step pPrev->m_pNext to index
	while (pPrev->m_pNext)
	{
		if (nIndex == ii)
		{
			// pPrev->m_pNext located.
			LkList* p = pPrev->m_pNext;
			// save
			if (pElem)
			{
				*pElem = p->m_eData;
			}
			pPrev->m_pNext = p->m_pNext;
			p->m_pNext = NULL;
			// free deleted
			delete p;
			return 0;
		}
		pPrev = pPrev->m_pNext;
		++ii;
	}

	return -1;
}

#include <wx/sstream.h>
void LkList::PrintData(void)
{
	wxString strData;
	LkList* pNext = m_pNext;
	while (pNext)
	{
		strData << pNext->m_eData << " ";
		pNext = pNext->m_pNext;
	}
	wxLogMessage(strData);
}


void LkListTest(void)
{
	unsigned nLength = 0;
	int nRet = 0;
	LkList lk1(pElem1, sizeof(pElem1) / sizeof(ElemType));
	lk1.PrintData();
	nLength = lk1.Length();

	ElemType elem = 0;

	nRet = lk1.Get_std(0, elem);
	nRet = lk1.Get_std(2, elem);
	nRet = lk1.Get_std(4, elem);
	nRet = lk1.Get_std(5, elem);

	elem = 100;
	nRet = lk1.Insert_std(0, elem);
	elem = 101;
	nRet = lk1.Insert_std(3, elem);
	elem = 102;
	nRet = lk1.Insert_std(6, elem);
	elem = 103;
	nRet = lk1.Insert_std(8, elem);
	elem = 104;
	nRet = lk1.Insert_std(11, elem);

	LkList lk2(pElem2, sizeof(pElem2) / sizeof(ElemType));
	lk2.PrintData();

	elem = 0;
	nRet = lk2.Delete_std(0, &elem);
	elem = 0;
	nRet = lk2.Delete_std(4, &elem);
	elem = 0;
	nRet = lk2.Delete_std(7, &elem);
	elem = 0;
	nRet = lk2.Delete_std(7, &elem);
	
	elem = 100;
	nRet = lk1.Insert_std(0, elem);
	nLength = lk1.Length();
	nRet = lk1.Delete_std(3, &elem);
	elem = 101;
	nRet = lk1.Insert2(3, elem);
	nLength = lk1.Length();
	nRet = lk1.Delete2(2, &elem);
	elem = 102;
	nRet = lk1.Insert(5, elem);
	nLength = lk1.Length();
	elem = 103;
	nRet = lk1.Insert(7, elem);
	nLength = lk1.Length();

	elem = 101;
	nRet = lk1.Locate(elem);
	elem = 102;
	nRet = lk1.Locate(elem);




	lk2.PrintData();
	nLength = lk2.Length();


	LkList lk3(pElem1, sizeof(pElem1) / sizeof(ElemType));
	LkList lk4(pElem2, sizeof(pElem2) / sizeof(ElemType));
	LkList lk5;
	MergeLinkList(lk3, lk4, lk5);

	LkList lk7(pElem1, sizeof(pElem1) / sizeof(ElemType));
	LkList lk8(pElem2, sizeof(pElem2) / sizeof(ElemType));
	LkList lk9;
	MergeLinkList_InsNode(lk7, lk8, lk9);
}


// 将lsa, lsb按照非递减合并到lsc
int MergeLinkList(LkList& lsa, LkList& lsb, LkList& lsc)
{
	if (lsc.m_pNext)
	{
		// lsc has data
		return -1;
	}

	LkList* pa = lsa.m_pNext;
	LkList* pb = lsb.m_pNext;
	LkList* pc = &lsc;

	if (!pa && !pb)
	{
		// one is empty
		return -2;
	}

	while (pa && pb)
	{
		if (pa->m_eData <= pb->m_eData)
		{
			pc->m_pNext = new LkList();
			pc = pc->m_pNext;

			pc->m_eData = pa->m_eData;
			pc->m_pNext = NULL;

			// step to next
			pa = pa->m_pNext;
		}
		else
		{
			pc->m_pNext = new LkList();
			pc = pc->m_pNext;

			pc->m_eData = pb->m_eData;
			pc->m_pNext = NULL;

			// step to next
			pb = pb->m_pNext;
		}
	}

	while (pa)
	{
		pc->m_pNext = new LkList();
		pc = pc->m_pNext;

		pc->m_eData = pa->m_eData;
		pc->m_pNext = NULL;

		// step to next
		pa = pa->m_pNext;
	}

	while (pb)
	{
		pc->m_pNext = new LkList();
		pc = pc->m_pNext;

		pc->m_eData = pb->m_eData;
		pc->m_pNext = NULL;

		// step to next
		pb = pb->m_pNext;
	}

	return 0;
}


// 将lsa, lsb按照非递减合并到lsc,节点插入方式
// 插入后，lsa, lsb为空
int MergeLinkList_InsNode(LkList& lsa, LkList& lsb, LkList& lsc)
{
	if (lsc.m_pNext)
	{
		// lsc has data
		return -1;
	}

	LkList* pa = lsa.m_pNext;
	LkList* pb = lsb.m_pNext;
	LkList* pc = &lsc;

	if (!pa && !pb)
	{
		// one is empty
		return -2;
	}

	while (pa && pb)
	{
		if (pa->m_eData <= pb->m_eData)
		{
			pc->m_pNext = pa;

			pc = pa;

			pa = pa->m_pNext;
		}
		else
		{
			pc->m_pNext = pb;
			pc = pb;
			pb = pb->m_pNext;
		}
	}

	if (pa)
	{
		pc->m_pNext = pa;
	}

	if (pb)
	{
		pc->m_pNext = pb;
	}

	lsa.m_pNext = NULL;
	lsb.m_pNext = NULL;
}