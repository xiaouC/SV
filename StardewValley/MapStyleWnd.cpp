
#include "stdafx.h"

#include "OutputWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "MapStyleWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputBar

CMapStyleWnd::CMapStyleWnd()
{
}

CMapStyleWnd::~CMapStyleWnd()
{
	std::map<CString, CMapDetail*>::iterator iter = m_mapMapDetails.begin();
	std::map<CString, CMapDetail*>::iterator iter_end = m_mapMapDetails.end();
	for( ; iter != iter_end; ++iter )
		delete iter->second;
}

BEGIN_MESSAGE_MAP(CMapStyleWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_KEYUP()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

int CMapStyleWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建选项卡窗口:
	if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1))
	{
		TRACE0("未能创建输出选项卡窗口\n");
		return -1;      // 未能创建
	}

	return 0;
}

void CMapStyleWnd::initMapDetailWndList()
{
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	int nMapDetailWndID = 1;
	int nMapDetailIndex = 0;
	const DWORD dwStyle = WS_CHILD | WS_VISIBLE | SS_NOTIFY ;

	int nStyleNum = ::GetPrivateProfileInt( _T("mapStyle"), _T("styleNum"), 0, _T("./Editor/EditorConfig.ini") );
	for( int i=1; i <= nStyleNum; ++i )
	{
		CString strKey;
		strKey.Format( _T("msName%d"), i );

		CString strMapStyleName;
		GetPrivateProfileString( _T("mapStyle"), strKey, _T(""), strMapStyleName.GetBuffer( MAX_PATH ), MAX_PATH, _T("./Editor/EditorConfig.ini") );

		++nMapDetailWndID;
		CMapDetail* pMapDetail = new CMapDetail;
		if( !pMapDetail->Create( strMapStyleName, dwStyle, rectDummy, &m_wndTabs, nMapDetailWndID ) )
		{
			TRACE0("未能创建地表贴图窗口\n");
			return;      // 未能创建
		}

		// 将列表窗口附加到选项卡:
		m_wndTabs.AddTab( pMapDetail, strMapStyleName, (UINT)nMapDetailIndex++ );

		// 前缀
		CString strPrevKey;
		strPrevKey.Format( _T("msPrev%d"), i );

		CString strPrev;
		GetPrivateProfileString( _T("mapStyle"), strPrevKey, _T(""), strPrev.GetBuffer( MAX_PATH ), MAX_PATH, _T("./Editor/EditorConfig.ini") );

		// 数量
		CString strNumKey;
		strNumKey.Format( _T("msNum%d"), i );
		
		int nNum = ::GetPrivateProfileInt( _T("mapStyle"), strNumKey, 0, _T("./Editor/EditorConfig.ini") );

		pMapDetail->initMapDetail( strPrev, nNum );

		m_mapMapDetails[strMapStyleName] = pMapDetail;
	}
}

void CMapStyleWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// 选项卡控件应覆盖整个工作区:
	m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);

	
}

void CMapStyleWnd::AdjustHorzScroll(CListBox& wndListBox)
{
	//CClientDC dc(this);
	//CFont* pOldFont = dc.SelectObject(&afxGlobalData.fontRegular);

	//int cxExtentMax = 0;

	//for (int i = 0; i < wndListBox.GetCount(); i ++)
	//{
	//	CString strItem;
	//	wndListBox.GetText(i, strItem);

	//	cxExtentMax = max(cxExtentMax, dc.GetTextExtent(strItem).cx);
	//}

	//wndListBox.SetHorizontalExtent(cxExtentMax);
	//dc.SelectObject(pOldFont);
}

/////////////////////////////////////////////////////////////////////////////
// CMapDetail

CMapDetail::CMapDetail()
{
}

CMapDetail::~CMapDetail()
{
	std::map<std::string,ImageInfo>::iterator iter = m_mapImages.begin();
	std::map<std::string,ImageInfo>::iterator iter_end = m_mapImages.end();
	for( ; iter != iter_end; ++iter )
	{
		delete iter->second.pImage;
	}
}

BEGIN_MESSAGE_MAP(CMapDetail, CStatic)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_VIEW_OUTPUTWND, OnViewOutput)
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// COutputList 消息处理程序
int CMapDetail::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	m_bgBrush.CreateSolidBrush( RGB( 255, 255, 255 ) );

	return 0;
}

void CMapDetail::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_OUTPUT_POPUP);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}

	SetFocus();
}

void CMapDetail::OnEditCopy()
{
	MessageBox(_T("复制输出"));
}

void CMapDetail::OnEditClear()
{
	MessageBox(_T("清除输出"));
}

void CMapDetail::OnViewOutput()
{
	CDockablePane* pParentBar = DYNAMIC_DOWNCAST(CDockablePane, GetOwner());
	CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());

	if (pMainFrame != NULL && pParentBar != NULL)
	{
		pMainFrame->SetFocus();
		pMainFrame->ShowPane(pParentBar, FALSE, FALSE, FALSE);
		pMainFrame->RecalcLayout();

	}
}

