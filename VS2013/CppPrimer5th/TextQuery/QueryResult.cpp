#include "QueryResult.h"


QueryResult::QueryResult(std::string _word,
	std::shared_ptr<std::set<unsigned>> _spLineno,
	std::shared_ptr<std::vector<std::string>> _spLines)
	: word(_word)
	, spLinenos(_spLineno)
	, spLines(_spLines)
{
}


QueryResult::~QueryResult()
{
}
