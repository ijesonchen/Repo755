#include "stdafx.h"
#include "StudentAccess.h"
#include <iostream>

#include "MongocHelp.h"
#include "CommonFunction.h"

using namespace std;
using namespace MongoClib;
using namespace CommonFunction;

StudentAccess::StudentAccess()
{
}


StudentAccess::~StudentAccess()
{
}


bool StudentAccess::QueryLoad(std::deque<Student>& dqObjects, const bson_t* query, int nMax /*= 0*/)
{
	AutoPoolColl coll(m_collname);
	if (!coll)
	{
		cout << "Can not create collection. Too many client thread?" << endl;
		return false;
	}
	if (!query)
	{
		cout << "query parameter is null" << endl;
		return false;
	}

	AutoCursor cursor(mongoc_collection_find(coll, MONGOC_QUERY_NONE, 0, nMax, 0, query, nullptr, nullptr));
	if (!cursor)
	{
		cout << "query failed." << endl;
		return false;
	}

	const bson_t *doc;
	while (mongoc_cursor_next(cursor.Cursor(), &doc)) {
		Student stu;
		ParseBson(stu, doc);
		dqObjects.push_back(stu);
	}

	return true;
}

AutoBson StudentAccess::BuildBson(const Student& student)
{
	AutoBson doc;
	if (!doc)
	{
		return nullptr;
	}
	if (!BSON_APPEND_UTF8(doc, "strName", EncodeUtf8MS(student.strName).c_str()))
	{
		return nullptr;
	}
	if (!BSON_APPEND_UTF8(doc, "strMemo", EncodeUtf8MS(student.strMemo).c_str()))
	{
		return nullptr;
	}
	if (!BSON_APPEND_BOOL(doc, "bValue", student.bValue))
	{
		return nullptr;
	}
	if (!BSON_APPEND_INT32(doc, "nValue", student.nValue))
	{
		return nullptr;
	}
	if (!BSON_APPEND_DOUBLE(doc, "fValue", student.nValue))
	{
		return nullptr;
	}
	vector<string> vt;
	for (auto it : student.vtText)
	{
		vt.push_back(EncodeUtf8MS(it));
	}
	if (!BsonAddArray(doc, "vtText", vt))
	{
		return nullptr;
	}
	return std::move(doc);
}

bson_t* StudentAccess::BuildBson(const CString& name)
{
	AutoBson doc;
	if (!doc)
	{
		return nullptr;
	}
	if (!BSON_APPEND_UTF8(doc, "strName", EncodeUtf8MS(name).c_str()))
	{
		return nullptr;
	}
	return doc.Release();
}

void StudentAccess::ParseBson(Student& student, const bson_t* doc)
{
	if (!doc)
	{
		return;
	}
	student.strName = DecodeUtf8MS(BsonValStr(doc, "strName"));
	student.strMemo = DecodeUtf8MS(BsonValStr(doc, "strMemo"));
	student.bValue = BsonValueBool(doc, "bValue");
	student.nValue = BsonValueInt32(doc, "nValue");
	student.fValue = (float)BsonValueDouble(doc, "fValue");
	BsonValueW(doc, "vtText", back_inserter(student.vtText));
}

bool StudentAccess::Insert(const Student& student)
{
	AutoBson doc(BuildBson(student));
	if (!doc)
	{
		cout << "Insert failed: build doc" << endl;
		return false;
	}

	AutoPoolColl coll(m_collname);
	if (!coll.Insert(doc))
	{
		cout << "Insert failed: " << coll.Error() << endl;
		return false;
	}

	return true;
}

bool StudentAccess::Delete(const CString& name)
{
	AutoBson query(BuildBson(name));
	if (!query)
	{
		cout << "Delete failed: build query" << endl;
		return false;
	}

	AutoPoolColl coll(m_collname);
	if (!coll.Delete(query))
	{
		cout << "Delete failed: " << coll.Error();
		return false;
	}
	return true;
}

