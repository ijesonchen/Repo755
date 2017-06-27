#ifndef TextQuery_h__
#define TextQuery_h__

#include <vector>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <fstream>
#include <set>

#include "QueryResult.h"

class TextQuery
{
public:
	// read is to spLines & m_mapWordLines
	TextQuery(std::ifstream& ifs);
	~TextQuery();

private:
	using SpLineSet = std::shared_ptr < std::set < unsigned > >;

	std::shared_ptr<std::vector<std::string>> spLines;
	std::unordered_map<std::string, SpLineSet> mapWordLines;
public:
	QueryResult query(const std::string& word);
};

#endif // TextQuery_h__
