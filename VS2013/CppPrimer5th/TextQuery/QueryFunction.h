#ifndef QueryFunction_h__
#define QueryFunction_h__

#include <iostream>

class QueryResult;
std::ostream& print(std::ostream& os, const QueryResult& qr);

void runQueries(std::ifstream& ifs);

#endif // QueryFunction_h__
