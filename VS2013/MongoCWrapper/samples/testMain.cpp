#include "mfcafx.h"

#include "TestFunc.h"

#include "mfcnew.h"

#ifdef _MSC_VER
	#pragma comment(lib, "bson-1.0.lib")
	#pragma comment(lib, "mongoc-1.0.lib")
#endif // _MSC_VER


int main(void)
{
	TestMongo();

	return 0;
}