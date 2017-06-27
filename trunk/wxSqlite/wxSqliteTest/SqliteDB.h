#ifndef SqliteDB_h__
#define SqliteDB_h__

#pragma once

#include <string>
#include <deque>

struct sqlite3;

class SqliteDB
{
public:
	SqliteDB(void);
	~SqliteDB(void);
private:
	sqlite3* m_pdb;
	std::string m_strDBFile;
	std::string m_strMsg;
	std::deque<std::string> m_dqTable;
public:
	void Close(void);
	int Open(const std::string& dbfile);
	int LoadSchema(void);

	bool IsOpen(void) const
	{
		return m_pdb ? true : false;
	};

	sqlite3* GetDBPtr(void) const
	{
		return m_pdb;
	}

	std::string GetDBFile(void) const
	{
		return m_strDBFile;
	};

	std::string Msg(void) const
	{
		return m_strMsg;
	};
	
	std::deque<std::string> GetTables(void) const
	{
		return m_dqTable;
	}

private:
	static int cbSelStrings(void* parg, int ncol, char** pvals, char** pnames);
};
#endif // SqliteDB_h__
