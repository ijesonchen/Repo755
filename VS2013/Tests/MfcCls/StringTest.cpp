#include "stdafx.h"
#include <string>

using namespace std;

void SafeCopy(void)
{
	string str("ab");

	char p[5];

	int len = sizeof(p);
	_memccpy(p, str.c_str(), 0, len);
	p[len - 1] = 0;

}