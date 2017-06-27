#include "LogMsg.h"
#include <string>
#include <sstream>
#include <ctime>
#include <iomanip>

using namespace std;

namespace
{
	// 2015-11-06 17:30:52 [信息] KeywordLayer: message received
	static const string fmtDate = "2015-11-06";
	static const string fmtTime = "17:30:52";
	static const string fmtDt = fmtDate + " " + fmtTime; // "2015-11-06 17:30:52"
	static const string fmtCate = "[信息]";
	static const string fmtLead = fmtDt + " " + fmtCate; // "2015-11-06 17:30:52 [信息]"
	static const string fmtMsgPart = "KeywordLayer: message received";
	static const string fmtMsg = fmtDt + " " + fmtCate + " " + fmtMsgPart;

	static const string fmtTm = "%Y-%m-%d %H:%M:%S";

	const int maxMsgLength = 1024;
}


LogMsg::LogMsg(const std::vector<std::string>& vtlogLines)
	: isValid(false)
{
	try
	{
		ostringstream oss;
		for (auto beg = vtlogLines.cbegin(); beg != vtlogLines.cend(); ++beg)
		{
			oss << *beg << " \\n ";
		}
		const string logLine(oss.str());
		if (!CheckLogLeadin(logLine))
		{
			return;
		}
		istringstream iss(oss.str());
		string word;
		// 2015-11-06 14:27:21 [信息] RecogLayer: SocketProcData sent 29 bytes.
		if (!(iss >> word))
		{
			return;
		}
		if (word.length() == string(fmtDate).length())
		{
			date = word;
		}
		if (!(iss >> word))
		{
			return;
		}
		if (word.length() == string(fmtTime).length())
		{
			time = word;
		}
		dateTime = date + " " + time;
		if (!(iss >> word))
		{
			return;
		}
		if (word.length() == string(fmtCate).length())
		{
			if (word.front() != '[' || word.back() != ']')
			{
				return;
			}
			category = word.substr(1, string(fmtCate).length() - 2);
		}
		if (!(iss >> word))
		{
			return;
		}
		if ((word.at(word.length() - 1) == ':') ||
			(word.at(word.length() - 1) == '：'))
		{
			if (word.length() > 1)
			{
				tag = word.substr(0, word.length() - 1);
			}
			stringbuf issleft;
			iss >> &issleft;

			msg = issleft.str();
		}
		else
		{
			stringbuf issleft;
			iss >> &issleft;

			ostringstream ossLog;
			ossLog << word << " " << issleft.str();
			msg = ossLog.str();
		}

		if (msg.length() >= maxMsgLength)
		{
			msg = msg.substr(0, maxMsgLength);
		}

		isValid = true;
	}
	catch (std::exception& e)
	{
		string str = e.what();
	}
}


LogMsg::~LogMsg()
{
}


bool LogMsg::CheckLogLeadin(const std::string& str)
{
	if (str.length() < fmtLead.length())
	{
		return false;
	}
	istringstream iss(str.substr(0, fmtDt.length()));
	tm when;
	if (!(iss >> get_time(&when, fmtTm.c_str())))
	{
		return false;
	}

	return true;
}
