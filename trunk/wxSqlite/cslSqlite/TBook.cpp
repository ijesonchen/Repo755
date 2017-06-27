#include "TBook.h"
#include <sstream>
#include "..\Sqlite3008006\sqlite3.h"



int cbSelTBook(void* parg, int ncol, char** pvals, char** pnames)
{
	TBook* ptbook = (TBook*)parg;
	if (ncol != ptbook->COLUMN)
	{
		return -1;
	}
	TBook::Column col;
	std::stringstream ss;
	ss << pvals[0] ? pvals[0] : "-1";
	ss >> col.id;
	col.book = pvals[1] ? pvals[1] : "";
	col.author = pvals[2] ? pvals[2] : "";
	col.remark = pvals[3] ? pvals[3] : "";

	ptbook->cols.push_back(col);
	return 0;
}

TBook::TBook(sqlite3* pdb, const char* szname)
: m_pdb(pdb)
, name(szname)
{
}

TBook::~TBook(void)
{
}

int TBook::Load(void)
{
	if (!m_pdb)
	{
		return -1;
	}
	char* selTBook = "select * from book";
	char* szerrmsg = NULL;
	int nRet = sqlite3_exec(m_pdb, selTBook, cbSelTBook, this, &szerrmsg);
	if (nRet != SQLITE_OK)
	{
		return nRet;
	}
	return 0;
}
