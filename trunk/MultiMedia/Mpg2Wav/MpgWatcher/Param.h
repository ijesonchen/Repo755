#ifndef Param_h__
#define Param_h__

#define DEFAULT_UNSIGNED	0

#pragma once

class CParam
{
public:
	CParam() 
		: w_strIniFile(_T(""))
		, w_strWatcherDirectory(_T(""))
		, w_strOutputDirectory(_T(""))
		, w_strConverter(_T(""))
		, w_strInterfaceFile(_T(""))
		, w_strRootDirInput(_T(""))
		, w_strRootDirOutput(_T(""))
		, w_nWorkerThread(DEFAULT_UNSIGNED) 
		, w_nNoiseTh(DEFAULT_UNSIGNED)
		, w_nTaskReaderTimeSlice(0)
		, w_nMaxReadRecord(0)
		, w_strSexConfigFile(_T(""))
		, w_fSexThreshold(0)
		, w_bShowTaskRunDetail(0)
		, w_strVpInterfaceFile(_T(""))
		, w_bVpInterface(false)
	{
	}

	~CParam(){}
	bool ReadIniFile(const CString& strIniFile);
	bool IsValid();

	bool Reload(void);

	CString GetDetailInfo();

	bool WriteStartFileID(unsigned int nStartFIleID);

	CString w_strIniFile;

	CString w_strWatcherDirectory;
	CString w_strOutputDirectory;
	CString w_strConverter;
	CString w_strInterfaceFile;



	// 语音存储根路径(输入)
	CString w_strRootDirInput;
	// 语音输出根路径
	CString w_strRootDirOutput;

	// 话音识别判决长度门限, 小于该值为非话音 
	unsigned w_nNoiseTh;

	unsigned w_nWorkerThread;

	// 任务读取间隔，单位ms
	unsigned int w_nTaskReaderTimeSlice;
	// 单次最大读取任务数
	unsigned int w_nMaxReadRecord;

	// 性别识别配置文件
	CString w_strSexConfigFile;
	// 性别阈值，大于为男性
	float w_fSexThreshold;
	// 任务run函数详细log，用于debug
	unsigned w_bShowTaskRunDetail;
	// 声纹接口文件名
	CString w_strVpInterfaceFile;
	// 是否启用声纹接口
	bool w_bVpInterface;
};
#endif // Param_h__
