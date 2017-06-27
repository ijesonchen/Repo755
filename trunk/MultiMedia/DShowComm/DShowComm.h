#ifndef __DSHOWCOMMON_H__
#define __DSHOWCOMMON_H__

#define __STR2__(x) #x
#define __STR1__(x) __STR2__(x)
#define ___A __FILE__ "("__STR1__(__LINE__)") : +++++++++++ : "

#pragma message(___A "qedit.h need DirectShow include directory included")
#pragma message(___A "DShowComm.h should be included in Stdafx.h")
#pragma message(___A "DShowComm.cpp no precompiled headers")

// C:\Program Files\Microsoft DirectX 9.0 SDK (Summer 2004)\Include

// disable Security Enhancements in the CRT warning
#pragma warning(once: 4995  4996)
#pragma warning(disable: 4819)

#include <afxwin.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <DShow.h>	// DShow common heads
//#include <qedit.h>
#include <stdlib.h>

#define AUDIO_FILE_DLL	(_T("Hzmv2PcmDll.dll"))


#pragma comment(lib, "strmiids.lib")	// for CLSID & IID
#pragma comment(lib, "rpcrt4.lib")		// uuidfromstring
#pragma comment(lib, "Quartz.lib")		// AMGetErrorText

HRESULT AddFilterByClsid( IGraphBuilder *pGraph, LPCWSTR wszName, const GUID& clsid, IBaseFilter **ppF );
bool SetFilterFormat( AM_MEDIA_TYPE* pStreamFormat, IBaseFilter* pBaseFilter );
HRESULT ConnectToMuxer(IGraphBuilder* pGraphBuild, 
					   IBaseFilter* pRender,
					   IBaseFilter* pCodec,
					   REFCLSID clsid,
					   IBaseFilter** ppMuxer);
// #define _DEBUG_GRAPHEDIT_

extern const int ERROR_THROW;

extern const int ERROR_UNKNOWN;

extern const int ERROR_INITFGM_COINIT;
extern const int ERROR_INITFGM_FILEREND;
extern const int ERROR_INITFGM_QUERYINT;
extern const int ERROR_INITFGM_RUN;

extern const int ERROR_GETLEN_INIT;
extern const int ERROR_GETLEN_SETFILE;
extern const int ERROR_GETLEN_GETLEN;

extern const int ERROR_CONV_GRAPHBUILDER;
extern const int ERROR_CONV_ADDDEST;
extern const int ERROR_CONV_ADDCODEC;
extern const int ERROR_CONV_ADDOUTPUT;
extern const int ERROR_CONV_SETOUTPUT;
extern const int ERROR_CONV_RENDER;
extern const int ERROR_CONV_SETFMT;
extern const int ERROR_CONV_RUN;
extern const int ERROR_CONV_ADDPASER;
extern const int ERROR_CONV_CTRL;
extern const int ERROR_CONV_ADD_NULLRENDER;
extern const int ERROR_CONV_ADD_LAVSPLITTER;
extern const int ERROR_CONV_ADD_AMRDECODER;
extern const int ERROR_CONV_ADD_AMRSPLITTER;

extern const int ERROR_CDSHOW_GET_AVRENDER;
extern const int ERROR_CDSHOW_NOAUDIO;
extern const int ERROR_CDSHOW_VIDEO_NUKEUP;
extern const int ERROR_CDSHOW_CONNECT_SPLITTER;
extern const int ERROR_CDSHOW_ADD_ACM;
extern const int ERROR_CDSHOW_CONNECT_ACM;
extern const int ERROR_CDSHOW_ADD_WAVEDEST;
extern const int ERROR_CDSHOW_CONNECT_WAVEDEST;
extern const int ERROR_CDSHOW_ADD_FILEWRITER;
extern const int ERROR_CDSHOW_CONNECT_FILEWRITER;
extern const int ERROR_GET_FileSink;
extern const int ERROR_SetFileName;
extern const int ERROR_SetFilterFormat;
extern const int ERROR_GET_MediaControl;
extern const int ERROR_MediaControl_Run;
extern const int ERROR_GET_MediaEvent;

#endif // __DSHOWCOMMON_H__