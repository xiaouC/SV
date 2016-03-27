// SurfaceTexStatic.cpp : 实现文件
//

#include "stdafx.h"
#include "StardewValley.h"
#include "SurfaceTexStatic.h"

// CSurfaceTexStatic
IMPLEMENT_DYNAMIC(CSurfaceTexStatic, CStatic)
CSurfaceTexStatic::CSurfaceTexStatic( const CString& strTextureName )
: m_strTextureName(strTextureName)
{
	CString strTemp;
	strTemp.Format( "images/%s", strTextureName );

	m_kImage.Load( strTemp );
}

CSurfaceTexStatic::~CSurfaceTexStatic()
{
}

BEGIN_MESSAGE_MAP(CSurfaceTexStatic, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CSurfaceTexStatic 消息处理程序
void CSurfaceTexStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CStatic::OnPaint()

	CRect rect;
	GetClientRect( &rect );

	dc.SetStretchBltMode( HALFTONE ); //防止缩小图片失真
	m_kImage.Draw( dc, rect );
}
