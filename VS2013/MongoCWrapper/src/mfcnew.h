// for windows platform, MSVC, use afx.h & DEBUG_NEW to detect memory leak.
// mfcnew.h MUST be last .h file included ONLY in .cpp

#if defined( _WIN32 ) || defined (WIN32)  
	#if defined(_MSC_VER) && defined(_DEBUG) && defined( _AFXDLL ) && defined(_DLL)
		#define __AFX_DEBUG_NEW__
	#endif
	#if defined(_MSC_VER) && defined(_DEBUG) && !defined( _AFXDLL ) && !defined(_DLL)
		#define __AFX_DEBUG_NEW__
	#endif
#endif

#ifdef __AFX_DEBUG_NEW__
	#define new DEBUG_NEW
#endif // __AFX_DEBUG_NEW__