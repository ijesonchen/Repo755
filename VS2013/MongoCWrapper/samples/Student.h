#ifndef SStudent_h__
#define SStudent_h__

// this should test with mfc
#include <afx.h>
#include <vector>


struct Student
{
	friend
		bool operator==(const Student& lhs, const Student& rhs);
public:
	Student();
	~Student();

	CString strName;
	CString strMemo;
	bool bValue = false;
	int nValue = 0;
	float fValue = 0;
	std::vector<CString> vtText;
};

inline bool operator==(const Student& lhs, const Student& rhs)
{
	if (lhs.strName == rhs.strName &&
		lhs.strMemo == rhs.strMemo &&
		lhs.bValue == rhs.bValue &&
		lhs.nValue == rhs.nValue &&
		lhs.fValue == rhs.fValue &&
		lhs.vtText == rhs.vtText)
	{
		return true;
	}
	return false;
}

#endif // SStudent_h__
