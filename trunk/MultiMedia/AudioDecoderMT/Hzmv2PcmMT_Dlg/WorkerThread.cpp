#include "StdAfx.h"
#include "Hzmv2PcmMT_DlgDlg.h"
#include "WorkerThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

UINT CWorkerThread::TrainThreadFun( LPVOID pParam )
{
	return ((CWorkerThread *)pParam)->Run();
}

UINT CWorkerThread::Run()
{
	//线程循环，一个循环执行完一个训练任务。
	while (!w_bExit && !w_pDoc->w_bProcessQuit)//如果是关闭通知，退出
	{
		CSharePtr<ITask> spTrainTask;
		//中间文件缓冲区操作
		CSingleLock singLock(&w_pDoc->w_csTasks, TRUE);
		size_t nDqSize = w_pDoc->w_dqSpTasks.size();
		if ( nDqSize != 0 )
		{
			spTrainTask = w_pDoc->w_dqSpTasks.front();
			w_pDoc->w_dqSpTasks.pop_front();
		}
		else
		{
			break;
		}
		
		singLock.Unlock();

		
		if (!spTrainTask.IsValid())
		{	
			continue;
		}

		spTrainTask->WriteLog(this, nDqSize);
		

		if (w_pDoc->w_bProcessQuit)//如果是系统退出通知，退出
		{
			return 0;
		}

		int nRet = 0;
		while (true)
		{
			if (nRet = spTrainTask->Run(this))
			{
				CString strMsg;
				strMsg.Format(_T("线程%d--训练任务运行失败: 0x%x"), w_nID, nRet);
				w_pDoc->WriteMessage(strMsg);
			}
		}
	}
	if (!w_pDoc->w_bProcessQuit)
	{
//		AfxGetApp()->m_pMainWnd->PostMessage(WM_THREAD_EXIT, (WPARAM)w_nID);
		CString strMsg;
		strMsg.Format(_T("线程%d--线程退出"), w_nID);
		w_pDoc->WriteMessage(strMsg);
	}
	return 0;
}
