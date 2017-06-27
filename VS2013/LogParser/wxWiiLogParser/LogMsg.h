#ifndef LogMsg_h__
#define LogMsg_h__

#include <string>
#include <vector>

class LogMsg
{
public:
	LogMsg(const std::vector<std::string>& vtlogLines);
	~LogMsg();

	std::string date;
	std::string time;
	std::string dateTime;
	std::string category;
	std::string tag;
	std::string msg;
	bool isValid;

	static bool CheckLogLeadin(const std::string& str);

private:
};

#endif // LogMsg_h__
