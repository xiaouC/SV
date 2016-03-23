
#pragma once
#include <map>
#include <string>
#include <atlimage.h>

/////////////////////////////////////////////////////////////////////////////
// CMapDetail 窗口

class CMapDetail : public CStatic
{
// 构造
public:
	CMapDetail();

	void initMapDetail( const CString& strPrev, int nNum );

// 实现
public:
	virtual ~CMapDetail();

	void adjustLayout();

protected:
	CBrush m_bgBrush;

	struct ImageInfo {
		CImage* pImage;
		int nWidth;
		int nHeight;
		int nOffsetX;
		int nOffsetY;
	};
	std::map<std::string,ImageInfo> m_mapImages;

protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnViewOutput();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

class CMapStyleWnd : public CDockablePane
{
// 构造
public:
	CMapStyleWnd();

	void initMapDetailWndList();

// 特性
protected:
	CMFCTabCtrl	m_wndTabs;

	std::map<CString, CMapDetail*>	m_mapMapDetails;

protected:
	void AdjustHorzScroll(CListBox& wndListBox);

// 实现
public:
	virtual ~CMapStyleWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

