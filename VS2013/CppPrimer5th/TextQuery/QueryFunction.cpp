#include "QueryFunction.h"
#include "QueryResult.h"
#include "TextQuery.h"

using namespace std;

/************************************************************************/
/*
* word occurs n times
* (line xx) xxxxx
*/
/************************************************************************/
std::ostream& print(std::ostream& os, const QueryResult& qr)
{
	os << qr.word << " occurs " << qr.spLinenos->size() << " time";
	if (qr.spLinenos->size())
	{
		os << "s";
	}
	os << endl;
	auto& vtLineNo = *qr.spLinenos;
	for (auto beg = vtLineNo.cbegin(); beg != vtLineNo.cend(); ++beg)
	{
		os << "(line " << *beg + 1 << ") " << (*qr.spLines)[*beg] << endl;
	}
	os << endl;
	return os;
}

void runQueries(std::ifstream& ifs)
{
	TextQuery text(ifs);
	string word;
	while (cin >> word)
	{
		if (word == "q")
		{
			return;
		}
		QueryResult result = text.query(word);
		print(cout, result);
	}

	cout << "run Queries finished." << endl;
}