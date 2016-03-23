
// ChildView.cpp : CChildView 类的实现
//

#include "stdafx.h"
#include "StardewValley.h"
#include "ChildView.h"

#include "MainFrm.h"
#include "CCDirector.h"
#include "MC/MCLoader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
	m_pAppDelegate = NULL;
	m_pGLView = NULL;
	m_pMainNode = NULL;
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
	m_pAppDelegate = new AppDelegate;
	if( StartLua( CCLuaEngine::defaultEngine(), "main" ) != 0 )
	{
        CCLog( "error start lua main.lua" );
    }

	m_pGLView = cocos2d::CCEGLView::create( (int)m_hWnd );
	m_pGLView->setFrameSize( 0, 0 );
	m_pMainNode = cocos2d::CCNode::create();
	cocos2d::CCDirector::sharedDirector()->addOpenGLView( "MainView", m_pGLView, m_pMainNode );

	m_pMainNode->addChild( MCLoader::sharedMCLoader()->loadSprite( "mc/mapStyle_1.png" ) );

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
