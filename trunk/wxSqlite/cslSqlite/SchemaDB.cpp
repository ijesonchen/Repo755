#include "SchemaDB.h"
#include <iostream>
#include <sstream>

//////////////////////////////////////////////////////////////////////////
// SchemaDB

void SchemaDB::Table::print(void)
{
	std::cout << "TABLE INFO: " << name << std::endl;
	std::cout << "cid|name|type|notnull|dflt_value|pk"<< std::endl;
	for (size_t ii = 0, ni = cols.size(); ii < ni; ++ii)
	{
		Column& col = cols[ii];
		output(col.cid);
		output(col.name);
		output(col.type);
		output(col.notnull);
		output(col.dflt_value);
		output(col.pk);
		std::cout << std::endl;
	}
}

void SchemaDB::Table::output(const int n)
{
	std::stringstream ss;
	ss << n;
	std::cout << ss.str() << " |";
}

void SchemaDB::Table::output(const std::string& str)
{
	std::string stro = str.length() ? str : "NULL";
	std::cout << stro << " |";
}
void SchemaDB::Table::output(const bool b)
{
	std::string stro =  b ? "T" : "F";
	std::cout << stro << " |";
}

void SchemaDB::print(void)
{
	std::cout << "TYPES:";
	for (size_t i = 0, n = types.size(); i < n; ++i)
	{
		std::cout << " " << types[i];
	}
	std::cout << std::endl << std::endl;

	std::cout << "TABLES: ";
	for (size_t i = 0, n = tables.size(); i < n; ++i)
	{
		std::cout << " " << tables[i].name;
	}
	std::cout << std::endl << std::endl;


	for (size_t i = 0, ni = tables.size(); i < ni; ++i)
	{
		tables[i].print();
		std::cout << std::endl;
	}
}