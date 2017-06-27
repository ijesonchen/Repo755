#include "TableInfo.h"
#include "..\Sqlite3008006\sqlite3.h"
#include <sstream>
#include <iostream>

#define CONSOLE_OUTPUT

using namespace std;

inline const char* Val(const char* pval)
{
	return pval ? pval : "null";
}

inline const char* Val(const unsigned char* pval)
{
	const char* p = (const char*)pval;
	return p ? p : "null";
}

TableInfo::TableInfo(string n)
: name(n)
{
}

bool TableInfo::GetTableInfo(sqlite3* pdb)
{
	if (!name.length() || !pdb)
	{
		return false;
	}
	int nRet = 0;
	sqlite3_stmt* pStmt = NULL;

	try
	{
		stringstream ss3;
		ss3 << "PRAGMA table_info(" << name << ")" << endl;
		string str3 = ss3.str();
		const char* zsql = str3.c_str();

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
				cout<< sqlite3_column_name(pStmt, ii) << " ";
				int nType = sqlite3_column_type(pStmt, ii);

				/*
				int sqlite3_column_type(sqlite3_stmt*, int iCol);

				int sqlite3_column_int(sqlite3_stmt*, int iCol);
				double sqlite3_column_double(sqlite3_stmt*, int iCol);

				int sqlite3_column_bytes(sqlite3_stmt*, int iCol);
				const unsigned char *sqlite3_column_text(sqlite3_stmt*, int iCol);

				int sqlite3_column_bytes(sqlite3_stmt*, int iCol);
				const void *sqlite3_column_blob(sqlite3_stmt*, int iCol);
				*/

				switch (nType)
				{
				case SQLITE_INTEGER:
					cout << "SQLITE_INTEGER" <<endl;
					break;
				case SQLITE_FLOAT:
					cout << "SQLITE_FLOAT" <<endl;
					break;
				case SQLITE3_TEXT:
					cout << "SQLITE3_TEXT" <<endl;
					break;
				case SQLITE_BLOB:
					cout << "SQLITE_BLOB" <<endl;
					break;
				case SQLITE_NULL:
					cout << "SQLITE_NULL" <<endl;
					break;
				default:
					cout << "default" <<endl;
					break;
				}
				ss2 << "|" << Val(sqlite3_column_text(pStmt, ii));
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
		msg = ss.str();

#ifdef CONSOLE_OUTPUT
		cout << msg << endl;
#endif // CONSOLE_OUTPUT

	}
	catch (int e)
	{
		stringstream ss;
		ss << "error "<< e << " : " << sqlite3_errmsg(pdb) << endl;
		msg = ss.str();

#ifdef CONSOLE_OUTPUT
		cout << msg << endl;
#endif // CONSOLE_OUTPUT

	}

	if (pStmt)
	{
		nRet = sqlite3_finalize(pStmt);
		pStmt = NULL;
	}

	return true;

}


int TableInfo::cbPragmaTable(void* parg, int ncol, char** pvals, char** pnames)
{
	char* szNames[] =
	{
		"cid","name","type","notnull","dflt_value","pk"
	};

	if (ncol != sizeof(szNames) / sizeof(char*))
	{ 
		return 1;
	}

	for (int i = 0; i < ncol; ++i)
	{
		if (strcmp(szNames[i], pnames[i]) != 0)
		{
			return 2;
		}
	}

	// cid|name|type|notnull|dflt_value|pk
	ColumnInfo table;
	std::stringstream ss;
	ss << pvals[0] ? pvals[0] : "-1";
	ss >> table.cid;
	table.name = pvals[1] ? pvals[1] : "";
	string type = pvals[2] ? pvals[2] : "";

	if (type == "INTEGER")
	{
		table.type = SQLITE_INTEGER;
	}
	else if (type == "TEXT")
	{
		table.type = SQLITE3_TEXT;
	}
	else if (type == "REAL")
	{
		table.type = SQLITE_FLOAT;
	}
	else if (type == "BLOB")
	{
		table.type = SQLITE_BLOB;
	}
	else
	{
		table.type = SQLITE_NULL;
	}

	table.notnull = pvals[3] ? true : false;
	table.dflt = pvals[4] ? pvals[4] : "";
	table.pk = pvals[5] ? true : false;

	std::deque<ColumnInfo>* pcols = (std::deque<ColumnInfo>*) parg;
	pcols->push_back(table);

	return 0;
}

