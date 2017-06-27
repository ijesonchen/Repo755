#include "TextQuery.h"
#include <sstream>
#include <algorithm>

using namespace std;

// read is to spLines & m_mapWordLines
TextQuery::TextQuery(std::ifstream& ifs)
	: spLines(new vector<string>)
{
	string line;
	while (getline(ifs, line))
	{
		spLines->push_back(line);
		string word;
		string lowLine;
		transform(line.begin(), line.end(), back_inserter(lowLine),
			[](const char ch){ return isalnum(ch) ? tolower(ch) : ' '; });
		istringstream iss(lowLine);
		while (iss >> word)
		{
			auto& spLineno = mapWordLines[word];
			if (!spLineno)
			{
				spLineno.reset(new set<unsigned>);
			}
			spLineno->insert(spLines->size() - 1);
		}
	}
}

TextQuery::~TextQuery()
{
}

QueryResult TextQuery::query(const std::string& word)
{
	string lowWord(word);
	transform(word.cbegin(), word.cend(), back_inserter(lowWord), ::towlower);
	auto& spLineno = mapWordLines[lowWord];
	if (!spLineno)
	{
		spLineno.reset(new set<unsigned>);
	}
	return QueryResult(word, spLineno, spLines);
}
