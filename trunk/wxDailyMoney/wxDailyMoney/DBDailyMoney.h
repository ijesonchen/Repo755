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

	// 重写该函数，获取具体数据.
	virtual void GetData(sqlite3_stmt* pStmt, void* pData) = 0;
};

//////////////////////////////////////////////////////////////////////////
// Account

class Account : public ITable
{
public:
	std::wstring id;    // 标识         
	std::wstring name;  // 名称         
	wchar_t type;		// 类别         
	bool iscash;		// 是否现金账户 
	double init;		// 初始值     

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
	int id;               // 序号     
	std::wstring from;    // 来源账户 
	wchar_t fromType;		  // 来源类别 
	std::wstring to;      // 目的账户 
	wchar_t toType;		  // 目的类别 
	long long date;       // 时间     
	double money;         // 金额     
	int bookid;           // 账簿号
	std::wstring note;    // 备注     

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
	// 打开的数据库指针
	sqlite3* m_pDB;
	// 数据库文件名
	std::wstring m_strFileName;
	// 错误信息
	std::wstring m_strMsg;
};

} // namespace DailyMoney