// 
// {
// 	int nRet = 0;
// 	sqlite3_stmt* pStmt = NULL;
// 	try
// 	{
// 		stringstream ss3;
// 		ss3 << "PRAGMA table_info(" << name << ")" << endl;
// 		string str3 = ss3.str();
// 		const char* zsql = str3.c_str();
// 
// 		nRet = sqlite3_prepare_v2(pdb, zsql, strlen(zsql) + 1, &pStmt, NULL);
// 		if (nRet != SQLITE_OK || !pStmt)
// 		{
// 			stringstream ss;
// 			ss << "sqlite3_prepare_v2 error: " << nRet << endl;
// 			throw ss.str();
// 		}
// 
// 		bool bHeaderOutput = true;
// 		int ncol = sqlite3_column_count(pStmt);
// 		if (ncol <= 0)
// 		{
// 			stringstream ss;
// 			ss << "sqlite3_column_count returns no data: " << ncol << endl;
// 			throw ss.str();
// 		}
// 
// 		while ((nRet = sqlite3_step(pStmt)) == SQLITE_ROW)
// 		{
// 			if (bHeaderOutput)
// 			{
// 				stringstream ss;
// 				ss << sqlite3_column_name(pStmt, 0);
// 				for (int ii = 1; ii < ncol; ++ii)
// 				{
// 					ss << "|" << sqlite3_column_name(pStmt, ii);
// 				}
// 				ss << endl;
// 				cout << ss.str();
// 				bHeaderOutput = false;
// 			}
// 
// 			stringstream ss2;
// 			ss2 << Val(sqlite3_column_text(pStmt, 0));
// 			for (int ii = 0; ii < ncol; ++ii)
// 			{
// 				cout<< sqlite3_column_name(pStmt, ii) << " ";
// 				int nType = sqlite3_column_type(pStmt, ii);
// 
// 				/*
// 				int sqlite3_column_type(sqlite3_stmt*, int iCol);
// 
// 				int sqlite3_column_int(sqlite3_stmt*, int iCol);
// 				double sqlite3_column_double(sqlite3_stmt*, int iCol);
// 
// 				int sqlite3_column_bytes(sqlite3_stmt*, int iCol);
// 				const unsigned char *sqlite3_column_text(sqlite3_stmt*, int iCol);
// 
// 				int sqlite3_column_bytes(sqlite3_stmt*, int iCol);
// 				const void *sqlite3_column_blob(sqlite3_stmt*, int iCol);
// 				*/
// 
// 				switch (nType)
// 				{
// 				case SQLITE_INTEGER:
// 					cout << "SQLITE_INTEGER" <<endl;
// 					break;
// 				case SQLITE_FLOAT:
// 					cout << "SQLITE_FLOAT" <<endl;
// 					break;
// 				case SQLITE3_TEXT:
// 					cout << "SQLITE3_TEXT" <<endl;
// 					break;
// 				case SQLITE_BLOB:
// 					cout << "SQLITE_BLOB" <<endl;
// 					break;
// 				case SQLITE_NULL:
// 					cout << "SQLITE_NULL" <<endl;
// 					break;
// 				default:
// 					cout << "default" <<endl;
// 					break;
// 				}
// 				ss2 << "|" << Val(sqlite3_column_text(pStmt, ii));
// 			}
// 			ss2 << endl;
// 			cout << ss2.str();
// 		}
// 		if (nRet == SQLITE_ERROR)
// 		{
// 			throw -2;
// 		}
// 
// 	}
// 	catch (string e)
// 	{
// 		stringstream ss;
// 		ss << "error: " << e << endl;
// 		msg = ss.str();
// 
// #ifdef CONSOLE_OUTPUT
// 		cout << msg << endl;
// #endif // CONSOLE_OUTPUT
// 
// 	}
// 	catch (int e)
// 	{
// 		stringstream ss;
// 		ss << "error "<< e << " : " << sqlite3_errmsg(pdb) << endl;
// 		msg = ss.str();
// 
// #ifdef CONSOLE_OUTPUT
// 		cout << msg << endl;
// #endif // CONSOLE_OUTPUT
// 
// 	}
// 
// 	if (pStmt)
// 	{
// 		nRet = sqlite3_finalize(pStmt);
// 		pStmt = NULL;
// 	}
// 
// 	return true;
// 
// }