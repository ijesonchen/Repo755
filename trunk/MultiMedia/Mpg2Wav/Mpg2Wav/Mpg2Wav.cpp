// Mpg2Wav.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Mpg2Wav.h"
#include "..\..\DShowComm\DShow.h"
#include "..\..\DShowComm\DShowComm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// The one and only application object

CWinApp theApp;

using namespace std;


// #define RELEASE_PTR_RESOURCE(x) if (x)\
// {\
// 	unsigned long r = 0;\
// 	r = x->Release();\
// 	x = NULL;\
// }

inline unsigned long RELEASE_PTR_RESOURCE(IUnknown* x) 
{
	unsigned long r = 0;
	if (x)
	{
		r = x->Release();
		x = NULL;
	}
	return r;
}

#define THROW_FAILED_HR(hr) if ((((HRESULT)(hr)) < 0))\
{\
	nRet = hr;\
	throw ERROR_THROW;\
}

#define THROW_ERROR_CODE(hr, y) if (S_OK != hr)\
{\
	nRet = y;\
	throw ERROR_THROW;\
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
	}

	wchar_t* pInput = NULL;
	wchar_t* pOutput = NULL;

	if (argc != 3)
	{
		printf("Syntax error: Mpg2Wav.exe Input.mpg Output.wav\n");
		return -1;
	}

	pInput = argv[1];
	pOutput = argv[2];


	// filter UUID table
	GUID CLSID_LavAudioDecoder;
	UuidFromStringA((unsigned char*)"E8E73B6B-4CB3-44A4-BE99-4F7BCB96E491", &CLSID_LavAudioDecoder);
	// LAVSplitter will be automatically add when RenderFile
// 	GUID CLSID_LAVSplitter;
// 	UuidFromStringA((unsigned char*)"171252A0-8820-4AFE-9DF8-5C92B2D66B04", &CLSID_LAVSplitter);
	// below UUIDs are defined in uuids.h of Platform SDK v6.0A
