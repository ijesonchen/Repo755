#include <deque>
#include <sstream>
#include "SchemaDB.h"
//////////////////////////////////////////////////////////////////////////
// functions

int cbSelStrings(void* parg, int ncol, char** pvals, char** pnames)
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

int cbPragmaTable(void* parg, int ncol, char** pvals, char** pnames)
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
	SchemaDB::Column table;
	std::stringstream ss;
	ss << pvals[0] ? pvals[0] : "-1";
	ss >> table.cid;
	table.name = pvals[1] ? pvals[1] : "";
	table.type = pvals[2] ? pvals[2] : "";
	table.notnull = pvals[3] ? true : false;
	table.dflt_value = pvals[4] ? pvals[4] : "";
	table.pk = pvals[5] ? true : false;

	std::deque<SchemaDB::Column>* ptables = (std::deque<SchemaDB::Column>*)parg;
	ptables->push_back(table);

	return 0;
}