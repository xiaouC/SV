
// ChildView.cpp : CChildView ���ʵ��
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
	m_fMainNodeScale = 1.0f;
	m_pMainNode = NULL;
	m_pMainScaleNode = NULL;
	m_pSMNode = NULL;
	m_pEditMapBlock = NULL;
	m_pEditSprite = NULL;

	m_fDownX = 0.0f;
	m_fDownY = 0.0f;

	m_nEditMode = EDIT_MODE_SELECT;
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

	// TODO:  �ڴ������ר�õĴ�������
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

	// TODO: �ڴ˴������Ϣ����������
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
		//AfxMessageBox( _T("���ȴ򿪵�ͼ��"), MB_OK );

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
	// TODO: �ڴ��������������� 
	char chFileName[MAX_PATH];   
	GetCurrentDirectory( MAX_PATH, chFileName); 

	CString strDir;
	strDir.Format( "%s/map/", chFileName );

	TCHAR szFilter[] = _T("��ͼ�ļ�(*.sm)|*.sm|�����ļ�(*.*)|*.*||"); 
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
	// TODO: �ڴ���������������
	if( m_pSMNode != NULL )
		m_pSMNode->save();
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	SetFocus();

	//m_bDownFlag = TRUE;
	//if( convertPoint( point, m_fDownX, m_fDownY ) )
	//{
	//	m_fLastRotation = m_pEditSprite->getRotation();
	//}

	CWnd::OnLButtonDown(nFlags, point);
}

const float g_fSelectedWidth = 100.0f;
const float g_fSelectedHeight = 100.0f;
void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//if( m_bDownFlag && ( nFlags & MK_CONTROL ) )
	//	rotatePoint( point );

	//m_bDownFlag = FALSE;

	CWnd::OnLButtonUp(nFlags, point);
}

void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//if( m_bDownFlag && ( nFlags & MK_CONTROL ) )
	//	rotatePoint( point );

	CWnd::OnMouseMove(nFlags, point);
}


BOOL CChildView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if( m_hAccel && ::TranslateAccelerator( m_hWnd, m_hAccel, pMsg ) )
		return TRUE;

	return CWnd::PreTranslateMessage(pMsg);
}


void CChildView::OnChildViewUp()
{
	// TODO: �ڴ���������������
	switch( m_nEditMode )
	{
	case EDIT_MODE_SELECT:
		break;
	case EDIT_MODE_TRANSLATION:
		if( m_pEditMapBlock != NULL )
		{
			m_pEditMapBlock->moveSprite( m_pEditSprite, 0.0f, 1.0f );
		}
		break;
	//case EDIT_MODE_ROTATION:
	//	if( m_pEditMapBlock != NULL )
	//	{
	//		m_pEditMapBlock->rotateSprite( m_pEditSprite, 1.0f );
	//	}
	//	break;
	//case EDIT_MODE_SCALE:
	//	if( m_pEditMapBlock != NULL )
	//	{
	//		m_pEditMapBlock->scaleSprite( m_pEditSprite, 1.1f );
	//	}
	//	break;
	}
}


void CChildView::OnChildViewDown()
{
	// TODO: �ڴ���������������
	switch( m_nEditMode )
	{
	case EDIT_MODE_SELECT:
		break;
	case EDIT_MODE_TRANSLATION:
		if( m_pEditMapBlock != NULL )
		{
			m_pEditMapBlock->moveSprite( m_pEditSprite, 0.0f, -1.0f );
		}
		break;
	//case EDIT_MODE_ROTATION:
	//	if( m_pEditMapBlock != NULL )
	//	{
	//		m_pEditMapBlock->rotateSprite( m_pEditSprite, 1.0f );
	//	}
	//	break;
	//case EDIT_MODE_SCALE:
	//	if( m_pEditMapBlock != NULL )
	//	{
	//		m_pEditMapBlock->scaleSprite( m_pEditSprite, 1.1f );
	//	}
	//	break;
	}
}


void CChildView::OnChildViewLeft()
{
	// TODO: �ڴ���������������
	switch( m_nEditMode )
	{
	case EDIT_MODE_SELECT:
		break;
	case EDIT_MODE_TRANSLATION:
		if( m_pEditMapBlock != NULL )
		{
			m_pEditMapBlock->moveSprite( m_pEditSprite, -1.0f, 0.0f );
		}
		break;
	case EDIT_MODE_ROTATION:
		if( m_pEditMapBlock != NULL )
		{
			m_pEditMapBlock->rotateSprite( m_pEditSprite, 1.0f );
		}
		break;
	//case EDIT_MODE_SCALE:
	//	if( m_pEditMapBlock != NULL )
	//	{
	//		m_pEditMapBlock->scaleSprite( m_pEditSprite, 1.1f );
	//	}
	//	break;
	}
}


void CChildView::OnChildViewRight()
{
	// TODO: �ڴ���������������
	switch( m_nEditMode )
	{
	case EDIT_MODE_SELECT:
		break;
	case EDIT_MODE_TRANSLATION:
		if( m_pEditMapBlock != NULL )
		{
			m_pEditMapBlock->moveSprite( m_pEditSprite, 1.0f, 0.0f );
		}
		break;
	case EDIT_MODE_ROTATION:
		if( m_pEditMapBlock != NULL )
		{
			m_pEditMapBlock->rotateSprite( m_pEditSprite, -1.0f );
		}
		break;
	//case EDIT_MODE_SCALE:
	//	if( m_pEditMapBlock != NULL )
	//	{
	//		m_pEditMapBlock->scaleSprite( m_pEditSprite, 1.1f );
	//	}
	//	break;
	}
}


void CChildView::OnChildViewDelete()
{
	// TODO: �ڴ���������������
	if( m_pEditSprite != NULL )
	{
		m_pEditMapBlock->removeSprite( m_pEditSprite );
	}
}

void CChildView::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if( zDelta > 0 )
	{
		if( m_nEditMode == EDIT_MODE_SCALE )
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
		if( m_nEditMode == EDIT_MODE_SCALE )
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

void CChildView::rotatePoint( const CPoint& point )
{
	float x = 0.0f, y = 0.0f;
	if( convertPoint( point, x, y ) )
	{
		float sx, sy;
		m_pEditSprite->getPosition( &sx, &sy );

		// Vector3 u( u1, u2, u3 )
		float u1 = m_fDownX - sx;
		float u2 = m_fDownY - sy;
		float u3 = 0.0f;

		// Vector3 v( v1, v2, v3 )
		float v1 = x - sx;
		float v2 = y - sy;
		float v3 = 0.0f;

		// u.cross( v )
		float cross_x = u2 * v3 - v2 * u3;
		float cross_y = u3 * v1 - v3 * u1;
		float cross_z = u1 * v2 - u2 * v1;

		float fAngle = abs( acos( ( u1 * v2 + u2 * v2 ) / sqrt( ( u1 * u1 + u2 * u2 ) * ( v1 * v1 + v2 * v2 ) ) ) );

		char chBuf[64];
		sprintf( chBuf, "%f\n", fAngle );
		OutputDebugString( chBuf );

		if( cross_z > 0.0f )
			fAngle = -fAngle;

		fAngle *= 180.0f / M_PI;
		m_pEditSprite->setRotation( m_fLastRotation + fAngle );
	}
}
