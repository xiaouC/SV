
// ChildView.cpp : CChildView ���ʵ��
//

#include "stdafx.h"
#include "StardewValley.h"
#include "ChildView.h"

#include "MainFrm.h"
#include "CCDirector.h"
#include "CCEGLView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CChildView ��Ϣ�������

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
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������
	
	// TODO: �ڴ˴������Ϣ����������
	
	// ��ҪΪ������Ϣ������ CWnd::OnPaint()
}

void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	cocos2d::CCEGLView* pGLView = cocos2d::CCDirector::sharedDirector()->getOpenGLView( "MainView" );
	if( pGLView != NULL )
	{
		pGLView->setFrameSize( cx, cy );

		cocos2d::CCSize kWinSizeInPoints = pGLView->getDesignResolutionSize();
        pGLView->setViewPortInPoints(0, 0, kWinSizeInPoints.width, kWinSizeInPoints.height);

		cocos2d::CCNode* pMainNode = ((CMainFrame*)AfxGetMainWnd())->getMainNode();
		pMainNode->setPosition( cx * 0.5f, cy * 0.5f );
	}
}
