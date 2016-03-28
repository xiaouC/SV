#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include <vector>
#include "SurfaceTexStatic.h"

struct TexInfo
{
	CString strTextureName;
	CImage* pImage;
	int origin_x;
	int origin_y;
	int origin_w;
	int origin_h;
};

// CTexStatic
class CSelectSurfaceTextureDlg;
class CTexStatic : public CStatic
{
	friend class CSelectSurfaceTextureDlg;
	DECLARE_DYNAMIC(CTexStatic)
public:
	CTexStatic();
	virtual ~CTexStatic();

protected:
	CBrush m_kWhiteBrush;
	CBrush m_kRedBrush;

	CSelectSurfaceTextureDlg* m_pSelDlg;

	int m_nPos;
	int m_nSelIndex;
	std::vector<TexInfo> m_vecTexStatic;

	BOOL m_bSelFlag;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

// CSelectSurfaceTextureDlg 对话框
class CSelectSurfaceTextureDlg : public CDialogEx
{
	friend class CTexStatic;
	DECLARE_DYNAMIC(CSelectSurfaceTextureDlg)
public:
	CSelectSurfaceTextureDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSelectSurfaceTextureDlg();
	
	CString getCurrentSelectedTexFile();

// 对话框数据
	enum { IDD = IDD_SELECT_SURFACE_TEXTURE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

protected:
	CScrollBar m_kScrollBar;
	CTexStatic m_kStatic;
public:
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