void CMapDetail::initMapDetail( const CString& strPrev, int nNum )
{
	for( int i=1; i <= nNum; ++i )
	{
		CString strTemp;
		strTemp.Format( "%s_%d.png", strPrev, i );

		CString strPath;
		strPath.Format( "./Editor/mapStyle/%s_%d.png", strPrev, i );

		std::string strFileName = strTemp.GetBuffer();

		CImage* pImage = new CImage;
		pImage->Load( strPath );

		ImageInfo ii;
		ii.pImage = pImage;
		ii.nWidth = pImage->GetWidth();
		ii.nHeight = pImage->GetHeight();
		ii.nOffsetX = 0;
		ii.nOffsetY = 0;

		m_mapImages[strFileName] = ii;
	}

	adjustLayout();
}

const int nInterval = 5;
void CMapDetail::adjustLayout()
{
	CRect rect;
	GetWindowRect( &rect );

	float nOffsetX = nInterval;
	float nOffsetY = nInterval;
	float nCurLineMaxHeight = 0;

	std::map<std::string,ImageInfo>::iterator iter = m_mapImages.begin();
	std::map<std::string,ImageInfo>::iterator iter_end = m_mapImages.end();
	for( ; iter != iter_end; ++iter )
	{
		if( nOffsetX + iter->second.nWidth + nInterval > rect.Width() )
		{
			nOffsetX = nInterval;
			nOffsetY = nOffsetY + nCurLineMaxHeight + nInterval * 2;
			nCurLineMaxHeight = 0;
		}

		iter->second.nOffsetX = nOffsetX;
		iter->second.nOffsetY = nOffsetY;

		nOffsetX = nOffsetX + iter->second.nWidth + nInterval * 2;
		if( iter->second.nHeight > nCurLineMaxHeight )
			nCurLineMaxHeight = iter->second.nHeight;
	}

	Invalidate();
}

void CMapDetail::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	adjustLayout();
}

void CMapDetail::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CStatic::OnPaint()

	CRect rect;
	GetClientRect( &rect );

	dc.FillRect( &rect, &m_bgBrush );

	CPen bluePen( PS_SOLID, 2, RGB( 0, 0, 255 ) );    //定义画笔
	CPen* pOldPen = dc.SelectObject( &bluePen );

	int nHalfInterval = nInterval / 2;
	std::map<std::string,ImageInfo>::iterator iter = m_mapImages.begin();
	std::map<std::string,ImageInfo>::iterator iter_end = m_mapImages.end();
	for( ; iter != iter_end; ++iter )
	{
		iter->second.pImage->Draw( dc, iter->second.nOffsetX, iter->second.nOffsetY );

		if( m_strCurrentName.compare( iter->first ) != 0 )
		{
			dc.MoveTo( iter->second.nOffsetX - nHalfInterval, iter->second.nOffsetY - nHalfInterval );
			dc.LineTo( iter->second.nOffsetX - nHalfInterval, iter->second.nOffsetY + iter->second.nHeight + nHalfInterval );
			dc.LineTo( iter->second.nOffsetX + iter->second.nWidth + nHalfInterval, iter->second.nOffsetY + iter->second.nHeight + nHalfInterval );
			dc.LineTo( iter->second.nOffsetX + iter->second.nWidth + nHalfInterval, iter->second.nOffsetY - nHalfInterval );
			dc.LineTo( iter->second.nOffsetX - nHalfInterval, iter->second.nOffsetY - nHalfInterval );
		}
	}

	iter = m_mapImages.find( m_strCurrentName );
	if( iter != iter_end )
	{
		CPen redPen( PS_SOLID, 2, RGB( 255, 0, 0 ) );    //定义画笔
		dc.SelectObject( &redPen );

		dc.MoveTo( iter->second.nOffsetX - nHalfInterval, iter->second.nOffsetY - nHalfInterval );
		dc.LineTo( iter->second.nOffsetX - nHalfInterval, iter->second.nOffsetY + iter->second.nHeight + nHalfInterval );
		dc.LineTo( iter->second.nOffsetX + iter->second.nWidth + nHalfInterval, iter->second.nOffsetY + iter->second.nHeight + nHalfInterval );
		dc.LineTo( iter->second.nOffsetX + iter->second.nWidth + nHalfInterval, iter->second.nOffsetY - nHalfInterval );
		dc.LineTo( iter->second.nOffsetX - nHalfInterval, iter->second.nOffsetY - nHalfInterval );
	}

	dc.SelectObject( pOldPen );
}

void CMapStyleWnd::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDockablePane::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CMapStyleWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值


	CDockablePane::OnLButtonUp(nFlags, point);
}

void CMapDetail::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	

	CStatic::OnLButtonUp(nFlags, point);
}


void CMapDetail::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_strCurrentName.clear();

	std::map<std::string,ImageInfo>::iterator iter = m_mapImages.begin();
	std::map<std::string,ImageInfo>::iterator iter_end = m_mapImages.end();
	for( ; iter != iter_end; ++iter )
	{
		if( iter->second.nOffsetX <= point.x &&
			iter->second.nOffsetX + iter->second.nWidth >= point.x &&
			iter->second.nOffsetY <= point.y &&
			iter->second.nOffsetY + iter->second.nHeight >= point.y )
		{
			m_strCurrentName = iter->first;

			Invalidate();

			break;
		}
	}

	if( !m_strCurrentName.empty() )
	{
		CMyDataSource ds( "FlyFly" + m_strCurrentName );
		CMyDropSource dropSource( m_strCurrentName );
		ds.DoDragDrop( DROPEFFECT_COPY, NULL, &dropSource );
	}

	CStatic::OnLButtonDown(nFlags, point);
}
