#pragma once
#include <string>
#include <deque>

struct sqlite3;

class TTable
{

public:
	typedef std::deque<std::string> DQString;

public:
	TTable(sqlite3* pdb, const char* szname);
	~TTable(void);

public:
	std::deque<DQString> colinfo;
	std::deque<DQString> records;

	void Reset(sqlite3* pdb = NULL, const char* szname = NULL);
	int Load(void);
	void Print(void);
	std::string Msg(void) const { return msg;};
	std::string Name(void) const { return name; };

protected:
	std::string name;
	sqlite3* m_pdb;
	DQString coltitle;
	DQString colname;
	std::string msg;
	int LoadHdr(void);
	int LoadData(void);
	void PrintOneRecord(DQString& record);

	static int cbTableHdr(void* parg, int ncol, char** pvals, char** pnames);
	static int cbTableData(void* parg, int ncol, char** pvals, char** pnames);
};