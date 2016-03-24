
// ChildView.h : CChildView ��Ľӿ�
//


#pragma once
#include "AppDelegate.h"
#include "base_nodes\CCNode.h"
#include "CCEGLView.h"

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
class CChildView : public CWnd
{
// ����
public:
	CChildView();

// ����
public:

// ����
public:
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
	cocos2d::CCNode* m_pMainNode;

	CMyDropTarget m_kOleTarget;

	// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
};

