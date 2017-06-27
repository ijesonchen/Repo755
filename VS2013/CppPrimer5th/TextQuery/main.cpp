#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <memory>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "QueryFunction.h"

using namespace std;

void main(void)
{	
	const string filename = "text.txt";
	ifstream ifs;

	ifs.open(filename, ios_base::in);
	if (!ifs)
	{
		cout << "open file error " << filename << endl;
		return;
	}

	auto p = [&filename](int n){
		stringstream ss;
		ss << filename << n;
		return ss.str();
	};
	runQueries(ifs);
}