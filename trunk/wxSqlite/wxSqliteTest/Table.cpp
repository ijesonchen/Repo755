#include "Table.h"
#include <sstream>
#include "..\Sqlite3008006\sqlite3.h"
#include <iostream>
using namespace std;

int TTable::cbTableHdr(void* parg, int ncol, char** pvals, char** pnames)
{
	// check
	static char* szNames[] =
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

	TTable* ptable = (TTable*)parg;
	if (!ptable->coltitle.size())
	{
		for (int i = 0; i < ncol; ++i)
		{
			ptable->coltitle.push_back(pnames[i]);
		}
	}


	// cid|name|type|notnull|dflt_value|pk
	// proc
	DQString colinfo;
	for (int i = 0; i < ncol; ++i)
	{
		colinfo.push_back(pvals[i] ? pvals[i] : "");
	}
	ptable->colinfo.push_back(colinfo);

	return 0;
}


int TTable::cbTableData(void* parg, int ncol, char** pvals, char** pnames)
{
	TTable::DQString col;
	for (int i = 0; i < ncol; ++i)
	{
		col.push_back(pvals[i] ? pvals[i] : "");
	}
	std::deque<TTable::DQString>* pdata = (std::deque<TTable::DQString>*)parg;
	pdata->push_back(col);

	return 0;
}

TTable::TTable(void)
{
}

TTable::~TTable(void)
{
}

int TTable::LoadHdr(sqlite3* pdb, const char* strTable)
{
	std::stringstream ss;
	ss << "PRAGMA table_info(" << strTable << ")" << std::endl;
	std::string strsql = ss.str();
	char* szerrmsg = NULL;
	int nRet = sqlite3_exec(pdb, strsql.c_str(), &TTable::cbTableHdr, this, &szerrmsg);
	if (nRet != SQLITE_OK)
	{
		ss.str(msg);
		ss << "LoadHdr error: " << szerrmsg << std::endl;
		msg = ss.str();
		return nRet;
	}
	return 0;
}

int TTable::LoadData(sqlite3* pdb, const char* strTable)
{
	std::stringstream ss;
	if (!coltitle.size())
	{
		ss.str(msg);
		ss << "LoadData error: hdrtitle is empty." << std::endl;
		msg = ss.str();
		return -1;
	}
	// check hdr 
	const char* szName = "name";
	int idxName = -1;
	for (size_t ii = 0, ni = coltitle.size();
		ii < ni; ++ii)
	{
		if (strcmp(coltitle[ii].c_str(), szName) == 0)
		{
			// name col found
			idxName = (int)ii;
			break;
		}
	}
	if (idxName == -1)
	{
		ss.str(msg);
		ss << "LoadData error: coltitle name col not found." << std::endl;
		msg = ss.str();
		return -3;
	}

	// check col info
	if (!colinfo.size())
	{
		ss.str(msg);
		ss << "LoadData error: colinfo is empty." << std::endl;
		msg = ss.str();
		return -4;
	}
	for (size_t ii = 0, ni = colinfo.size();
		ii < ni; ++ii)
	{
		DQString& colitem = colinfo[ii];
		if (!colitem[idxName].length())
		{
			ss.str(msg);
			ss << "LoadData error: colinfo col name error." << std::endl;
			msg = ss.str();
			return -5;
		}
		colname.push_back(colitem[idxName]);
	}

	if (colname.size())
	{
		colname.push_front("rowid");
	}

	ss << "select " << colname[0];
	for (size_t ii = 1, ni = colname.size();
		ii < ni; ++ii)
	{
		ss << ", " << colname[ii];
	}
	ss << " from " << strTable << " order by rowid" << std::endl;
	std::string strsql = ss.str();
	char* szerrmsg = NULL;
	int nRet = sqlite3_exec(pdb, strsql.c_str(), cbTableData, &records, &szerrmsg);
	if (nRet != SQLITE_OK)
	{
		ss.str(msg);
		ss << "select from " << strTable << " error: " << szerrmsg << std::endl;
		msg = ss.str();
		return nRet;
	}

	return 0;
}

void TTable::Reset(void)
{
	coltitle.clear();
	colinfo.clear();
	colname.clear();
	msg.clear();
	records.clear();
}

int TTable::Load(sqlite3* pdb, const char* strTable)
{
	if (!pdb)
	{
		return -1;
	}
	if (!strlen(strTable))
	{
		return -2;
	}
	if (LoadHdr(pdb, strTable))
	{
		return -3;
	}
	if (LoadData(pdb, strTable))
	{
		return -4;
	}
	return 0;
}

void TTable::Print(void)
{
	cout << "data of table: " << endl;
	if (!colname.size())
	{
		return;
	}
	cout << colname[0];
	for (int ii = 1, ni = colname.size();
		ii < ni; ++ii)
	{
		cout << " | " << colname[ii];
	}
	cout<<endl;

	size_t nRecord = records.size();

	if (!nRecord)
	{
		cout << "NO DATA" <<endl;
	}
	
	for (size_t ii = 0; ii < nRecord; ++ii)
	{
		PrintOneRecord(records[ii]);
	}	
}

void TTable::PrintOneRecord(DQString& record)
{
	if (record.size())
	{
		cout << record[0];
		for (int ii = 1, ni = record.size();
			ii < ni; ++ii)
		{
			cout << " | " << record[ii];
		}
		cout<<endl;
	}
}