#ifndef LogQuery_h__
#define LogQuery_h__

#include <memory>
#include <vector>

class LogMsg;

class LogQuery
{
public:
	LogQuery();
	~LogQuery();
private:
	std::vector<std::shared_ptr<LogMsg>> m_vtspLogs;
	std::vector<std::shared_ptr<LogMsg>>::const_iterator m_iter;
public:
	void clear();
	void push_back(std::shared_ptr<LogMsg>& pLog);
	const std::shared_ptr<LogMsg> operator[](const int idx) const;
	int Find(const std::string& strFind);
};

#endif // LogQuery_h__
