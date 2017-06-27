#include <regex> 
#include <deque>
#include <atlstr.h>

void ReMatchA(const char* pString, char* pRe, std::deque<std::string>& dqMatches)
{
	std::tr1::regex rx(pRe);
	std::tr1::match_results<std::string::const_iterator> mr2;	// matched
	std::string str = pString;
	while (regex_search(str, mr2, rx))
	{
		dqMatches.push_back(mr2.str());
		pString += mr2.position() + mr2.str().length();	
		str = pString;
	}
}

void ReMatchW(const wchar_t* pwString, wchar_t* pwRE, std::deque<std::wstring>& dqMatches)
{
	std::tr1::wregex rx(pwRE);
	std::tr1::match_results<std::wstring::const_iterator> mr2;	// matched
	std::wstring str = pwString;
	while (regex_search(str, mr2, rx))
	{
		dqMatches.push_back(mr2.str());
		pwString += mr2.position() + mr2.str().length();	
		str = pwString;
	}
}

#ifdef UNICODE
	#define stringT wstring
	#define REMatchT ReMatchW
#else
	#define stringT string
	#define REMatchT ReMatchA
#endif // UNICODE


void ReMatch(const TCHAR* ptString, TCHAR* ptRE, std::deque<CString>& dqMatches)
{
	std::deque<std::stringT> dqStrings;
	REMatchT(ptString, ptRE, dqStrings);

	for (size_t ii = 0, iLoop = dqStrings.size();
		ii < iLoop; ++ii)
	{
		dqMatches.push_back(dqStrings[ii].c_str());
	}
}

int main() 
{ 
	char* pString = "asdfiasanka-001dfAa002slan-da-009asdfsfdasf";
	char* pPattern = "[a-z, A-Z]{2,6}[-]?[0-9]{2,6}";
	std::deque<std::string> dqMatches;

	ReMatchA(pString, pPattern, dqMatches);

	return (0); 
} 
