#include "ch1-2.h"

using namespace std;


// 1.2.6 回环变为的检测
bool ex0206f(string s, string t)
{
	if (s.length() != t.length())
	{
		cout << s << " and " << t << " NOT circular rotation" << endl;
		return false;
	}
	string x = s + s;
	if (x.find(t) != string::npos)
	{
		cout << s << " and " << t << " IS circular rotation" << endl;
		return true;
	}
	cout << s << " and " << t << " NOT circular rotation" << endl;
	return false;
}

void ex0206(void)
{
	ex0206f("A", "B");
	ex0206f("A", "A");
	ex0206f("ABC", "A");
	ex0206f("ABC", "BCA");
	ex0206f("ABC", "CBA");
	ex0206f("ACTGACG", "TGACGAC");
}