// 	GUID CLSID_NullRender;
// 	UuidFromStringA((unsigned char*)"C1F400A4-3F08-11D3-9F0B-006008039E37", &CLSID_NullRender);
// 	GUID CLSID_AcmWrapper;
// 	UuidFromStringA((unsigned char*)"6A08CF80-0E18-11CF-A24D-0020AFD79767", &CLSID_AcmWrapper);
// 	GUID CLSID_WavDest;
// 	UuidFromStringA((unsigned char*)"3C78B8E2-6C4D-11D1-ADE2-0000F8754B99", &CLSID_WavDest);
// 	GUID CLSID_FileWriter;
// 	UuidFromStringA((unsigned char*)"8596E5F0-0DA5-11D0-BD21-00A0C911CE86", &CLSID_FileWriter);

	WAVEFORMATEX WavFmt;
	AM_MEDIA_TYPE AmType;
	WAVEFORMATEX* pWavFmt = &WavFmt;
	pWavFmt->wFormatTag = 1;
	pWavFmt->nChannels = 1;
	pWavFmt->nSamplesPerSec = 16000;
	pWavFmt->nAvgBytesPerSec = 32000;
	pWavFmt->nBlockAlign = 2;
	pWavFmt->wBitsPerSample = 16;
	pWavFmt->cbSize = 0;
	AM_MEDIA_TYPE* pAmType = &AmType;
	pAmType->majortype = MEDIATYPE_Audio;
	pAmType->subtype = MEDIASUBTYPE_PCM;
	pAmType->bFixedSizeSamples = 1;
	pAmType->bTemporalCompression = 0;
	pAmType->lSampleSize = 2;
	pAmType->formattype = FORMAT_WaveFormatEx;
	pAmType->pUnk = NULL;
	pAmType->cbFormat = sizeof(WAVEFORMATEX);
	pAmType->pbFormat = (BYTE*)pWavFmt;

	HRESULT hr = 0;
	int nRet = 0;
	IGraphBuilder* pGraphBuilder = NULL;
	IBaseFilter* pWavDest = NULL;
	IBaseFilter* pACMWrapper = NULL;
	IBaseFilter* pNullRender = NULL;
	IBaseFilter* pLavAudioDecoder = NULL;
	IBaseFilter* pFileWriter = NULL;
	IFileSinkFilter* pSink = NULL;
	IMediaEvent *pMediaEvent = NULL;
	IMediaControl* pMediaControl = NULL;

	bool bComInited = false;
	try
	{	
		hr = CoInitialize(NULL);
		switch (hr)
		{
		case S_FALSE:
			bComInited = true;
			break;
		default:
			break;
		}
		THROW_FAILED_HR(CoCreateInstance(CLSID_FilterGraph, 
										NULL, 
										CLSCTX_INPROC_SERVER, 
										IID_IGraphBuilder, 
										(void**) &pGraphBuilder));
		THROW_FAILED_HR(pGraphBuilder->RenderFile(pInput, NULL));

		IMediaFilter* spMediaFilter = NULL;
		THROW_FAILED_HR(pGraphBuilder->QueryInterface(IID_IMediaFilter,(void**)&spMediaFilter))
		THROW_FAILED_HR(spMediaFilter->SetSyncSource(NULL));

		RELEASE_PTR_RESOURCE(spMediaFilter);

		//get video and audio renderer
		CDShow::CBaseFilterList lstBV;
		CDShow::CBaseFilterList lstBA;
//		THROW_FAILED_HR(CDShow::GetAVRenderer(pGraphBuilder,lstBA,lstBV));
		THROW_ERROR_CODE(CDShow::GetAVRenderer(pGraphBuilder,lstBA,lstBV),
						ERROR_CDSHOW_GET_AVRENDER);

		// failed if no audio
		if(lstBA.empty())
		{
			nRet = ERROR_CDSHOW_NOAUDIO;
			throw ERROR_THROW;
		}

		//clear video link way, connect video stream to null renderer 
		for(CDShow::CBaseFilterList::iterator spBV=lstBV.begin();spBV!=lstBV.end();spBV++)
		{
			//delete invalid filter
			hr = CDShow::NukeUpstream(pGraphBuilder,*spBV);
			THROW_ERROR_CODE(hr, ERROR_CDSHOW_VIDEO_NUKEUP);

			//delete Video Renderer.
			hr = CDShow::RemoveFilter(pGraphBuilder,*spBV);
		}

		CDShow::CBaseFilterList::iterator spBA=lstBA.begin();
		hr = CDShow::ConnectToSplitter(pGraphBuilder, 
										*spBA, 
										CLSID_LavAudioDecoder, 
										&pLavAudioDecoder);

		THROW_ERROR_CODE(hr, ERROR_CDSHOW_CONNECT_SPLITTER);


		hr = CDShow::RemoveFilter(pGraphBuilder,*spBA);

		hr = CDShow::AddFilterByClsid(pGraphBuilder,CLSID_ACMWrapper,NULL,&pACMWrapper);
		THROW_ERROR_CODE(hr, ERROR_CDSHOW_ADD_ACM);
		hr = CDShow::ConnectFilters(pGraphBuilder,pLavAudioDecoder,pACMWrapper);
		THROW_ERROR_CODE(hr, ERROR_CDSHOW_CONNECT_ACM);		

		hr = CDShow::AddFilterByClsid(pGraphBuilder,CLSID_WaveDest,NULL,&pWavDest);
		THROW_ERROR_CODE(hr, ERROR_CDSHOW_ADD_WAVEDEST);
		hr = CDShow::ConnectFilters(pGraphBuilder,pACMWrapper,pWavDest);
		THROW_ERROR_CODE(hr, ERROR_CDSHOW_CONNECT_WAVEDEST);

		hr = CDShow::AddFilterByClsid(pGraphBuilder,CLSID_FileWriter,NULL,&pFileWriter);
		THROW_ERROR_CODE(hr, ERROR_CDSHOW_ADD_FILEWRITER);
		hr = CDShow::ConnectFilters(pGraphBuilder,pWavDest,pFileWriter);
		THROW_ERROR_CODE(hr, ERROR_CDSHOW_CONNECT_FILEWRITER);

		hr = pFileWriter->QueryInterface(IID_IFileSinkFilter, (void**) &pSink);
		THROW_ERROR_CODE(hr, ERROR_GET_FileSink);
		hr = pSink->SetFileName(pOutput, NULL);
		THROW_ERROR_CODE(hr, ERROR_SetFileName);

		// Check for output File
		CFileStatus fileStatus;
		CString str;
		str = pOutput;
		if (CFile::GetStatus(str, fileStatus))
		{
			try {
				CFile::Remove(str);
			} catch(...) {
				// nothing to do
			}			
		}

		if (!SetFilterFormat(pAmType, pACMWrapper))
		{
			nRet = ERROR_SetFilterFormat;
			throw ERROR_THROW;
		}

		// run graph
		hr = pGraphBuilder->QueryInterface(&pMediaControl);
		THROW_ERROR_CODE(hr, ERROR_GET_MediaControl)
		hr = pMediaControl->Run();
		if (hr == S_FALSE)
		{
			// check state
			OAFilterState state;
			DWORD dw = GetTickCount();
			hr = pMediaControl->GetState(1000, &state);
			dw = GetTickCount() - dw;
			if (hr != S_OK && hr != VFW_S_STATE_INTERMEDIATE)
			{
				nRet = ERROR_MediaControl_Run;
				throw ERROR_THROW;
			}
		}

		long nCode = 0;
		hr = pGraphBuilder->QueryInterface(IID_IMediaEvent, (void**) &pMediaEvent);
		THROW_ERROR_CODE(hr, ERROR_GET_MediaEvent);
		// Wait until job complete
		while (nCode != EC_COMPLETE) {
			hr = pMediaEvent->WaitForCompletion(1000, &nCode);
		}
		pMediaControl->Stop();

		wprintf(_T("O:  %s\n"), pOutput);
		nRet = 0;
	}
	catch (int)
	{
		// error occurred
		wprintf(_T("E:  %s  C: 0x%X\n"), pInput, nRet);
	}
	catch (CInvalidArgException*)
	{
		wprintf(_T("E:  %s  C: 0x%X\n"), pInput, -2);
		return -2;
	}

	// clean up
	RELEASE_PTR_RESOURCE(pGraphBuilder);
	RELEASE_PTR_RESOURCE(pLavAudioDecoder);
	RELEASE_PTR_RESOURCE(pMediaEvent);
	RELEASE_PTR_RESOURCE(pMediaControl);
	RELEASE_PTR_RESOURCE(pSink);
	RELEASE_PTR_RESOURCE(pFileWriter);
	RELEASE_PTR_RESOURCE(pWavDest);
	RELEASE_PTR_RESOURCE(pACMWrapper);
	RELEASE_PTR_RESOURCE(pNullRender);

	if (!bComInited)
	{
		CoUninitialize();
	}
	// if not, LAV filter will not exit normally.
	Sleep(100);

	return nRet;
}
