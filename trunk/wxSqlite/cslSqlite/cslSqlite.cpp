#include "..\Sqlite3008006\sqlite3.h"
#include <deque>
#include <iostream>
#include <sstream>
#include "SchemaDB.h"
#include "SqliteCB.h"
#include "TBook.h"
#include "Table.h"


using namespace std;

int main(void)
{
	SchemaDB schemaDb;

	sqlite3* pdb = NULL;
	char* szMsg = NULL;
	char* szFileName = "D:\\data\\sqlite\\test1.db";

	int nRet = 0;

	nRet = sqlite3_open(szFileName, &pdb);
	if (nRet)
	{
		printf("open file %s error.\n", szFileName);
		sqlite3_close(pdb);
		return -1;
	}

	// read table-name

	string msg;
	try
	{
		char* szerrmsg = NULL;

		// types
		char* szSqlSelType = "select DISTINCT type from sqlite_master";	
		nRet = sqlite3_exec(pdb, szSqlSelType, cbSelStrings, &schemaDb.types, &szerrmsg);
		if (nRet != SQLITE_OK)
		{
			msg = "szSqlSelType error: ";
			msg += szerrmsg;
			throw (msg);
		}

		deque<string> dqStrings;

		// tables
		dqStrings.clear();
		char* szSqlSelTable = "select distinct name from sqlite_master where type = 'table'";
		nRet = sqlite3_exec(pdb, szSqlSelTable, cbSelStrings, &dqStrings, &szerrmsg);
		if (nRet != SQLITE_OK)
		{
			msg = "szSqlSelTable error.";
			throw (msg);
		}

		for (size_t i = 0, n = dqStrings.size();
			i < n; ++i)
		{
			SchemaDB::Table table;
			table.name = dqStrings[i];
			std::stringstream ss;
			ss << "PRAGMA table_info(" << table.name << ")" << endl;
			std::string pragmatableinfo = ss.str();
			nRet = sqlite3_exec(pdb, pragmatableinfo.c_str(), cbPragmaTable, &table.cols, &szerrmsg);
			if (nRet != SQLITE_OK)
			{
				msg = "PRAGMA table_info error.";
				throw (msg);
			}
			schemaDb.tables.push_back(table);
		}

		schemaDb.print();

		std::deque<TTable> tables;
		for (size_t ii = 0, ni = schemaDb.tables.size();
			ii < ni; ++ii)
		{
			TTable table(pdb, schemaDb.tables[ii].name.c_str());
			if (table.Load())
			{
				cout << "load error: " << table.Name() <<endl;
				cout << "msg: " << table.Msg() << endl;
			}
			else
			{
				tables.push_back(table);
			}
		}

		cout << "content of tables" <<endl;

		for (size_t ii = 0, ni = tables.size();
			ii < ni; ++ii)
		{
			tables[ii].Print();
			cout<<endl;
		}

		//	sqlite3_exec(pdb, "szSqlSelType");

		// read index

		// read key

		// read table-content
	}
	catch (string e)
	{
		printf("error: %s\n", e);
		return 1;
	}
	catch (int code)
	{
		printf("error: %d\n", code);
		return code;
	}
	catch (...)
	{
		printf("unknown error.\n");
		return 2;
	}

	return 0;
}