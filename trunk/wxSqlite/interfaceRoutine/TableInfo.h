#pragma once
#include <string>
#include <deque>

struct sqlite3;

class ColumnInfo
{
public:
	int cid;
	std::string name;
	int type;
	bool notnull;
	std::string dflt; // default value
	bool pk;
};

class TableInfo
{
public:
	TableInfo(std::string n);
	bool GetTableInfo(sqlite3* pdb);

	std::deque<ColumnInfo> cols;

protected:
	std::string name;
	std::string msg;

	static int cbPragmaTable(void* parg, int ncol, char** pvals, char** pnames);
};
