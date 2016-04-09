
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

///////////////////////////////////////////////////////////////////////////
float vector_dot( ccVertex3F v1, ccVertex3F v2 )
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

ccVertex3F vector_cross( ccVertex3F v1, ccVertex3F v2 )
{
	ccVertex3F ret_v;

	ret_v.x = v1.y * v2.z - v2.y * v1.z;
	ret_v.y = v1.z * v2.x - v2.z * v1.x;
	ret_v.z = v1.x * v2.y - v2.x * v1.y;

	return ret_v;
}

float vector_distance( ccVertex3F v1, ccVertex3F v2 )
{
	return sqrt( ( v1.x * v1.x + v1.y * v1.y + v1.z * v1.z ) * ( v2.x * v2.x + v2.y * v2.y + v2.z * v2.z ) );
}

// CChildView
CChildView::CChildView()
{
	m_pAppDelegate = NULL;
	m_pGLView = NULL;
	m_fMainNodeScale = 1.0f;
	m_pMainNode = NULL;
	m_pMainScaleNode = NULL;
	m_pSMNode = NULL;
	m_pEditMapBlock = NULL;
	m_pEditSprite = NULL;

	m_bDownFlag = FALSE;
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
		m_pGLView->setDesignResolutionSize( cx, cy, kResolutionShowAll );
		//CCDirector::sharedDirector()->updateWinSizeInPoints();
		//CCDirector::sharedDirector()->setProjection( kCCDirectorProjection3D );

		m_pMainNode->setPosition( cx * 0.5f, cy * 0.5f );
	}
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
    m_pGLView = CCEGLView::sharedOpenGLView( m_hWnd );

    pDirector->setOpenGLView( m_pGLView );

	if( StartLua( CCLuaEngine::defaultEngine(), "main" ) != 0 )
	{
        CCLog( "error start lua main.lua" );
    }

	AssetsManager::sharedAssetsManager()->addSearchPath( "" );
    AssetsManager::sharedAssetsManager()->addSearchPath( "images/" );
    AssetsManager::sharedAssetsManager()->addSearchPath( "mc/" );

	MCLoader::sharedMCLoader()->loadIndexFile( "mc/anim.index", "mc/frames.index" );

	m_pGLView->setDesignResolutionSize( 1024, 768, kResolutionShowAll );
	pDirector->setDisplayStats( true );

	m_pMainNode = TLRunningScene::create();
	pDirector->runWithScene( m_pMainNode );

	m_pMainScaleNode = cocos2d::CCNode::create();
	m_pMainScaleNode->setScale( m_fMainNodeScale );
	m_pMainNode->addChild( m_pMainScaleNode );

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
		m_pGLView = NULL;
		m_pMainNode = NULL;
		m_pMainScaleNode = NULL;
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
				CRect rect;
				GetWindowRect( &rect );

				float x = ( pt.x - rect.Width() * 0.5f ) / m_fMainNodeScale;
				float y = ( rect.Height() * 0.5f - pt.y ) / m_fMainNodeScale;

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

				m_pEditMapBlock = pMapBlock;
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

void CChildView::openSeamlessMap( CString strFileName )
{
	std::string strTempFileName = strFileName.GetBuffer();
	TLSeamlessMap* pNewSMNode = TLSeamlessMap::create( strTempFileName, 0.0f, 0.0f );
	if( pNewSMNode != NULL )
	{
		m_pMainScaleNode->addChild( pNewSMNode );

		if( m_pSMNode != NULL )
			m_pSMNode->removeFromParentAndCleanup( true );

		m_pSMNode = pNewSMNode;
	}
}

void CChildView::OnSaveSm()
{
	// TODO: 在此添加命令处理程序代码
	if( m_pSMNode != NULL )
		m_pSMNode->save();
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SetFocus();

	m_bDownFlag = TRUE;
	if( convertPoint( point, m_fDownX, m_fDownY ) )
	{
		m_fLastX = m_pEditSprite->getPositionX();
		m_fLastY = m_pEditSprite->getPositionY();
		m_fLastRotation = m_pEditSprite->getRotation();
	}

	CWnd::OnLButtonDown(nFlags, point);
}

const float g_fSelectedWidth = 100.0f;
const float g_fSelectedHeight = 100.0f;
void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bDownFlag = FALSE;
	if( !m_bRotationFlag && m_pSMNode != NULL )
	{
		float x, y;
		if( convertPointToSM( point, x, y ) )
		{
			m_pEditMapBlock = m_pSMNode->getMapBlock( x, y );
			if( m_pEditMapBlock != NULL )
			{
				convertPointToMB( point, x, y );
				m_pEditSprite = m_pEditMapBlock->hitSprite( x, y );
			}
		}
	}

	if( m_bRotationFlag )
	{
		if( m_pEditMapBlock != NULL && m_pEditSprite != NULL )
			m_pEditMapBlock->rotateSprite( m_pEditSprite, m_pEditSprite->getRotation() );

		m_bRotationFlag = FALSE;
	}

	CWnd::OnLButtonUp(nFlags, point);
}

