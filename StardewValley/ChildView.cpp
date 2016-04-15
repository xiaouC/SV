
// ChildView.cpp : CChildView 类的实现
//

#include "stdafx.h"
#include "StardewValley.h"
#include "ChildView.h"

#include "MainFrm.h"
#include "CCDirector.h"
#include "MC/MCLoader.h"
#include "MC/AssetsManager.h"
#include "Map/TLMapBlock.h"
#include "Map/TLSeamlessMap.h"
#include "NewMapBlockDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMyDropTarget
DROPEFFECT CMyDropTarget::OnDragEnter( CWnd* pWnd, COleDataObject* pDO, DWORD dwKeyState, CPoint pt )
{
	if( m_pChildView->addSpriteByDrop( pDO, pt, TRUE ) )
		return m_dropAsEntered = DROPEFFECT_COPY | DROPEFFECT_MOVE;
	else
		return m_dropAsEntered = DROPEFFECT_NONE;
}

DROPEFFECT CMyDropTarget::OnDragOver( CWnd* pWnd, COleDataObject* pDO, DWORD dwKeyState, CPoint pt )
{
	return m_dropAsEntered;
}

DROPEFFECT CMyDropTarget::OnDropEx( CWnd* pWnd, COleDataObject* pDO, DROPEFFECT dropDefault, DROPEFFECT dropList, CPoint pt )
{
	if( m_pChildView->addSpriteByDrop( pDO, pt, FALSE ) )
		return DROPEFFECT_COPY | DROPEFFECT_MOVE;

	return DROPEFFECT_NONE;
}

// CChildView
CChildView::CChildView()
{
	m_pAppDelegate = NULL;
	m_fMainNodeScale = 1.0f;
	m_pSMNode = NULL;
	m_pEditMapBlock = NULL;
	m_pEditNode = NULL;

	m_bDownFlag = FALSE;
	m_bMoveSMFlag = FALSE;
	m_kLastPoint.x = 0;
	m_kLastPoint.y = 0;

	m_bRotationFlag = FALSE;
	m_fLastX = 0.0f;
	m_fLastY = 0.0f;
	m_fLastRotation = 0.0f;
	m_fDownX = 0.0f;
	m_fDownY = 0.0f;
}

CChildView::~CChildView()
{
}

BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_OPEN_SM, &CChildView::OnOpenSm)
	ON_COMMAND(ID_SAVE_SM, &CChildView::OnSaveSm)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_CHILD_VIEW_UP, &CChildView::OnChildViewUp)
	ON_COMMAND(ID_CHILD_VIEW_DOWN, &CChildView::OnChildViewDown)
	ON_COMMAND(ID_CHILD_VIEW_LEFT, &CChildView::OnChildViewLeft)
	ON_COMMAND(ID_CHILD_VIEW_RIGHT, &CChildView::OnChildViewRight)
	ON_COMMAND(ID_CHILD_VIEW_DELETE, &CChildView::OnChildViewDelete)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_SEAMLESSMAP_NEWBLOCK, &CChildView::OnSeamlessmapNewblock)
END_MESSAGE_MAP()

// CChildView 消息处理程序
BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // 用于绘制的设备上下文
	
	// TODO: 在此处添加消息处理程序代码
	
	// 不要为绘制消息而调用 CWnd::OnPaint()
}

void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
    CString strArgs;
    strArgs.Format( "%d|%d", cx, cy );
    platform_callback( "CHILD_VIEW_ON_SIZE", strArgs.GetBuffer() );
}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	m_hAccel = ::LoadAccelerators( AfxGetInstanceHandle(), MAKEINTRESOURCE( IDR_CHILD_VIEW ) );

	m_kOleTarget.m_pChildView = this;
	m_kOleTarget.Register( this );

	m_pAppDelegate = new AppDelegate;

	CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pGLView = CCEGLView::sharedOpenGLView( m_hWnd );

    pDirector->setOpenGLView( pGLView );

	if( StartLua( CCLuaEngine::defaultEngine(), "Editor.main" ) != 0 )
	{
        CCLog( "error start lua editor main.lua" );
    }

	return 0;
}

void CChildView::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if( m_pAppDelegate != NULL )
	{
		CCDirector::sharedDirector()->end();
		CCDirector::sharedDirector()->mainLoop();

		//Sdelete m_pAppDelegate;
		delete m_pAppDelegate;
		m_pAppDelegate = NULL;
	}
}

BOOL CChildView::addSpriteByDrop( COleDataObject* pDataObject, CPoint pt, BOOL bTestOnly )
{
	if( m_pSMNode == NULL )
	{
		//AfxMessageBox( _T("请先打开地图！"), MB_OK );

		return FALSE;
	}

	if( pDataObject->IsDataAvailable( CF_TEXT ) )
	{
		HGLOBAL hg = pDataObject->GetGlobalData( CF_TEXT );
		LPVOID lpv = GlobalLock( hg );
		std::string strData = (LPCSTR)lpv;
		GlobalUnlock(hg);
		GlobalFree(hg);

		if( strData.find( "FlyFly" ) != std::string::npos )
		{
			if( !bTestOnly )
			{
				strData.erase( 0, 6 );

                CString strArgs;
                strArgs.Format( "%d|%d|%s", pt.x, pt.y, strData.c_str() );
                platform_callback( "ADD_SPRITE_DROP", strArgs.GetBuffer() );
			}

			return TRUE;
		}
	}

	return FALSE;
}

