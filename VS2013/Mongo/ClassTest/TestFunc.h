#ifndef TestFunc_h__
#define TestFunc_h__

#include <string>
#include <vector>
#include <bson.h>
#include <mongoc.h>


void TestMongo(void);

void TestIndex(void);

void TestBuildDoc(void);

void TestClient(void);

void TestConn(void);

void TestStudent(void);

void TestLoadBin(void);

void TestInsert(void);

void TestU8(void);
std::string toU8(const std::wstring& strUnicode);
std::wstring fromU8(const std::string& strUtf8);


void TestAnsi(void);

#endif // TestFunc_h__
