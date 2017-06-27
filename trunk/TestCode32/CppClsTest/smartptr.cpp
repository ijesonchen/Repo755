#include <memory>
#include <iostream>

#

/*
结论：
1. shared_ptr传参可以使用 by const ref
2. shared_prt使用shared_prt构造时，会继承自定义的删除类
3. 

*/

using namespace std;
using namespace std::tr1;

class SmartWidget
{
public:
	SmartWidget(){ cout << "ctor SmartWidget" << endl; };
	~SmartWidget(){ cout << "dtor SmartWidget" << endl; };
};
class SmartCntr
{
public:
	SmartCntr(const shared_ptr<SmartWidget>& sp)
		: spWidget(sp)
	{ cout << "ctor SmartCntr" << endl; };

	~SmartCntr(){ cout << "dtor SmartCntr" << endl; };
private:
	shared_ptr<SmartWidget> spWidget;
};


void TestSmartPtr(void)
{
	const int nCnt = 10;
	int* pi = new int[10];
	delete pi;
	SmartWidget* pw = new SmartWidget[nCnt];
//	delete pw;
	shared_ptr<SmartWidget> spW(pw/*, [](SmartWidget* p){delete[] p; }*/);

	SmartCntr sc(spW);

}