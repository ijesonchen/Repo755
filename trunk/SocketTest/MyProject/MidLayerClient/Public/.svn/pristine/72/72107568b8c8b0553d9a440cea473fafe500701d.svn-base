#ifndef PublicFunc_h__
#define PublicFunc_h__

#include <string>

#ifndef NULL
#define NULL 0
#endif

namespace PublicFunc
{
	// Return length of string. Must be NULL terminated.
	// If length >= 1G, return -1; terminate NULL not counted
	template <typename T>
	int StringLength1G(const T* psz)
	{
		int nLength = 0;
		while (psz[nLength++])
		{
			if (nLength >= 0x40000000)
			{
				return -1;
			}
		}
		return --nLength;
	}

	// Sub string search, return index when sub string first occurred.
	// Very low efficient, but will not call too frequency, so still works well.

	// string must be NULL terminated.
	int SubIndexBF(const char* pSrc, const char* pSub, int nStartPos);

	// string can be not NULL terminated.
	int SubIndexBF(const char* pSrc, int nSrcLength, const char* pSub, int nSubLength, int nStartPos);

	/************************************************************************/
	/*                            RaiiPtr                                   */
	/************************************************************************/
	// used for RAII, store pointer and free mem when class destructed.
	// only class pointer & simple array supported. NO class array support. 
	class RaiiPtr
	{
	public:
		RaiiPtr();
		~RaiiPtr();

		void Add(void* p);

	private:
		void** pBuffer;
		int nCount;
		int nCapacity;
	};


	/************************************************************************/
	/*                         UNICODE <-> UTF-8                            */
	/************************************************************************/
	// Translate between UNICODE and UTF--8

	// wstring (unicode) -> string (utf-8)
	std::string MSEncodeUtf8(const std::wstring& strUnicode);
	// string (utf-8) -> wstring (unicode)
	std::wstring MSDecodeUtf8(const std::string& strUtf8);

	// wstring (unicode) -> string (utf-8)
	std::string EncodeUtf8(const std::wstring& strUnicode);
	// string (utf-8) -> wstring (unicode)
	std::wstring DecodeUtf8(const std::string& strUtf8);

	// Return data write count.
	// If pBuffer is NULL, return required buffer length.
	// Whether NULL terminator included lies on input.

	// input must NULL terminated.
	int UnicodeToUtf8(const wchar_t* pszUnicode, char* pUtf8, int nUtf8Len);

	// function does not check NULL terminator.
	int UnicodeToUtf8(const wchar_t* pUnicode, int nUnicodeBufLen, char* pUtf8, int nUtf8Len);


	// input must NULL terminated.
	int Utf8ToUnicode(const char* pszUtf8,  wchar_t* pUnicode, int nUnicodeBufLen);

	// function does not check NULL terminator.
	int Utf8ToUnicode(const char* pUtf8, int nUtf8Len,  wchar_t* pUnicode, int nUnicodeBufLen);
};

#endif // PublicFunc_h__
