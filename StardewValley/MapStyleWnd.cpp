
#include "stdafx.h"

#include "OutputWnd.h"
#include "Resource.h"
#include "MainFrm.h"

#include "MC/MCLoader.h"
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

	// ����ѡ�����:
	if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1))
	{
		TRACE0("δ�ܴ������ѡ�����\n");
		return -1;      // δ�ܴ���
	}

	return 0;
}

void CMapStyleWnd::initMapDetailWndList()
{
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	int nMapDetailWndID = 1;
	int nMapDetailIndex = 0;
	const DWORD dwStyle = WS_CHILD | WS_VISIBLE ;

	std::map< std::string, std::list<std::string> > mapStyleTexFileNames;
	MCLoader::sharedMCLoader()->getStyleTextureFileNameList( "mapStyle", mapStyleTexFileNames );

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
			TRACE0("δ�ܴ����ر���ͼ����\n");
			return;      // δ�ܴ���
		}

		// ���б��ڸ��ӵ�ѡ�:
		m_wndTabs.AddTab( pMapDetail, strMapStyleName, (UINT)nMapDetailIndex++ );

		// ǰ׺
		CString strPrevKey;
		strPrevKey.Format( _T("msPrev%d"), i );

		CString strPrev;
		GetPrivateProfileString( _T("mapStyle"), strPrevKey, _T(""), strPrev.GetBuffer( MAX_PATH ), MAX_PATH, _T("./Editor/EditorConfig.ini") );

		// ����
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

	// ѡ��ؼ�Ӧ��������������:
	m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);

	Invalidate();
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
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// COutputList ��Ϣ�������
int CMapDetail::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
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
	MessageBox(_T("�������"));
}

void CMapDetail::OnEditClear()
{
	MessageBox(_T("������"));
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

void CMapDetail::adjustLayout()
{
	CRect rect;
	GetWindowRect( &rect );

	float nOffsetX = 0;
	float nOffsetY = 0;
	float nCurLineMaxHeight = 0;

	std::map<std::string,ImageInfo>::iterator iter = m_mapImages.begin();
	std::map<std::string,ImageInfo>::iterator iter_end = m_mapImages.end();
	for( ; iter != iter_end; ++iter )
	{
		if( nOffsetX + iter->second.nWidth > rect.Width() )
		{
			nOffsetX = 0;
			nOffsetY = nOffsetY + nCurLineMaxHeight;
		}

		iter->second.nOffsetX = nOffsetX;
		iter->second.nOffsetY = nOffsetY;

		nOffsetX = nOffsetX + iter->second.nWidth;
		if( iter->second.nHeight > nCurLineMaxHeight )
			nCurLineMaxHeight = iter->second.nHeight;
	}

	InvalidateRect( &rect,TRUE );
}

void CMapDetail::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	adjustLayout();
}

void CMapDetail::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CStatic::OnPaint()

	CRect rect;
	GetWindowRect( &rect );

	dc.FillRect( &rect,&m_bgBrush );

	std::map<std::string,ImageInfo>::iterator iter = m_mapImages.begin();
	std::map<std::string,ImageInfo>::iterator iter_end = m_mapImages.end();
	for( ; iter != iter_end; ++iter )
	{
		iter->second.pImage->Draw( dc, iter->second.nOffsetX, iter->second.nOffsetY );
	}
}

void CMapStyleWnd::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDockablePane::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CMapStyleWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDockablePane::OnLButtonUp(nFlags, point);
}



