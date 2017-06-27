#pragma once

#include <string>
#include <deque>

struct sqlite3;
struct sqlite3_stmt;

namespace DailyMoney{

const long long kTimeOffset = 12 * 3600;

//////////////////////////////////////////////////////////////////////////
// ITable
class ITable
{
public:
	ITable(const wchar_t** colNames, int ncol);

	template<class T>
	bool LoadBySql(sqlite3* pdb, const wchar_t* zsql, std::deque<T>& dqData, std::wstring* perr = NULL);

private:
	// pass from constructor, outer const wchar* array.
	const wchar_t** const m_pColNames;
	const int m_nColumn;

	bool CheckHeader(sqlite3_stmt* pStmt, std::wstring* perr = NULL);

	// ��д�ú�������ȡ��������.
	virtual void GetData(sqlite3_stmt* pStmt, void* pData) = 0;
};

//////////////////////////////////////////////////////////////////////////
// Account

class Account : public ITable
{
public:
	std::wstring id;    // ��ʶ         
	std::wstring name;  // ����         
	wchar_t type;		// ���         
	bool iscash;		// �Ƿ��ֽ��˻� 
	double init;		// ��ʼֵ     

	Account();

	static bool Load(sqlite3* pdb, std::deque<Account>& dqAccounts, std::wstring* perr = NULL);

	static std::wstring GetTypeString(wchar_t chType);
private:
	virtual void GetData(sqlite3_stmt* pStmt, void* pData);
};


//////////////////////////////////////////////////////////////////////////
// Detail

class Detail : public ITable
{
public:
	int id;               // ���     
	std::wstring from;    // ��Դ�˻� 
	wchar_t fromType;		  // ��Դ��� 
	std::wstring to;      // Ŀ���˻� 
	wchar_t toType;		  // Ŀ����� 
	long long date;       // ʱ��     
	double money;         // ���     
	int bookid;           // �˲���
	std::wstring note;    // ��ע     

	Detail();

	// tfrom, tto is in seconds
	static bool Load(sqlite3* pdb, std::deque<Detail>& dqDetails, 
		long long tfrom, long long tto, int bookid, std::wstring* perr = NULL);

private:
	virtual void GetData(sqlite3_stmt* pStmt, void* pData);
};


//////////////////////////////////////////////////////////////////////////
// DBDailyMoney
class DBDailyMoney
{
public:
	DBDailyMoney(void);
	~DBDailyMoney(void);

public:
	std::deque<Account> m_dqAccounts;

	bool LoadAccounts(void);
	bool LoadDetail(std::deque<Detail>& dqDetails, long long tfrom, long long tto, int bookid = 0);

	bool Open(const wchar_t* putf16name);
	void Close(void);

	bool IsOpen(void)
	{
		return m_pDB != NULL;
	}

	std::wstring GetMsg(void)
	{
		return m_strMsg;
	}

private:
	// �򿪵����ݿ�ָ��
	sqlite3* m_pDB;
	// ���ݿ��ļ���
	std::wstring m_strFileName;
	// ������Ϣ
	std::wstring m_strMsg;
};

} // namespace DailyMoney