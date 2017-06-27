changelog:


//////////////////////////////////////////////////////////////////////////
// 20131226
1.
TIXML_NO_CRLF_TRANSFORM micro for CR/LF transform to &#xxx;

2.
TiXmlElement:
	// user defined funcions
protected:
	// max char[] length for math value
	static const int MAX_MATH_VALUE_LEN = 32;

public:
	// quick add new child element with text
	void AddNewChild(const char* const pChildName, const char* const pChildText);
	void AddNewChild(const char* const pChildName, const unsigned uChildValue);
	void AddNewChild(const char* const pChildName, const int iChildValue);
	void AddNewChild(const char* const pChildName, const float fChildValue);

	// quick add new child element only.
	TiXmlElement* AddNewElem(const char* const pChildName);

3.
TiXmlDocument:
	// quick add new declaration. !!! call only once
	void AddNewDeclaration(	const char * _version,
							const char * _encoding,
							const char * _standalone = "");

	// quick add root element. !!! call only once
	TiXmlElement* AddRoot(const char * rootName);
