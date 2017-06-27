#include <cstdlib>
#include <iostream>

void maptest(void);
void ThreadTest(void);

using namespace std;

void main(void)
{
	ThreadTest();
	cout << "press enter to exit." << endl;
	getchar();

	// this code is used to check if memory leak detect is activated.
	char* p = new char[1234];
}