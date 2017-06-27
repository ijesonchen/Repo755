#include "DShow.h"
#include "dshowcomm.h"


const int ERROR_THROW				= 0x81010000;

const int ERROR_UNKNOWN				= 0x81011000;

const int ERROR_INITFGM_COINIT		= 0x81012001;
const int ERROR_INITFGM_FILEREND	= 0x81012002;
const int ERROR_INITFGM_QUERYINT	= 0x81012003;
const int ERROR_INITFGM_RUN			= 0x81012004;

const int ERROR_GETLEN_INIT			= 0x81013001;
const int ERROR_GETLEN_SETFILE		= 0x81013002;
const int ERROR_GETLEN_GETLEN		= 0x81013003;

const int ERROR_CONV_GRAPHBUILDER	= 0x81014001;
const int ERROR_CONV_ADDDEST		= 0x81014002;
const int ERROR_CONV_ADDCODEC		= 0x81014003;
const int ERROR_CONV_ADDOUTPUT		= 0x81014004;
const int ERROR_CONV_SETOUTPUT		= 0x81014005;
const int ERROR_CONV_RENDER			= 0x81014006;
const int ERROR_CONV_SETFMT			= 0x81014007;
const int ERROR_CONV_RUN			= 0x81014008;
const int ERROR_CONV_ADDPASER		= 0x81014009;
const int ERROR_CONV_CTRL			= 0x8101400A;
const int ERROR_CONV_ADD_AMRDECODER	= 0x8101400B;
const int ERROR_CONV_ADD_AMRSPLITTER	= 0x8101400C;
const int ERROR_CONV_ADD_NULLRENDER	= 0x8101400D;
const int ERROR_CONV_ADD_LAVSPLITTER	= 0x8101400E;

// mpg2wav error code
const int ERROR_CDSHOW_GET_AVRENDER	= 0x81015001;
const int ERROR_CDSHOW_NOAUDIO		= 0x81015002;
const int ERROR_CDSHOW_VIDEO_NUKEUP	= 0x81015003;
const int ERROR_CDSHOW_CONNECT_SPLITTER	= 0x81015004;
const int ERROR_CDSHOW_ADD_ACM			= 0x81015005;
const int ERROR_CDSHOW_CONNECT_ACM		= 0x81015006;
const int ERROR_CDSHOW_ADD_WAVEDEST		= 0x81015007;
const int ERROR_CDSHOW_CONNECT_WAVEDEST	= 0x81015008;
const int ERROR_GET_FileSink		= 0x81015009;
const int ERROR_SetFileName			= 0x8101500A;
const int ERROR_SetFilterFormat		= 0x8101500B;
const int ERROR_GET_MediaControl	= 0x8101500C;
const int ERROR_MediaControl_Run	= 0x8101500D;
const int ERROR_GET_MediaEvent		= 0x8101500E;
const int ERROR_CDSHOW_ADD_FILEWRITER		= 0x81015010;
const int ERROR_CDSHOW_CONNECT_FILEWRITER	= 0x81015011;



HRESULT AddFilterByClsid( IGraphBuilder *pGraph, LPCWSTR wszName, const GUID& clsid, IBaseFilter **ppF )
{
	*ppF = NULL;
	HRESULT hr = CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER,
		IID_IBaseFilter, (void**)ppF);
	if (S_OK != hr)
	{
		return hr;
	}
	hr = pGraph->AddFilter((*ppF), wszName);
	return hr;
}

bool SetFilterFormat( AM_MEDIA_TYPE* pStreamFormat, IBaseFilter* pBaseFilter )
{
	HRESULT hr;
	bool retVal = false;

	// Pin enumeration
	IEnumPins* pEnumPins = NULL;
	hr = pBaseFilter->EnumPins(&pEnumPins);
	if (FAILED(hr)) {
		// ERROR HERE
		return false;
	}

	IPin* pPin = NULL;
	while (pEnumPins->Next(1, &pPin, 0) == S_OK) {
		PIN_DIRECTION sDirection;
		pPin->QueryDirection(&sDirection);
		// Output Pin ?
		if (sDirection == PINDIR_OUTPUT) {
			IAMStreamConfig* pStreamConfig = NULL;
			hr = pPin->QueryInterface(IID_IAMStreamConfig, (void**) &pStreamConfig);
			if (SUCCEEDED(hr)) {
				hr = pStreamConfig->SetFormat(pStreamFormat);
				if (SUCCEEDED(hr)) {
					retVal = true;
				}
				pStreamConfig->Release();
			}
		}
		pPin->Release();
	}

	// Free memory
	pEnumPins->Release();

	return retVal;
}


//connect stream with mux and through pCompressor
//only used by mkx,avi,mp3,ogg converter
HRESULT ConnectToMuxer(IGraphBuilder* pGraphBuild, 
					   IBaseFilter* pRender,
					   IBaseFilter* pCodec,
					   REFCLSID clsid,
					   IBaseFilter** ppMuxer)
{
	ASSERT(pRender!=NULL);

	HRESULT hr = S_OK;

	//find pin connected with Renderer, connect this pin with Compressor's Input Pin
	CComPtr<IPin> spOutPin = NULL;
	hr = CDShow::FindConnectedPin(pRender,&spOutPin);
	if(FAILED(hr))
		return hr;

	//remove Renderer
	hr = CDShow::RemoveFilter(pGraphBuild,pRender);
	if(FAILED(hr))
		return hr;

	//connect with compressor and connect compressor with muxer
	IBaseFilter* pMuxer = NULL;
	if(pCodec!=NULL)
	{
		hr = CDShow::ConnectFilters(pGraphBuild,spOutPin,pCodec);
		if(FAILED(hr))
			return hr;

		hr = CDShow::AddFilterByClsid(pGraphBuild,clsid,NULL,&pMuxer);
		if(FAILED(hr))
			return hr;
		hr = CDShow::ConnectFilters(pGraphBuild,pCodec,pMuxer);
	}
	//connect with muxer directly
	else
	{
		hr = CDShow::AddFilterByClsid(pGraphBuild,clsid,NULL,&pMuxer);
		if(FAILED(hr))
			return hr;
		hr = CDShow::ConnectFilters(pGraphBuild,spOutPin,pMuxer);
	}

	if(ppMuxer!=NULL)
	{
		*ppMuxer = pMuxer;
	}
	return hr;
}
