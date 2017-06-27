#pragma once
#include <string>
#include <deque>

struct sqlite3;

class TTable
{

public:
	typedef std::deque<std::string> DQString;

public:
	TTable(void);
	~TTable(void);

public:
	// info of each column: cid|name|type|notnull|dflt_value|pk
	std::deque<DQString> colinfo; 
	// each column's name only 
	DQString colname;
	// records:
	std::deque<DQString> records; 	

	void Reset(void);
	int Load(sqlite3* pdb, const char* strTable);
	void Print(void);
	std::string Msg(void) const { return msg;};

protected:
	// should be: cid|name|type|notnull|dflt_value|pk
	DQString coltitle;

	// error message
	std::string msg;
	int LoadHdr(sqlite3* pdb, const char* strTable);
	int LoadData(sqlite3* pdb, const char* strTable);
	void PrintOneRecord(DQString& record);

	static int cbTableHdr(void* parg, int ncol, char** pvals, char** pnames);
	static int cbTableData(void* parg, int ncol, char** pvals, char** pnames);
};