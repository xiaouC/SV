
// ChildView.h : CChildView 类的接口
//


#pragma once
#include "AppDelegate.h"
#include "base_nodes\CCNode.h"
#include "CCEGLView.h"

// CChildView 窗口

class CChildView : public CWnd
{
// 构造
public:
	CChildView();

// 特性
public:

// 操作
public:

// 重写
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CChildView();

protected:
	AppDelegate* m_pAppDelegate;
	cocos2d::CCEGLView* m_pGLView;
	cocos2d::CCNode* m_pMainNode;

	// 生成的消息映射函数
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
};

