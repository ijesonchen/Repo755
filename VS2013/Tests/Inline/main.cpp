#include "Widgets.h"
#include "WidgetInline.h"
#include "WidgetTmpl.h"
#include <string>
#include <iostream>
#include <memory>
/*
���ۣ�
Ĭ������£�VS2013���Զ��ж�����������inline�ؼ����Ƿ���ڣ��������ں��������Ա������
MSDNע���Ż�ѡ���Դ���һ��Ӱ�죬���� ����������չ /Obx �� �Ż����� /Ox
/Obx
0
Disables inline expansion, which is on by default.

1
Expands only functions marked as inline, __inline, __forceinline or __inline or, in a C++ member function, defined within a class declaration.

2
Expands functions marked as inline or __inline and any other function that the compiler chooses (expansion occurs at the compiler's discretion, often referred to as auto-inlining).

/Ob2 is in effect when /O1, /O2 (Minimize Size, Maximize Speed) or /Ox (Full Optimization) is used.

This option requires that you enable optimizations using /O1, /O2, /Ox, or /Og.

3. ���һ������ҪΪ�����࣬���Խ���������������Ϊ���麯�����������塣

*/

using namespace std;

class Base
{
public:
	Base(int* _p) : p(_p) {};
	virtual ~Base()  = 0 {};
protected:
private:
	int* p;
};

class Drv : public Base
{
public:
	int x = 'c';

	Drv() 
		: Base(&x)
	{}
protected:
private:
};

class XX
{
public:
	operator std::string () const
	{
		return "asd";
	}
};

// std::ostream& operator << (std::ostream& os, const XX& x)
// {
// 	return os << x;
// }


int main(void)
{
	std::shared_ptr<char> sp;
	if (!sp)
	{
		cout << 1;
	}

	sp.reset(new char[100]);

	if (!sp)
	{
		cout << 2;
	}

	Drv drv;
	drv.x = 3;


	Widgets w;
	int x = 0;
	x = w.GetA();
	x = w.GetB(); 
	x = w.GetC();
	x = GetD();
	x = GetE();
	x = GetF();

	WidgetInline wi;
	x = wi.GetA();
	x = wi.GetB();

	WidgetImpl wimpl;
	wimpl.Set(23.3);
	x = wimpl.Get();

	XX a;
	std::string b = a;
//	std::cout << a;
	return 0;
}