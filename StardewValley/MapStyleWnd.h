
#pragma once
#include <map>
#include <string>
#include <atlimage.h>

class CMyDataSource : public COleDataSource
{
public:
	CMyDataSource( const std::string& strData )
	{
		HGLOBAL hg = GlobalAlloc( GHND, strData.length() + 1 );
		LPVOID lpv = GlobalLock( hg );
		memmove( lpv, strData.c_str(), strData.length() + 1 );
		GlobalUnlock( hg );
		CacheGlobalData( CF_TEXT, hg, NULL );
	}
};

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
	CPen m_kPen;

	struct ImageInfo {
		CImage* pImage;
		int nWidth;
		int nHeight;
		int nOffsetX;
		int nOffsetY;
	};
	std::map<std::string,ImageInfo> m_mapImages;
	std::string m_strCurrentName;

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
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
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

