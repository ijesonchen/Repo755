#include "stdafx.h"
#include <memory>
#include <iostream>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/*
结论：
1. shared_ptr传参可以使用 by const ref
2. shared_prt使用shared_prt构造时，会继承自定义的删除类
3. vs2008, 2013下
   对内置类型delete, delete[]没有区别，也不会导致内存泄露
   对用户类型，new[]对应的delete，
   debug版会报assert错误，
   release版下，vs2008不会有提示，但会有内存泄露，vs2013会抛异常
4. 指针需要外部管理，但内存在 申请函数 内申请内存时，使用unique_ptr
   申请函数使用 by ref 的unique_ptr，使用reset函数或swap函数修改入参管理的指针
   但是 自定义删除类无法更改。
   如果 申请函数 申请的是数组内存，设计到的unique_ptr定义都需要指明所使用的删除函数
   unique_ptr<SmartWidget, decltype(del)> up2(new SmartWidget[nCnt], del);
   或者
   unique_ptr<SmartWidget, default_delete<SmartWidget[]>> up(new SmartWidget[nCnt]);
5. 在4的情况下，如果需要修改自定义删除类，可以使用 shared_ptr
6. 自定义删除类：std::default_delete 和 自定义 两种
   unique_ptr
   声明时如果不指定，则默认为std::default_delete<T>，并且无法更改。
   如果是管理 数组 ， 或者 使用自定义删除类需要声明中指明，
   构造参数如果不指定，默认根据声明参数生成一个。一下三种都合法
   auto del = [](SmartWidget* p){delete[] p; };
   unique_ptr<SmartWidget, default_delete<SmartWidget[]>> up（new new SmartWidget[nCnt]);
   unique_ptr<SmartWidget, decltype(del)> up2(new SmartWidget[nCnt]);
   unique_ptr<SmartWidget, decltype(del)> up2(new SmartWidget[nCnt], del);

   shared_ptr
   声明中可以不指定，可以自动根据构造参数推断，
   可以修改,swap函数同时交换二者管理的指针和删除类
   

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
// 	delete pi;
// 	delete new int;
//	SmartWidget* pw = new SmartWidget[nCnt];
//	delete pw;
// 	shared_ptr<SmartWidget> spW(pw/*, [](SmartWidget* p){delete[] p; }*/);
// 
// 	SmartCntr sc(spW);

}