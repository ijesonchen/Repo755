#include <iostream>
using namespace std;

// Çón!

int Factorial(int n)
{
	if (n == 1)
	{
		return 1;
	}
	return n * Factorial(n - 1);
}

void eg0210(void)
{
	int x = 5;
	cout << Factorial(x) << endl;
}