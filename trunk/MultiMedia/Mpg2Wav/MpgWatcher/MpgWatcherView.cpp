
// MpgWatcherView.cpp : CMpgWatcherView ���ʵ��
//

#include "stdafx.h"
#include "MpgWatcher.h"

#include "MpgWatcherDoc.h"
#include "MpgWatcherView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMpgWatcherView

IMPLEMENT_DYNCREATE(CMpgWatcherView, CEditView)

BEGIN_MESSAGE_MAP(CMpgWatcherView, CEditView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMpgWatcherView::OnFilePrintPreview)
	ON_MESSAGE(WM_WRITELOGTOTEXT, OnLogMessage)
END_MESSAGE_MAP()

// CMpgWatcherView ����/����

CMpgWatcherView::CMpgWatcherView()
{
	// TODO: �ڴ˴���ӹ������

}

CMpgWatcherView::~CMpgWatcherView()
{
}

BOOL CMpgWatcherView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	CWiiLog::SetMsgReceiver(this);//����LOG��Ϣ����
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CEditView::PreCreateWindow(cs);
}

// CMpgWatcherView ����

void CMpgWatcherView::OnDraw(CDC* /*pDC*/)
{
	CMpgWatcherDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CMpgWatcherView ��ӡ


void CMpgWatcherView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CMpgWatcherView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CMpgWatcherView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CMpgWatcherView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CMpgWatcherView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMpgWatcherView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CMpgWatcherView ���

#ifdef _DEBUG
void CMpgWatcherView::AssertValid() const
{
	CEditView::AssertValid();
}

void CMpgWatcherView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CMpgWatcherDoc* CMpgWatcherView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMpgWatcherDoc)));
	return (CMpgWatcherDoc*)m_pDocument;
}
#endif //_DEBUG


// CMpgWatcherView ��Ϣ�������
LRESULT CMpgWatcherView::OnLogMessage(WPARAM wParam,LPARAM lParam)
{
	std::auto_ptr<CString> apStrLogContent((CString *)lParam);
	ShowData(*apStrLogContent);
	return 1;
}

void CMpgWatcherView::ShowData(CString strLogContent)
{
	CString str;
	int nCharIndex;
	int nBackcount = 0;

	// Make sure existing text doesn't get too large.
	int len = GetEditCtrl().GetWindowTextLength();
	if (len >= 16384)
	{
		GetEditCtrl().GetWindowText(str);
		GetEditCtrl().SetWindowText(str.GetBuffer(len)+4097);
		len -= 4097;
	}

	// Handle backspace characters.
	str = strLogContent;
	while ((nCharIndex = str.Find(TCHAR(VK_BACK))) != -1)
	{
		if (nCharIndex == 0)
		{
			++nBackcount;
			str = str.Mid(nCharIndex+1);
		}
		else
			str = str.Left(nCharIndex-1) + str.Mid(nCharIndex+1);
	}

	// Set text insertion point according to number of backspaces
	// at the begining of strData.
	if (nBackcount>len)
		nBackcount = len;
	GetEditCtrl().SetSel(len-nBackcount,len);

	// Eliminate line feeds.
	while ((nCharIndex = str.Find(TCHAR('\n'))) != -1)
		str = str.Left(nCharIndex) + str.Mid(nCharIndex + 1);

	// Ensure all carriage returns have a line feed following.
	LPCTSTR pchStart = (LPCTSTR)str;
	LPTSTR pchIndex = (LPTSTR)(char*)(_tcschr(pchStart,(TCHAR)'\r'));

	while (pchIndex != NULL)
	{
		int nPos = (int)(pchIndex - pchStart);

		str = str.Left(nPos+1) + CString('\n') + str.Mid(nPos+1);

		// Above operation probably allocated a new string buffer.
		pchStart = (LPCTSTR)str;
		pchIndex = (LPTSTR)(char*)(_tcschr(pchStart + nPos + 1,(TCHAR)'\r'));

	}

	// Add filtered data to edit control.
	GetEditCtrl().ReplaceSel(str);
}