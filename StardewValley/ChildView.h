
// ChildView.h : CChildView 类的接口
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

// CChildView 窗口
class TLSeamlessMap;
class TLMapBlock;
class CMainFrame;
class CChildView : public CWnd
{
	friend class CMainFrame;
// 构造
public:
	CChildView();

// 特性
public:

// 操作
public:
    BOOL newSeamlessMap( const CString& strFileName, const CString& strBlockName, int nBlockRow, int nBlockCol, int nGridWidth, int nGridHeight, const CString& strMaterial );
	void openSeamlessMap( CString strFileName );
	BOOL addSpriteByDrop( COleDataObject* pDataObject, CPoint pt, BOOL bTestOnly );

// 重写
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CChildView();

protected:
	AppDelegate* m_pAppDelegate;

	CMyDropTarget m_kOleTarget;

	HACCEL   m_hAccel;

    CPoint m_kNewBlockPoint;

	// 生成的消息映射函数
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
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnSeamlessmapNewblock();
};

