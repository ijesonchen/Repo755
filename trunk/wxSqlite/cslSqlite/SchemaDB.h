#pragma once
#include <string>
#include <deque>

class SchemaDB
{
public:
	class Column
	{
	public:
		int cid;
		std::string name;
		std::string type;
		bool notnull;
		std::string dflt_value;
		bool pk;

		Column(): cid(0), notnull(false), pk(false) {};
	};

	class Table
	{
	public:
		std::string name;
		std::deque<Column> cols;

		void print(void);
	protected:
		void output(const int n);
		void output(const std::string& str);
		void output(const bool b);
	};

public:
	std::deque<Table> tables;
	std::deque<std::string> types;

public:
	void print(void);
};
