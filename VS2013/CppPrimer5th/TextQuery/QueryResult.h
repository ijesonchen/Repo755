#ifndef QueryResult_h__
#define QueryResult_h__

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <set>

class QueryResult
{
	friend std::ostream& print(std::ostream&, const QueryResult&);
public:
	QueryResult(std::string _word,
		std::shared_ptr<std::set<unsigned>> _spLineno,
		std::shared_ptr<std::vector<std::string>> _spLines);

	~QueryResult();
private:
	std::string word;
	std::shared_ptr<std::set<unsigned>> spLinenos;
	std::shared_ptr<std::vector<std::string>> spLines;
};

#endif // QueryResult_h__
