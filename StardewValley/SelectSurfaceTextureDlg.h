#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include <list>
#include "SurfaceTexStatic.h"

// CSelectSurfaceTextureDlg 对话框
class CSelectSurfaceTextureDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectSurfaceTextureDlg)

public:
	CSelectSurfaceTextureDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSelectSurfaceTextureDlg();

// 对话框数据
	enum { IDD = IDD_SELECT_SURFACE_TEXTURE_DLG };

protected:
	std::list<CSurfaceTexStatic*> m_listSurfaceTexWnd;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
protected:
	CScrollBar m_kScrollBar;
public:
	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
protected:
	CStatic m_kStatic;
};
