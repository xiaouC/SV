// SelectSurfaceTextureDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "StardewValley.h"
#include "SelectSurfaceTextureDlg.h"
#include "afxdialogex.h"

// CTexStatic
IMPLEMENT_DYNAMIC(CTexStatic, CStatic)
CTexStatic::CTexStatic()
: m_nPos(0)
, m_nSelIndex(0)
, m_bSelFlag(FALSE)
{
}

CTexStatic::~CTexStatic()
{
	for( int i=0; i < (int)m_vecTexStatic.size(); ++i )
		delete m_vecTexStatic[i].pImage;
}

BEGIN_MESSAGE_MAP(CTexStatic, CStatic)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

// CTexStatic 消息处理程序
int CTexStatic::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	m_kWhiteBrush.CreateSolidBrush( RGB( 255, 255, 255 ) );
	m_kRedBrush.CreateSolidBrush( RGB( 255, 0, 0 ) );

	return 0;
}
void CTexStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CStatic::OnPaint()

	CRect rect;
	GetClientRect( &rect );

	int isdc = dc.SaveDC();
	CRgn itemRegion;
	itemRegion.CreateRectRgnIndirect( &rect );
	dc.SelectClipRgn( &itemRegion );

	dc.FillRect( &rect, &m_kWhiteBrush );

	//dc.SetStretchBltMode( HALFTONE ); //防止缩小图片失真

	CPen redPen( PS_SOLID, 2, RGB( 255, 0, 0 ) );    //定义画笔
	CPen* pOldPen = dc.SelectObject( &redPen );

	for( int i=0; i < (int)m_vecTexStatic.size(); ++i )
	{
		TexInfo& ti = m_vecTexStatic[i];

		CRect rc;
		rc.left = ti.origin_x;
		rc.right = rc.left + ti.origin_w;
		rc.top = ti.origin_y - m_nPos;
		rc.bottom = rc.top + ti.origin_h;

		if( m_nSelIndex == i )
		{
			CRect selRect;
			selRect.left = rc.left - 2;
			selRect.right = rc.right + 2;
			selRect.top = rc.top - 2;
			selRect.bottom = rc.bottom + 2;
			dc.FillRect( &selRect, &m_kRedBrush );
		}

		ti.pImage->Draw( dc, rc );
	}
	
	dc.SelectObject( pOldPen );

	itemRegion.DeleteObject();
	dc.RestoreDC( isdc );
}

void CTexStatic::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bSelFlag = TRUE;

	SetFocus();

	CStatic::OnLButtonDown(nFlags, point);
}

void CTexStatic::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if( m_bSelFlag )
	{
		m_bSelFlag = FALSE;

		for( int i=0; i < (int)m_vecTexStatic.size(); ++i )
		{
			TexInfo& ti = m_vecTexStatic[i];

			CRect rc;
			rc.left = ti.origin_x;
			rc.right = rc.left + ti.origin_w;
			rc.top = ti.origin_y - m_nPos;
			rc.bottom = rc.top + ti.origin_h;

			if( rc.PtInRect( point ) )
			{
				m_nSelIndex = i;

				Invalidate();

				break;
			}
		}
	}

	CStatic::OnLButtonUp(nFlags, point);
}

void CTexStatic::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	for( int i=0; i < (int)m_vecTexStatic.size(); ++i )
	{
		TexInfo& ti = m_vecTexStatic[i];

		CRect rc;
		rc.left = ti.origin_x;
		rc.right = rc.left + ti.origin_w;
		rc.top = ti.origin_y - m_nPos;
		rc.bottom = rc.top + ti.origin_h;

		if( rc.PtInRect( point ) )
		{
			m_nSelIndex = i;

			Invalidate();

			m_pSelDlg->OnBnClickedOk();

			break;
		}
	}

	CStatic::OnLButtonDblClk(nFlags, point);
}

BOOL CTexStatic::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int nMin,nMax;
	m_pSelDlg->m_kScrollBar.GetScrollRange( &nMin, &nMax );

	m_nPos -= zDelta / 5;
	if( m_nPos < nMin )
		m_nPos = nMin;

	if( m_nPos > nMax )
		m_nPos = nMax;

	m_pSelDlg->m_kScrollBar.SetScrollPos( m_nPos );

	Invalidate();

	return CStatic::OnMouseWheel(nFlags, zDelta, pt);
}

// CSelectSurfaceTextureDlg 对话框
IMPLEMENT_DYNAMIC(CSelectSurfaceTextureDlg, CDialogEx)
CSelectSurfaceTextureDlg::CSelectSurfaceTextureDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectSurfaceTextureDlg::IDD, pParent)
{
}

CSelectSurfaceTextureDlg::~CSelectSurfaceTextureDlg()
{
}

void CSelectSurfaceTextureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_SCROLLBAR1, m_kScrollBar);
	//DDX_Control(pDX, IDC_STATIC_VIEW, m_kStatic);
}

BEGIN_MESSAGE_MAP(CSelectSurfaceTextureDlg, CDialogEx)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDOK, &CSelectSurfaceTextureDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSelectSurfaceTextureDlg::OnBnClickedCancel)
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CSelectSurfaceTextureDlg 消息处理程序
int CSelectSurfaceTextureDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CRect rectDummy( 10, 26, 860, 486 );
	const DWORD dwStyle = WS_CHILD | WS_VISIBLE | SS_NOTIFY | WS_BORDER;
	if( !m_kStatic.Create( "", dwStyle, rectDummy, this, 1111 ) )
		return -1;

	m_kStatic.m_pSelDlg = this;

	int nOffsetX = 10, nOffsetY = 10;
	int nRowHeight = 200, nColWidth = 200;
	int nColCount = 4;
	int nInterval = 10;

	int nCount = ::GetPrivateProfileInt( _T("init"), _T("count"), 0, _T("./Editor/MapBlock.ini") );
	for( int i=1; i <= nCount; ++i )
	{
		CString strKey;
		strKey.Format( _T("surfaceTex%d"), i );

		CString strTextureName;
		GetPrivateProfileString( _T("init"), strKey, _T(""), strTextureName.GetBuffer( MAX_PATH ), MAX_PATH, _T("./Editor/MapBlock.ini") );

		if( strTextureName.Compare( m_strInitSelectedFileName ) == 0 )
			m_kStatic.m_nSelIndex = i - 1;

		int nX = ( i - 1 ) % nColCount * ( nColWidth + nInterval ) + nOffsetX;
		int nY = ( i - 1 ) / nColCount * ( nRowHeight + nInterval ) + nOffsetY;

		CString strTemp;
		strTemp.Format( "images/%s", strTextureName );

		TexInfo ti;
		ti.strTextureName = strTextureName;
		ti.pImage = new CImage;
		ti.pImage->Load( strTemp );
		ti.origin_x = nX;
		ti.origin_y = nY;
		ti.origin_w = nColWidth;
		ti.origin_h = nRowHeight;
		m_kStatic.m_vecTexStatic.push_back( ti );
	}

	// scroll bar range
	CRect rectDummy1( 865, 26, 885, 486 );
	const DWORD dwStyle1 = WS_CHILD | WS_VISIBLE | SBS_VERT;
	m_kScrollBar.Create( dwStyle1, rectDummy1, this, 1000 );

	int nRow = nCount / nColCount;
	if( nRow * nColCount < nCount )
		nRow = nRow + 1;

	int nScrollMin = 0;
	int nScrollMax = ( nRow - 2 ) * ( nRowHeight + nInterval ) + nOffsetY;
	if( nScrollMax < 0 )
		nScrollMax = 0;
	m_kScrollBar.SetScrollRange( nScrollMin, nScrollMax );

	return 0;
}

CString CSelectSurfaceTextureDlg::getCurrentSelectedTexFile()
{
	if( m_kStatic.m_nSelIndex >= 0 && m_kStatic.m_nSelIndex < (int)m_kStatic.m_vecTexStatic.size() )
	{
		TexInfo& ti = m_kStatic.m_vecTexStatic[m_kStatic.m_nSelIndex];
		return ti.strTextureName;
	}

	return "";
}

void CSelectSurfaceTextureDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}

void CSelectSurfaceTextureDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

void CSelectSurfaceTextureDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int nMin,nMax;
	pScrollBar->GetScrollRange( &nMin, &nMax );

	int nTemp1 = 0;
	int nTemp2 = 0;

	nTemp1 = pScrollBar->GetScrollPos();
	switch (nSBCode)
	{
	case SB_THUMBPOSITION:
		pScrollBar->SetScrollPos( nPos );
		break;
	case SB_LINEUP:
		nTemp2 = ( nMax - nMin ) / 10;
		if( ( nTemp1 - nTemp2 ) > nMin)
		{
			nTemp1 -= nTemp2;
		}
		else
		{
			nTemp1 = nMin;
		}
		pScrollBar->SetScrollPos( nTemp1 );

		m_kStatic.m_nPos = nTemp1;
		m_kStatic.Invalidate();

		break;
	case SB_LINEDOWN:
		nTemp2 = ( nMax - nMin ) / 10;
		if ( (nTemp1+nTemp2) > nMax)
		{
			nTemp1 = nMax;
		}
		else
		{
			nTemp1 += nTemp2;
		}
		pScrollBar->SetScrollPos( nTemp1 );

		m_kStatic.m_nPos = nTemp1;
		m_kStatic.Invalidate();

		break;
	}

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}



void CSelectSurfaceTextureDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_kStatic.SetFocus();

	CDialogEx::OnLButtonDown(nFlags, point);
}