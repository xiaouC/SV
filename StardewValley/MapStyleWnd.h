
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

class CMyDropSource : public COleDropSource
{
public:
	CMyDropSource( const std::string& strFileName )
	{
		m_rc.SetRectEmpty();

		CString strPath;
		strPath.Format( "./Editor/mapStyle/%s", strFileName.c_str() );

		m_pImage = new CImage;
		m_pImage->Load( strPath );
	}

	virtual ~CMyDropSource()
	{
		::InvalidateRect( GetDesktopWindow(), &m_rc, TRUE );

		delete m_pImage;
	}

	virtual SCODE QueryContinueDrag( BOOL bEscapePressed, DWORD dwKeyState )
	{
		::InvalidateRect( GetDesktopWindow(), &m_rc, TRUE );

		if( bEscapePressed || ( dwKeyState & MK_RBUTTON ) != 0 )
		{
			m_bDragStarted = FALSE;

			return DRAGDROP_S_CANCEL;
		}

		if( ( dwKeyState & MK_LBUTTON ) == 0 )
			return m_bDragStarted ? DRAGDROP_S_DROP : DRAGDROP_S_CANCEL;

		CPoint point;
		GetCursorPos( &point );

		m_rc.left = point.x - 2;
		m_rc.right = m_rc.left + m_pImage->GetWidth() + 4;
		m_rc.top = point.y - 2;
		m_rc.bottom = m_rc.top + m_pImage->GetHeight() + 4;
		
		HDC hDC = ::GetDC( NULL );

		m_pImage->Draw( hDC, point.x - (int)( m_pImage->GetWidth() * 0.5 ), point.y - (int)( m_pImage->GetHeight() * 0.5 ) );

		ReleaseDC( NULL , hDC );

		return S_OK;
	}

private:
	CImage* m_pImage;
	CRect m_rc;
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

