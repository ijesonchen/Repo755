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



	// �����洢��·��(����)
	CString w_strRootDirInput;
	// ���������·��
	CString w_strRootDirOutput;

	// ����ʶ���о���������, С�ڸ�ֵΪ�ǻ��� 
	unsigned w_nNoiseTh;

	unsigned w_nWorkerThread;

	// �����ȡ�������λms
	unsigned int w_nTaskReaderTimeSlice;
	// ��������ȡ������
	unsigned int w_nMaxReadRecord;

	// �Ա�ʶ�������ļ�
	CString w_strSexConfigFile;
	// �Ա���ֵ������Ϊ����
	float w_fSexThreshold;
	// ����run������ϸlog������debug
	unsigned w_bShowTaskRunDetail;
	// ���ƽӿ��ļ���
	CString w_strVpInterfaceFile;
	// �Ƿ��������ƽӿ�
	bool w_bVpInterface;
};
#endif // Param_h__
