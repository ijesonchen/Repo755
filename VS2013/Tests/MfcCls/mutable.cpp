
//////////////////////////////////////////////////////////////////////////
// mutable const
#include "stdafx.h"

void ChangeMutableConst(const char** p)
{
	*p = 0;
}

class Widgets
{
public:
	Widgets(const char* pp) : p(pp){};
	void Change(void) const
	{
		ChangeMutableConst(&p);
		++x;
	}
protected:
private:
	mutable const char* p = nullptr;
	mutable int x = 0;
};

void TestMutableConst(void)
{
	char p[10] = { 0 };
	const Widgets w(p);
	w.Change();
}