void CChildView::OnOpenSm()
{
	// TODO: 在此添加命令处理程序代码 
	char chFileName[MAX_PATH];   
	GetCurrentDirectory( MAX_PATH, chFileName); 

	CString strDir;
	strDir.Format( "%s/map/", chFileName );

	TCHAR szFilter[] = _T("地图文件(*.sm)|*.sm|所有文件(*.*)|*.*||"); 
    CFileDialog openFileDlg( TRUE, _T("sm"), NULL, OFN_HIDEREADONLY | OFN_READONLY | OFN_NOCHANGEDIR, szFilter, this );  
    openFileDlg.GetOFN().lpstrInitialDir = strDir;  
    if( openFileDlg.DoModal() == IDOK ) {  
        CString strFilePath = openFileDlg.GetPathName();
		strFilePath.Delete( 0, strlen( chFileName ) + 1 );

		openSeamlessMap( strFilePath );
    }
}

BOOL CChildView::newSeamlessMap( const CString& strFileName, const CString& strBlockName, int nBlockRow, int nBlockCol, int nGridWidth, int nGridHeight, const CString& strMaterial )
{
    CString strArgs;
    strArgs.Format( "%s|%s|%d|%d|%d|%d|%s", strFileName, strBlockName, nBlockRow, nBlockCol, nGridWidth, nGridHeight, strMaterial );
    platform_callback( "NEW_SEAMLESS_MAP", strArgs.GetBuffer() );

    return TRUE;
}

void CChildView::openSeamlessMap( CString strFileName )
{
    platform_callback( "OPEN_SEAMLESS_MAP", strFileName.GetBuffer() );
}

void CChildView::OnSaveSm()
{
	// TODO: 在此添加命令处理程序代码
    platform_callback( "SAVE_SEAMLESS_MAP", "" );

	AfxMessageBox( _T("保存成功!"), MB_OK );
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SetFocus();

    CString strArgs;
    strArgs.Format( "%d|%d|%s|%s", point.x, point.y, ( nFlags & MK_CONTROL ) ? "true" : "false", ( nFlags & MK_SHIFT ) ? "true" : "false" );
    platform_callback( "CHILD_VIEW_LBUTTON_DOWN", strArgs.GetBuffer() );

	CWnd::OnLButtonDown(nFlags, point);
}

void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
    CString strArgs;
    strArgs.Format( "%d|%d|%s|%s", point.x, point.y, ( nFlags & MK_CONTROL ) ? "true" : "false", ( nFlags & MK_SHIFT ) ? "true" : "false" );
    platform_callback( "CHILD_VIEW_LBUTTON_UP", strArgs.GetBuffer() );

	CWnd::OnLButtonUp(nFlags, point);
}

void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
    CString strArgs;
    strArgs.Format( "%d|%d|%s|%s", point.x, point.y, ( nFlags & MK_CONTROL ) ? "true" : "false", ( nFlags & MK_SHIFT ) ? "true" : "false" );
    platform_callback( "CHILD_VIEW_MOUSE_MOVE", strArgs.GetBuffer() );

	CWnd::OnMouseMove(nFlags, point);
}

BOOL CChildView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if( m_hAccel && ::TranslateAccelerator( m_hWnd, m_hAccel, pMsg ) )
		return TRUE;

	return CWnd::PreTranslateMessage(pMsg);
}

void CChildView::OnChildViewUp()
{
	// TODO: 在此添加命令处理程序代码
    platform_callback( "CHILD_VIEW_UP", "" );
}

void CChildView::OnChildViewDown()
{
	// TODO: 在此添加命令处理程序代码
    platform_callback( "CHILD_VIEW_DOWN", "" );
}

void CChildView::OnChildViewLeft()
{
	// TODO: 在此添加命令处理程序代码
    platform_callback( "CHILD_VIEW_LEFT", "" );
}

void CChildView::OnChildViewRight()
{
	// TODO: 在此添加命令处理程序代码
    platform_callback( "CHILD_VIEW_RIGHT", "" );
}

void CChildView::OnChildViewDelete()
{
	// TODO: 在此添加命令处理程序代码
    platform_callback( "CHILD_VIEW_DELETE", "" );
}

void CChildView::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if( zDelta > 0 )
	{
		if( nFlags & MK_CONTROL )
		{
            platform_callback( "EDIT_NODE_SCALE", "1.1" );
		}
		else
		{
            platform_callback( "MAIN_NODE_SCALE", "1.1" );
		}
	}
	else
	{
		if( nFlags & MK_CONTROL )
		{
            platform_callback( "EDIT_NODE_SCALE", "0.9" );
		}
		else
		{
            platform_callback( "MAIN_NODE_SCALE", "0.9" );
		}
	}
}

void CChildView::OnContextMenu(CWnd* pWnd, CPoint point)
{
    m_kNewBlockPoint = point;

	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_MENU_SEAMLESS_MAP, point.x, point.y, this, TRUE);

	SetFocus();
}


void CChildView::OnSeamlessmapNewblock()
{
	// TODO: 
	CNewMapBlockDlg dlg;
	if( dlg.DoModal() == IDOK )
	{
        CString strArgs;
        strArgs.Format( "%s|%d|%d|%s", dlg.m_strBlockName, m_kNewBlockPoint.x, m_kNewBlockPoint.y, dlg.m_strMaterial );
        platform_callback( "NEW_MAP_BLOCK", strArgs.GetBuffer() );
	}
}
