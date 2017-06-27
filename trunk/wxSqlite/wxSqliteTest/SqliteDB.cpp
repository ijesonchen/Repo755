#include "SqliteDB.h"
#include "..\Sqlite3008006\sqlite3.h"
#include <deque>
#include <sstream>

using namespace std;

SqliteDB::SqliteDB(void)
: m_pdb(NULL)
{
}

SqliteDB::~SqliteDB(void)
{
}

int SqliteDB::Open(const std::string& strDBFile)
{
	// reset
	Close();

	// open file
	sqlite3* pdb = NULL;
	char* szMsg = NULL;

	int nRet = 0;

	nRet = sqlite3_open(strDBFile.c_str(), &pdb);
	if (nRet)
	{
		stringstream ss(m_strMsg);
		ss << "open file error: %s" << strDBFile << endl;
		m_strMsg = ss.str();
		sqlite3_close(pdb);
		return nRet;
	}
	else
	{
		m_pdb = pdb;
		m_strDBFile = strDBFile;
	}

	return nRet;
}

void SqliteDB::Close(void)
{
	if (m_pdb)
	{
		sqlite3_close(m_pdb);
		m_pdb = NULL;
	}
	m_strDBFile.clear();
	m_strMsg.clear();
	m_dqTable.clear();
}

int SqliteDB::LoadSchema(void)
{
	const int E_LOAD_OPEN = -1;
	const int E_LOAD_TABLE = -2;

	if (!IsOpen())
	{
		return E_LOAD_OPEN;
	}

	string msg;
	try
	{
		int nRet = 0;
		char* szerrmsg = NULL;

		deque<string> dqStrings;
		dqStrings.clear();
		char* szSqlSelTable = "select distinct name from sqlite_master where type = 'table'";
		nRet = sqlite3_exec(m_pdb, szSqlSelTable, &SqliteDB::cbSelStrings, &dqStrings, &szerrmsg);
		if (nRet != SQLITE_OK)
		{
			msg = "szSqlSelTable error.";
			throw (E_LOAD_TABLE);
		}
		m_dqTable.swap(dqStrings);
	}
	catch (int e)
	{
		stringstream ss(m_strMsg);
		ss << "load error: " << e << ": " << msg << endl;
		m_strMsg = ss.str();
		return e;
	}

	return 0;
}

int SqliteDB::cbSelStrings(void* parg, int ncol, char** pvals, char** pnames)
{
	std::deque<std::string>* pstrs = (std::deque<std::string>*)parg;
	for (int i = 0; i < ncol; ++i)
	{
		if (strlen(pvals[i]))
		{
			pstrs->push_back(pvals[i]);
		}
	}
	return 0;
}