// SurfaceTexStatic.cpp : ʵ���ļ�
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

// CSurfaceTexStatic ��Ϣ�������
void CSurfaceTexStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CStatic::OnPaint()

	CRect rect;
	GetClientRect( &rect );

	dc.SetStretchBltMode( HALFTONE ); //��ֹ��СͼƬʧ��
	m_kImage.Draw( dc, rect );
}
