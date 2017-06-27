#pragma once
#include <string>
#include <deque>

struct sqlite3;

class TBook
{
public:
	static const int COLUMN = 4;
	class Column
	{
	public:
		int id;
		std::string book;
		std::string author;
		std::string remark;

		Column(): id(0) {};
	};

public:
	std::string name;
	std::deque<Column> cols;

public:
	TBook(sqlite3* pdb, const char* szname);
	~TBook(void);

	int Load(void);

protected:
	sqlite3* m_pdb;
};
