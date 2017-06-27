#include "DBDailyMoney.h"
#include "..\Sqlite3008006\sqlite3.h"
#include <iostream>
#include <sstream>
#include <xstring>

using namespace std;


namespace DailyMoney{

// 数据库时间附加时差
// const long long kTimeOffset = 12 * 3600;

const wchar_t* const ksqlAccountSelect = 
	L"select * from dm_acc order by id_";

const wchar_t* const ksqlDetailSelect = 
	L"select * from dm_det ";
const wchar_t* const ksqlDetailOrder = 
	L" order by dt_, id_";

static inline const char* CheckVal(const unsigned char* pval)
{
	const char* p = (const char*)pval;
	if (!p)
	{
		stringstream ss;
		ss << "CheckVal error, NULL string" << endl;
		throw ss.str();
	}
	return p ? p : "";
}

static inline const wchar_t* CheckVal(const void* pval)
{
	const wchar_t* p = (const wchar_t*)pval;
	if (!p)
	{
		wstringstream ss;
		ss << L"CheckVal error, NULL string" << endl;
		throw ss.str();
	}
	return p ? p : L"";
}

//////////////////////////////////////////////////////////////////////////
// ITable
//////////////////////////////////////////////////////////////////////////

ITable::ITable(const wchar_t** colNames, int ncol)
: m_pColNames(colNames)
, m_nColumn(ncol)
{
}

template<class T>
bool ITable::LoadBySql(sqlite3* pdb, const wchar_t* zsql, 
					  std::deque<T>& dqAccounts, std::wstring* perr /*= NULL*/)
{
	if (perr)
	{
		perr->clear();
	}

	// check member
	if (!m_pColNames || m_nColumn <= 0)
	{
		if (perr)
		{
			*perr = L"error: column names or count not set.";
		}
		return false;
	}

	bool bRet = false;
	int nRet = 0;
	sqlite3_stmt* pStmt = NULL;

	try
	{
		nRet = sqlite3_prepare16_v2(pdb, zsql, (wcslen(zsql) + 1) * 2, &pStmt, NULL);
		if (nRet != SQLITE_OK || !pStmt)
		{
			wstringstream ss;
			ss << L"sqlite3_prepare_v2 error: " << nRet << endl;
			throw ss.str();
		}

		int nColCount = sqlite3_column_count(pStmt);
		if (nColCount <= 0)
		{
			wstringstream ss;
			ss << L"sqlite3_column_count returns no data: " << nColCount << endl;
			throw ss.str();
		}

		bool bCheckHeader = true;
		while ((nRet = sqlite3_step(pStmt)) == SQLITE_ROW)
		{
			if (bCheckHeader)
			{
				wstring strerr;
				if (!CheckHeader(pStmt, &strerr))
				{
					throw strerr;
				}

				bCheckHeader = false;
			}

			// table spec
			T data;
			GetData(pStmt, &data);

			dqAccounts.push_back(data);
		}
		if (nRet == SQLITE_ERROR)
		{
			throw -2;
		}
		bRet = true;
	}
	catch (wstring e)
	{
		wstringstream ss;
		ss << L"error: " << e << endl;
		if (perr)
		{
			*perr = ss.str();
		}
	}
	catch (int e)
	{
		wstringstream ss;
		ss << L"error "<< e << L" : " << sqlite3_errmsg16(pdb) << endl;
		if (perr)
		{
			*perr = ss.str();
		}
	}

	if (pStmt)
	{
		nRet = sqlite3_finalize(pStmt);
		pStmt = NULL;
	}

	return bRet;
}


bool ITable::CheckHeader(sqlite3_stmt* pStmt, std::wstring* perr /*= NULL*/)
{
	const int nColCount = sqlite3_column_count(pStmt);

	try
	{
		if (m_nColumn != nColCount)
		{
			wstringstream ss;
			ss << L"DBDailyMoney::Account::CheckHeader column count error: " 
				<< nColCount << L" should be " << nColCount << endl;
			throw ss.str();
		}
		for (int ii = 0; ii < nColCount; ++ii)
		{
			const wchar_t* pColName = (const wchar_t*)sqlite3_column_name16(pStmt, ii);
			if (memcmp(pColName, m_pColNames[ii], (wcslen(pColName) + 1) * 2) != 0)
			{
				wstringstream ss;
				ss << L"DBDailyMoney::Account::CheckHeader column name error: " 
					<< pColName << L" should be " << m_pColNames[ii] << endl;
				throw ss.str();
			}
		}

	}
	catch (wstring str)
	{
		if (perr)
		{
			*perr = str;
		}
		return false;
	}
	return true;
}


//////////////////////////////////////////////////////////////////////////
// Account
//////////////////////////////////////////////////////////////////////////

enum AccountColumnIdx
{
	AccountColumnIdxId = 0,
	AccountColumnIdxName = 1,
	AccountColumnIdxType = 2,
	AccountColumnIdxCash = 3,
	AccountColumnIdxInit = 4,
};

const int kaccColCount = 5;
const wchar_t* kaccColNames[kaccColCount]= {
	L"id_", L"nm_", L"tp_", L"ca_", L"iv_",
} ;

Account::Account()
: ITable(kaccColNames, kaccColCount)
, type(0)
, iscash(false)
, init(0)
{
}

bool Account::Load(sqlite3* pdb, std::deque<Account>& dqAccounts, std::wstring* perr /*= NULL*/)
{
	return Account().LoadBySql<Account>(pdb, ksqlAccountSelect, dqAccounts, perr);
}

std::wstring Account::GetTypeString(wchar_t chType)
{
	switch (chType)
	{
	case L'A':
		return L"收入";
		break;
	case L'B':
		return L"消费";
		break;
	case L'C':
		return L"资产";
		break;
	case L'D':
		return L"债务";
		break;
	case L'E':
		return L"其他";
		break;
	default:
		return L"未知";
		break;
	}
}
void Account::GetData(sqlite3_stmt* pStmt, void* pData)
{
	Account& acc = *(Account*)pData;

	acc.id = CheckVal(sqlite3_column_text16(pStmt, AccountColumnIdxId));
	acc.name = CheckVal(sqlite3_column_text16(pStmt, AccountColumnIdxName));
	wchar_t type = CheckVal(sqlite3_column_text16(pStmt, AccountColumnIdxType))[0];
	acc.type = type;
	acc.iscash = sqlite3_column_int(pStmt, AccountColumnIdxCash) != 0;
	acc.init = sqlite3_column_double(pStmt, AccountColumnIdxInit);
}

//////////////////////////////////////////////////////////////////////////
// Detail
//////////////////////////////////////////////////////////////////////////

enum DetailColumnIdx
{
	DetailColumnIdxId = 0,
	DetailColumnIdxFrom = 1,
	DetailColumnIdxFromType = 2,
	DetailColumnIdxTo = 3,
	DetailColumnIdxToType = 4,
	DetailColumnIdxDate = 5,
	DetailColumnIdxMoney = 6,
	DetailColumnIdxBookId = 7,
	DetailColumnIdxNote = 8
};

const int kdetColCount = 9;
const wchar_t* kdetColNames[kdetColCount] = {
	L"id_", L"fr_", L"frt_", L"to_", L"tot_",
	L"dt_", L"mn_", L"ar_", L"nt_",
} ;

Detail::Detail()
: ITable(kdetColNames, kdetColCount)
, id(-1)
, fromType(0)
, toType(0)
, date(0)
, money(0)
, bookid(0)
{
}

// tfrom, tto is in seconds
bool Detail::Load(sqlite3* pdb, std::deque<Detail>& dqDetails,
				  long long tfrom, long long tto, int bookid, std::wstring* perr /*= NULL*/)
{
	// tfrom, tto is in milliseconds in db
	tfrom += kTimeOffset;
	tto += kTimeOffset;
	wstringstream ss;
	ss << ksqlDetailSelect 
		<< L"where dt_ >= " << tfrom * 1000 << L" and dt_ <= " << tto * 1000
		<< L" and ar_ = " << bookid
		<< ksqlDetailOrder;
	wstring wstr = ss.str();
	const wchar_t* psql = wstr.c_str();
	return Detail().LoadBySql<Detail>(pdb, psql, dqDetails, perr);
}


void Detail::GetData(sqlite3_stmt* pStmt, void* pData)
{
	Detail& det = *(Detail*)pData;
	wchar_t wch = 0;

	det.id = sqlite3_column_int(pStmt, DetailColumnIdxId);
	det.from = CheckVal(sqlite3_column_text16(pStmt, DetailColumnIdxFrom));
	wch = CheckVal(sqlite3_column_text16(pStmt, DetailColumnIdxFromType))[0];
	det.fromType = wch;

	det.to = CheckVal(sqlite3_column_text16(pStmt, DetailColumnIdxTo));
	wch = CheckVal(sqlite3_column_text16(pStmt, DetailColumnIdxToType))[0];
	det.toType = wch;
	det.date = sqlite3_column_int64(pStmt, DetailColumnIdxDate);
	det.money = sqlite3_column_double(pStmt, DetailColumnIdxMoney);
	det.bookid = sqlite3_column_int(pStmt, DetailColumnIdxBookId);
	det.note = CheckVal(sqlite3_column_text16(pStmt, DetailColumnIdxNote));
}

//////////////////////////////////////////////////////////////////////////
// DBDailyMoney

DBDailyMoney::DBDailyMoney(void)
: m_pDB(NULL)
{
}

DBDailyMoney::~DBDailyMoney(void)
{
	Close();
}


bool DBDailyMoney::LoadAccounts(void)
{
	if (!m_pDB)
	{
		m_strMsg = L"DB not open.";
		return false;
	}
	return Account::Load(m_pDB, m_dqAccounts, &m_strMsg);
}

bool DBDailyMoney::LoadDetail(std::deque<Detail>& dqDetails, long long tfrom, long long tto, int bookid /*= 0*/)
{
	if (!m_pDB)
	{
		m_strMsg = L"DB not open.";
		return false;
	}
	return Detail::Load(m_pDB, dqDetails, tfrom, tto, bookid, &m_strMsg);
}


bool DBDailyMoney::Open(const wchar_t* putf16name)
{
	if (m_pDB)
	{
		Close();
	}
	if (!putf16name || (*putf16name == 0))
	{
		// NULL input or empty file name
		wstringstream ss;
		ss << L"Open file error, NULL input or empty file name." << endl;
		m_strMsg = ss.str();
		return false;
	}

	int nRet = 0;

	nRet = sqlite3_open16(putf16name, &m_pDB);
	if (SQLITE_OK != nRet)
	{
		wstringstream ss;
		ss << L"Open file " << putf16name << L" error, code: " << nRet << endl;
		m_strMsg = ss.str();
		sqlite3_close(m_pDB);
		m_pDB = NULL;
		return false;
	}
	m_strFileName = putf16name;
	return true;
}

void DBDailyMoney::Close(void)
{
	m_dqAccounts.clear();
	sqlite3_close(m_pDB);
	m_pDB = NULL;;

	m_strFileName.clear();
	m_strMsg.clear();
}

} // namespace DailyMoney