bool StudentAccess::Update(const Student& student)
{
	AutoBson query(BuildBson(student.strName));
	if (!query)
	{
		cout << "construct query bson failed" << endl;
		return false;
	}
	AutoBson update;
	if (!update)
	{
		cout << "construct update bson failed" << endl;
		return false;
	}

	AutoBson stu(BuildBson(student));
	if (!stu)
	{
		cout << "construct stu bson failed" << endl;
		return false;
	}
	if (!BSON_APPEND_DOCUMENT(update, "$set", stu))
	{
		cout << "build update bson failed" << endl;
		return false;		
	}

	AutoPoolColl coll(m_collname);
	if (!coll.Update(query, update))
	{
		cout << "Delete failed: " << coll.Error();
		return false;
	}
	return true;
}

bool StudentAccess::UpdateBool(const CString& name, bool b)
{
	AutoBson query(BuildBson(name));
	if (!query)
	{
		cout << "construct query bson failed" << endl;
		return false;
	}
	
	AutoBson update(BCON_NEW("$set", "{", "bValue", BCON_BOOL(b), "}"));
	if (!update)
	{
		cout << "construct update bson failed" << endl;
		return false;
	}

	AutoPoolColl coll(m_collname);
	if (!coll.Update(query, update))
	{
		cout << "Delete failed: " << coll.Error();
		return false;
	}
	return true;
}
bool StudentAccess::ModifyBool(const CString& name, bool b)
{
	AutoBson query(BuildBson(name));
	if (!query)
	{
		cout << "construct query bson failed" << endl;
		return false;
	}

	AutoBson update(BCON_NEW("$set", "{", "bValue", BCON_BOOL(b), "}"));
	if (!update)
	{
		cout << "construct update bson failed" << endl;
		return false;
	}
	AutoPoolColl coll(m_collname);
	if (!coll.Modify(query, update))
	{
		m_err = coll.Error();
		cout << "Delete failed: " << coll.Error();
		return false;
	}
	return true;
}

bool StudentAccess::LoadOne(const CString& name, Student& student)
{
	AutoBson query(BuildBson(name));
	if (!query)
	{
		cout << "construct query bson failed" << endl;
		return false;
	}
	std::deque<Student> dqObjects;
	if (!QueryLoad(dqObjects, query))
	{
		cout << L"QueryLoad failed." << endl;
		return false;
	}
	if (dqObjects.size() <= 0)
	{
		cout << "student not found" << endl;
		return false;
	}

	if (dqObjects.size() > 1)
	{
		cout << "more than one student found" << endl;
	}

	student = dqObjects.front();

	return true;
}

bool StudentAccess::LoadAll(std::deque<Student>& dqStudents)
{
	return QueryLoad(dqStudents);
}

bool StudentAccess::LoadCond(std::deque<Student>& dqStudents, const int n)
{
	AutoBson query;
	if (!query)
	{
		cout << "construct query bson failed" << endl;
		return false;
	}
	if (!BSON_APPEND_INT32(query, "nValue", n))
	{
		cout << "build query bson failed" << endl;
		return false;
	}

	return QueryLoad(dqStudents, query);
}

int  StudentAccess::LoadState(const CString& name)
{
	AutoBson query(BuildBson(name));
	if (!query)
	{
		cout << "construct query bson failed" << endl;
		return false;
	}
	Student stu;
	if (!LoadOne(name, stu))
	{
		cout << "load one student failed." << endl;
		// return default value
		return 0;
	}

	return stu.nValue;
}


bool StudentAccess::CreateIndex(void)
{
	AutoPoolColl coll(m_collname);
	if (!coll)
	{
		return false;
	}
	string strError;
	if (!MongoClib::CreateIndex(coll, "strName", true, true, &m_err))
	{
		return false;
	}
	if (!MongoClib::CreateIndex(coll, "fValue", true, false, &m_err))
	{
		return false;
	}
	return true;
}