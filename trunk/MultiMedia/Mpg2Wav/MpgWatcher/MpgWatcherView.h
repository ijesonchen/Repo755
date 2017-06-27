
// MpgWatcherView.h : CMpgWatcherView ��Ľӿ�
//


#pragma once


class CMpgWatcherView : public CEditView
{
protected: // �������л�����
	CMpgWatcherView();
	DECLARE_DYNCREATE(CMpgWatcherView)

// ����
public:
	CMpgWatcherDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CMpgWatcherView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnLogMessage(WPARAM wParam,LPARAM lParam);
	void ShowData(CString strLogContent);
};

#ifndef _DEBUG  // MpgWatcherView.cpp �еĵ��԰汾
inline CMpgWatcherDoc* CMpgWatcherView::GetDocument() const
   { return reinterpret_cast<CMpgWatcherDoc*>(m_pDocument); }
#endif

