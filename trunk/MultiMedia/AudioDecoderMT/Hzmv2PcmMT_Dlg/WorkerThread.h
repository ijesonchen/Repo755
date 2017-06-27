#pragma once

class CHzmv2PcmMT_DlgDlg;

class CWorkerThread
{
public:
	//Function
	CWorkerThread(CHzmv2PcmMT_DlgDlg* pDoc, int nID) : w_pDoc(pDoc), w_nID(nID), w_bExit(FALSE){}
	static UINT TrainThreadFun( LPVOID pParam );
	
	void SetExitFlag(BOOL bExit = TRUE)
	{
		InterlockedExchange(&w_bExit, bExit);
	}
	
	//Data
	CHzmv2PcmMT_DlgDlg *w_pDoc;
	int w_nID;
private:
	UINT Run();

	//Data
	volatile LONG w_bExit;
};
