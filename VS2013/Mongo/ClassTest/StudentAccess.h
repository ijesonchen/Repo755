#ifndef StudentAccess_h__
#define StudentAccess_h__

#include "Student.h"
#include <string>
#include <deque>
#include "MongoClient.h"

class StudentAccess
{
public:
	StudentAccess();
	~StudentAccess();

	bool Insert(const Student& student);
	bool Delete(const CString& name);
	bool Update(const Student& student);
	bool UpdateBool(const CString& name, bool b);
	bool ModifyBool(const CString& name, bool b);
	bool LoadOne(const CString& name, Student& student);
	bool LoadAll(std::deque<Student>& dqStudents);
	bool LoadCond(std::deque<Student>& dqStudents, const int n);
	int  LoadState(const CString& name);

	bool CreateIndex(void);
private:
	const std::string m_collname = "Student";
	std::string m_err;
	bool QueryLoad(std::deque<Student>& dqObjects, const bson_t* query, int nMax = 0);

	bool QueryLoad(std::deque<Student>& dqObjects, int nMax = 0)
	{
		MongoClib::AutoBson query;
		return QueryLoad(dqObjects, query, nMax);
	}

	MongoClib::AutoBson BuildBson(const Student& student);
	bson_t* BuildBson(const CString& name);
	void ParseBson(Student& student, const bson_t* doc);
};

#endif // StudentAccess_h__
