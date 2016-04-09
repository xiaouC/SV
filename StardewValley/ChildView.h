
// ChildView.h : CChildView ��Ľӿ�
//
#pragma once
#include "AppDelegate.h"
#include "base_nodes\CCNode.h"
#include "CCEGLView.h"
#include "TLRunningScene.h"

class CChildView;
class CMyDropTarget : public COleDropTarget
{
public:
	CChildView* m_pChildView;
	DROPEFFECT m_dropAsEntered;

	virtual DROPEFFECT OnDragEnter(CWnd* pWnd,COleDataObject* pDO,DWORD dwKeyState,CPoint pt);
	virtual DROPEFFECT OnDragOver(CWnd* pWnd,COleDataObject* pDO,DWORD dwKeyState,CPoint pt);
	virtual DROPEFFECT OnDropEx(CWnd* pWnd,COleDataObject* pDO,DROPEFFECT dropDefault,DROPEFFECT dropList,CPoint pt);
};

// CChildView ����
class TLSeamlessMap;
class TLMapBlock;
class CMainFrame;
class CChildView : public CWnd
{
	friend class CMainFrame;
// ����
public:
	CChildView();

// ����
public:

// ����
public:
	void openSeamlessMap( CString strFileName );
	BOOL addSpriteByDrop( COleDataObject* pDataObject, CPoint pt, BOOL bTestOnly );

// ��д
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CChildView();

protected:
	AppDelegate* m_pAppDelegate;
	cocos2d::CCEGLView* m_pGLView;

	float m_fMainNodeScale;
	TLRunningScene* m_pMainNode;
	cocos2d::CCNode* m_pMainScaleNode;

	TLSeamlessMap* m_pSMNode;
	TLMapBlock* m_pEditMapBlock;
	cocos2d::CCSprite* m_pEditSprite;

	CMyDropTarget m_kOleTarget;

	HACCEL   m_hAccel;

protected:
	BOOL m_bDownFlag;
	BOOL m_bRotationFlag;
	float m_fLastX, m_fLastY;
	float m_fLastRotation;
	float m_fDownX, m_fDownY;
	BOOL convertPoint( const CPoint& point, float& ret_x, float& ret_y );
	BOOL convertPointToSM( const CPoint& point, float& ret_x, float& ret_y );
	BOOL convertPointToMB( const CPoint& point, float& ret_x, float& ret_y );

	// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnOpenSm();
	afx_msg void OnSaveSm();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnChildViewUp();
	afx_msg void OnChildViewDown();
	afx_msg void OnChildViewLeft();
	afx_msg void OnChildViewRight();
	afx_msg void OnChildViewDelete();
	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
};

