#include "LogQuery.h"
#include <string>
#include <algorithm>
#include <cstdlib>
#include "LogMsg.h"

using namespace std;


LogQuery::LogQuery()
{
	m_iter = m_vtspLogs.cbegin();
}


LogQuery::~LogQuery()
{
}


void LogQuery::clear()
{
	m_vtspLogs.clear();
	m_iter = m_vtspLogs.cbegin();
}


void LogQuery::push_back(std::shared_ptr<LogMsg>& pLog)
{
	m_vtspLogs.push_back(pLog);
	m_iter = m_vtspLogs.cbegin();
}

const std::shared_ptr<LogMsg> LogQuery::operator[](const int idx) const
{
	return m_vtspLogs[idx];
}


int LogQuery::Find(const std::string& strFind)
{
	string strFindLower;
	transform(strFind.cbegin(), strFind.cend(), back_inserter(strFindLower), tolower);
	for (auto beg = m_iter; beg != m_vtspLogs.cend(); ++beg)
	{
		string strMsg = (*beg)->msg;
		string strMsgLower;
		transform(strMsg.cbegin(), strMsg.cend(), back_inserter(strMsgLower), tolower);

		if (strMsgLower.find(strFindLower) != std::string::npos)
		{
			m_iter = beg + 1;
			return beg - m_vtspLogs.cbegin();
		}
	}
	m_iter = m_vtspLogs.cbegin();
	return -1;
}

