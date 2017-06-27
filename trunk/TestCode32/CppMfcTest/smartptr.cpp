#include "stdafx.h"
#include <memory>
#include <iostream>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/*
���ۣ�
1. shared_ptr���ο���ʹ�� by const ref
2. shared_prtʹ��shared_prt����ʱ����̳��Զ����ɾ����
3. vs2008, 2013��
   ����������delete, delete[]û������Ҳ���ᵼ���ڴ�й¶
   ���û����ͣ�new[]��Ӧ��delete��
   debug��ᱨassert����
   release���£�vs2008��������ʾ���������ڴ�й¶��vs2013�����쳣
4. ָ����Ҫ�ⲿ�������ڴ��� ���뺯�� �������ڴ�ʱ��ʹ��unique_ptr
   ���뺯��ʹ�� by ref ��unique_ptr��ʹ��reset������swap�����޸���ι����ָ��
   ���� �Զ���ɾ�����޷����ġ�
   ��� ���뺯�� ������������ڴ棬��Ƶ���unique_ptr���嶼��Ҫָ����ʹ�õ�ɾ������
   unique_ptr<SmartWidget, decltype(del)> up2(new SmartWidget[nCnt], del);
   ����
   unique_ptr<SmartWidget, default_delete<SmartWidget[]>> up(new SmartWidget[nCnt]);
5. ��4������£������Ҫ�޸��Զ���ɾ���࣬����ʹ�� shared_ptr
6. �Զ���ɾ���ࣺstd::default_delete �� �Զ��� ����
   unique_ptr
   ����ʱ�����ָ������Ĭ��Ϊstd::default_delete<T>�������޷����ġ�
   ����ǹ��� ���� �� ���� ʹ���Զ���ɾ������Ҫ������ָ����
   ������������ָ����Ĭ�ϸ���������������һ����һ�����ֶ��Ϸ�
   auto del = [](SmartWidget* p){delete[] p; };
   unique_ptr<SmartWidget, default_delete<SmartWidget[]>> up��new new SmartWidget[nCnt]);
   unique_ptr<SmartWidget, decltype(del)> up2(new SmartWidget[nCnt]);
   unique_ptr<SmartWidget, decltype(del)> up2(new SmartWidget[nCnt], del);

   shared_ptr
   �����п��Բ�ָ���������Զ����ݹ�������ƶϣ�
   �����޸�,swap����ͬʱ�������߹����ָ���ɾ����
   

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