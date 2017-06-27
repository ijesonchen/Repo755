#include "..\Sqlite3008006\sqlite3.h"
#include <iostream>
#include <sstream>
using namespace std;

#include <locale>

#include "TableInfo.h"

/*
test for sqlite c/c++ interface routine
sqlite3_open() 
sqlite3_prepare() 
sqlite3_step() 
sqlite3_column() 
sqlite3_finalize() 
sqlite3_close() 

class TableInfo实现cslSqlite中SchemaDB功能，但是使用动态数据类型。
未完成
*/

inline const char* Val(const char* pval)
{
	return pval ? pval : "null";
}

inline const char* Val(const unsigned char* pval)
{
	const char* p = (const char*)pval;
	return p ? p : "null";
}

// test for UTF-16
void Test16(void);

int main(void)
{
	setlocale(LC_ALL, "chs");
	Test16();
	char* szFileName = "D:\\data\\sqlite\\test1.db";
	sqlite3* pdb = NULL;
	sqlite3_stmt* pStmt = NULL;

	int nRet = 0;

	nRet = sqlite3_open(szFileName, &pdb);
	if (nRet)
	{
		printf("open file %s error.\n", szFileName);
		sqlite3_close(pdb);
		return -1;
	}
	try
	{
		char* zsql = "select * from book";

		nRet = sqlite3_prepare_v2(pdb, zsql, strlen(zsql) + 1, &pStmt, NULL);
		if (nRet != SQLITE_OK || !pStmt)
		{
			stringstream ss;
			ss << "sqlite3_prepare_v2 error: " << nRet << endl;
			throw ss.str();
		}

		bool bHeaderOutput = true;
		int ncol = sqlite3_column_count(pStmt);
		if (ncol <= 0)
		{
			stringstream ss;
			ss << "sqlite3_column_count returns no data: " << ncol << endl;
			throw ss.str();
		}

		while ((nRet = sqlite3_step(pStmt)) == SQLITE_ROW)
		{
			if (bHeaderOutput)
			{
				stringstream ss;
				ss << sqlite3_column_name(pStmt, 0);
				for (int ii = 1; ii < ncol; ++ii)
				{
					ss << "|" << sqlite3_column_name(pStmt, ii);
				}
				ss << endl;
				cout << ss.str();
				bHeaderOutput = false;
			}

			stringstream ss2;
			ss2 << Val(sqlite3_column_text(pStmt, 0));
			for (int ii = 0; ii < ncol; ++ii)
			{
				int ntype = sqlite3_column_type(pStmt, ii);
				const char* pVal =  Val(sqlite3_column_text(pStmt, ii));
				double d = sqlite3_column_double(pStmt, ii);
				int n = sqlite3_column_int(pStmt, ii);
				sqlite3_int64 n2 = sqlite3_column_int64(pStmt, ii);
				char* p = (char*)sqlite3_column_blob(pStmt, ii);
				int nb = sqlite3_column_bytes(pStmt, ii);
				ss2 << "|" << pVal;
			}
			ss2 << endl;
			cout << ss2.str();
		}
		if (nRet == SQLITE_ERROR)
		{
			throw -2;
		}

	}
	catch (string e)
	{
		stringstream ss;
		ss << "error: " << e << endl;
		cout << ss.str();
	}
	catch (int e)
	{
		stringstream ss;
		ss << "error "<< e << " : " << sqlite3_errmsg(pdb) << endl;
		cout << ss.str();
	}

	if (pStmt)
	{
		nRet = sqlite3_finalize(pStmt);
		pStmt = NULL;
	}

	TableInfo table("book");
	bool b = table.GetTableInfo(pdb);

	// 使用sqlite3_get_table

	const char* zSql = "select * from book";
	char** azResults = NULL;
	int nRow = 0;
	int nCol = 0;
	char* zErrmsg = NULL;

	cout << endl << "using sqlite3_get_table" << endl;
	nRet = sqlite3_get_table(pdb, zSql, &azResults, &nRow, &nCol, &zErrmsg);
	if (nRet != SQLITE_OK)
	{
		stringstream ss;
		ss << "select error: " << zErrmsg << endl;
	}
	else
	{
		// col title has one row.
		for (int ii = 0; ii <= nRow; ++ii)
		{
			stringstream ss;
			ss << Val(azResults[ii * nCol]);
			for (int jj = 0; jj < nCol; ++jj)
			{
				ss << "|" << Val(azResults[ii * nCol + jj]);
			}
			ss << endl;
			cout << ss.str();
		}
	}

	nRet = sqlite3_close(pdb);
	pdb = NULL;

	return 0;
}

void Test16(void)
{
	char* szFileName = "D:\\data\\sqlite\\test1.db";
	sqlite3* pdb = NULL;
	sqlite3_stmt* pStmt = NULL;

	wchar_t* pw = 0;

	int nRet = 0;

	nRet = sqlite3_open(szFileName, &pdb);
	if (nRet)
	{
		printf("open file %s error.\n", szFileName);
		sqlite3_close(pdb);
		return ;
	}
	try
	{
		wchar_t* zsql = L"select * from memberu";

		nRet = sqlite3_prepare16_v2(pdb, zsql, (wcslen(zsql) + 1) * 2, &pStmt, NULL);
		if (nRet != SQLITE_OK || !pStmt)
		{
			stringstream ss;
			ss << "sqlite3_prepare_v2 error: " << nRet << endl;
			throw ss.str();
		}

		bool bHeaderOutput = true;
		int ncol = sqlite3_column_count(pStmt);
		if (ncol <= 0)
		{
			stringstream ss;
			ss << "sqlite3_column_count returns no data: " << ncol << endl;
			throw ss.str();
		}

		while ((nRet = sqlite3_step(pStmt)) == SQLITE_ROW)
		{
			if (bHeaderOutput)
			{
				wstringstream ss;
				pw = (wchar_t*)sqlite3_column_name16(pStmt, 0);
				ss << pw;
				for (int ii = 1; ii < ncol; ++ii)
				{
					pw = (wchar_t*)sqlite3_column_name16(pStmt, ii);
					ss << L"|";
					ss << pw;
				}
				ss << endl;
				wcout << ss.str();
				bHeaderOutput = false;
			}

			wstringstream ss2;
			pw = (wchar_t*)sqlite3_column_text16(pStmt, 0);
			ss2 << pw;
			for (int ii = 0; ii < ncol; ++ii)
			{
				int ntype = sqlite3_column_type(pStmt, ii);
				const wchar_t* p = (const wchar_t *)sqlite3_column_text16(pStmt, ii);
				int nb = sqlite3_column_bytes16(pStmt, ii);
				ss2 << L"|";
				ss2 << p;
			}

			wstring ww = ss2.str();
			const wchar_t* ppp = ww.c_str();
			ss2 << endl;
			wcout << ss2.str();
		}
		if (nRet == SQLITE_ERROR)
		{
			throw -2;
		}

	}
	catch (string e)
	{
		stringstream ss;
		ss << "error: " << e << endl;
		cout << ss.str();
	}
	catch (int e)
	{
		stringstream ss;
		ss << "error "<< e << " : " << sqlite3_errmsg(pdb) << endl;
		cout << ss.str();
	}

	if (pStmt)
	{
		nRet = sqlite3_finalize(pStmt);
		pStmt = NULL;
	}

	nRet = sqlite3_close(pdb);
	pdb = NULL;

}