void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if( m_bDownFlag && ( nFlags & MK_CONTROL ) )
	{
		float fMoveX, fMoveY;
		if( convertPoint( point, fMoveX, fMoveY ) )
		{
			ccVertex3F v1;
			v1.x = m_fDownX - m_fLastX;
			v1.y = m_fDownY - m_fLastY;
			v1.z = 0.0f;

			ccVertex3F v2;
			v2.x = fMoveX - m_fLastX;
			v2.y = fMoveY - m_fLastY;
			v2.z = 0.0f;

			float fDot = vector_dot( v1, v2 );
			ccVertex3F vCross = vector_cross( v1, v2 );
			float fDis = vector_distance( v1, v2 );

			float angle = acos( fDot / fDis ) * 180.0 / M_PI;

			if( vCross.z > 0 )
				m_pEditSprite->setRotation( -angle );
			else
				m_pEditSprite->setRotation( angle );

			m_bRotationFlag = TRUE;
		}
	}

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
	if( m_pEditMapBlock != NULL && m_pEditSprite != NULL )
		m_pEditMapBlock->moveSprite( m_pEditSprite, 0.0f, 1.0f );
}

void CChildView::OnChildViewDown()
{
	// TODO: 在此添加命令处理程序代码
	if( m_pEditMapBlock != NULL && m_pEditSprite != NULL )
		m_pEditMapBlock->moveSprite( m_pEditSprite, 0.0f, -1.0f );
}

void CChildView::OnChildViewLeft()
{
	// TODO: 在此添加命令处理程序代码
	if( m_pEditMapBlock != NULL && m_pEditSprite != NULL )
		m_pEditMapBlock->moveSprite( m_pEditSprite, -1.0f, 0.0f );
}

void CChildView::OnChildViewRight()
{
	// TODO: 在此添加命令处理程序代码
	if( m_pEditMapBlock != NULL && m_pEditSprite != NULL )
		m_pEditMapBlock->moveSprite( m_pEditSprite, 1.0f, 0.0f );
}

void CChildView::OnChildViewDelete()
{
	// TODO: 在此添加命令处理程序代码
	if( m_pEditSprite != NULL )
	{
		m_pEditMapBlock->removeSprite( m_pEditSprite );
	}
}

void CChildView::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if( zDelta > 0 )
	{
		if( nFlags & MK_CONTROL )
		{
			m_pEditMapBlock->scaleSprite( m_pEditSprite, 1.1f );
		}
		else
		{
			m_fMainNodeScale *= 1.1f;
			m_pMainScaleNode->setScale( m_fMainNodeScale );
		}
	}
	else
	{
		if( nFlags & MK_CONTROL )
		{
			m_pEditMapBlock->scaleSprite( m_pEditSprite, 0.9f );
		}
		else
		{
			m_fMainNodeScale *= 0.9f;
			m_pMainScaleNode->setScale( m_fMainNodeScale );
		}
	}
}

BOOL CChildView::convertPoint( const CPoint& point, float& ret_x, float& ret_y )
{
	if( m_pEditMapBlock == NULL || m_pEditSprite == NULL )
		return FALSE;

	CRect rect;
	GetWindowRect( &rect );

	float x = ( point.x - rect.Width() * 0.5f ) / m_fMainNodeScale;
	float y = ( rect.Height() * 0.5f - point.y ) / m_fMainNodeScale;

	float sm_x = 0.0f;
	float sm_y = 0.0f;
	m_pSMNode->getPosition( &sm_x, &sm_y );

	float fWorldX = x - sm_x;
	float fWorldY = y - sm_y;

	float mb_x = 0.0f;
	float mb_y = 0.0f;
	m_pEditMapBlock->getPosition( &mb_x, &mb_y );

	ret_x = fWorldX - mb_x;
	ret_y = fWorldY - mb_y;

	return TRUE;
}

BOOL CChildView::convertPointToSM( const CPoint& point, float& ret_x, float& ret_y )
{
	if( m_pSMNode == NULL )
		return FALSE;

	CRect rect;
	GetWindowRect( &rect );

	float x = ( point.x - rect.Width() * 0.5f ) / m_fMainNodeScale;
	float y = ( rect.Height() * 0.5f - point.y ) / m_fMainNodeScale;

	float sm_x = 0.0f;
	float sm_y = 0.0f;
	m_pSMNode->getPosition( &sm_x, &sm_y );

	ret_x = x - sm_x;
	ret_y = y - sm_y;

	return TRUE;
}

BOOL CChildView::convertPointToMB( const CPoint& point, float& ret_x, float& ret_y )
{
	if( m_pSMNode == NULL )
		return FALSE;

	CRect rect;
	GetWindowRect( &rect );

	float x = ( point.x - rect.Width() * 0.5f ) / m_fMainNodeScale;
	float y = ( rect.Height() * 0.5f - point.y ) / m_fMainNodeScale;

	float sm_x = 0.0f;
	float sm_y = 0.0f;
	m_pSMNode->getPosition( &sm_x, &sm_y );

	float fWorldX = x - sm_x;
	float fWorldY = y - sm_y;

	TLMapBlock* pMapBlock = m_pSMNode->getMapBlock( fWorldX, fWorldY );
	if( pMapBlock == NULL )
		return FALSE;

	float mb_x = 0.0f;
	float mb_y = 0.0f;
	pMapBlock->getPosition( &mb_x, &mb_y );

	ret_x = fWorldX - mb_x;
	ret_y = fWorldY - mb_y;

	return TRUE;
}