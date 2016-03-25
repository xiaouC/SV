
// ChildView.cpp : CChildView 类的实现
//

#include "stdafx.h"
#include "StardewValley.h"
#include "ChildView.h"

#include "MainFrm.h"
#include "CCDirector.h"
#include "MC/MCLoader.h"
#include "Map/TLMapBlock.h"
#include "Map/TLSeamlessMap.h"

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
	m_pGLView = NULL;
	m_pMainNode = NULL;
	m_pSMNode = NULL;
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONUP()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_OPEN_SM, &CChildView::OnOpenSm)
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
	if( m_pGLView != NULL )
	{
		m_pGLView->setFrameSize( cx, cy );

		cocos2d::CCSize kWinSizeInPoints = m_pGLView->getDesignResolutionSize();
        m_pGLView->setViewPortInPoints(0, 0, kWinSizeInPoints.width, kWinSizeInPoints.height);

		m_pMainNode->setPosition( cx * 0.5f, cy * 0.5f );
	}
}


void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CWnd::OnLButtonUp(nFlags, point);
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	m_kOleTarget.m_pChildView = this;
	m_kOleTarget.Register( this );

	m_pAppDelegate = new AppDelegate;
	if( StartLua( CCLuaEngine::defaultEngine(), "main" ) != 0 )
	{
        CCLog( "error start lua main.lua" );
    }

	m_pGLView = cocos2d::CCEGLView::create( (int)m_hWnd );
	m_pGLView->setFrameSize( 0, 0 );
	m_pMainNode = cocos2d::CCNode::create();
	cocos2d::CCDirector::sharedDirector()->addOpenGLView( "MainView", m_pGLView, m_pMainNode );

	//m_pMainNode->addChild( MCLoader::sharedMCLoader()->loadSprite( "mc/mapStyle_1.png" ) );

	return 0;
}


void CChildView::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	CCDirector::sharedDirector()->end();
	CCDirector::sharedDirector()->mainLoop();

	//Sdelete m_pAppDelegate;
	m_pAppDelegate = NULL;
	m_pGLView = NULL;
	m_pMainNode = NULL;
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
				CRect rect;
				GetWindowRect( &rect );

				float x = pt.x - rect.Width() * 0.5f;
				float y = rect.Height() * 0.5f - pt.y;

				float sm_x = 0.0f;
				float sm_y = 0.0f;
				m_pSMNode->getPosition( &sm_x, &sm_y );

				float fWorldX = x - sm_x;
				float fWorldY = y - sm_y;
				TLMapBlock* pMapBlock = m_pSMNode->getMapBlock( fWorldX, fWorldY );
				if( pMapBlock == NULL )
					return FALSE;

				strData.erase( 0, 6 );

				float mb_x = 0.0f;
				float mb_y = 0.0f;
				pMapBlock->getPosition( &mb_x, &mb_y );
				float fLocalX = fWorldX - mb_x;
				float fLocalY = fWorldY - mb_y;
				CCSprite* pSprite = pMapBlock->addSprite( strData, fLocalX, fLocalY );
				if( pSprite == NULL )
					return FALSE;

				m_pEditSprite = pSprite;
			}

			return TRUE;
		}
	}

	return FALSE;
}

void CChildView::OnOpenSm()
{
	// TODO: 在此添加命令处理程序代码
	TLSeamlessMap* pNewSMNode = TLSeamlessMap::create( "./map/FirstMap.sm", 0.0f, 0.0f );
	if( pNewSMNode != NULL )
	{
		m_pMainNode->addChild( pNewSMNode );

		if( m_pSMNode != NULL )
			m_pSMNode->removeFromParentAndCleanup( true );

		m_pSMNode = pNewSMNode;
	}
}
