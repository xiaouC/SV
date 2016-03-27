// SelectSurfaceTextureDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "StardewValley.h"
#include "SelectSurfaceTextureDlg.h"
#include "afxdialogex.h"

// CSelectSurfaceTextureDlg 对话框
IMPLEMENT_DYNAMIC(CSelectSurfaceTextureDlg, CDialogEx)
CSelectSurfaceTextureDlg::CSelectSurfaceTextureDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectSurfaceTextureDlg::IDD, pParent)
{
}

CSelectSurfaceTextureDlg::~CSelectSurfaceTextureDlg()
{
}

void CSelectSurfaceTextureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCROLLBAR1, m_kScrollBar);
	DDX_Control(pDX, IDC_STATIC_VIEW, m_kStatic);
}

BEGIN_MESSAGE_MAP(CSelectSurfaceTextureDlg, CDialogEx)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDOK, &CSelectSurfaceTextureDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSelectSurfaceTextureDlg::OnBnClickedCancel)
	ON_WM_MOUSEHWHEEL()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()

// CSelectSurfaceTextureDlg 消息处理程序
int CSelectSurfaceTextureDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	int nOffsetX = 10, nOffsetY = 10;
	int nRowHeight = 200, nColWidth = 200;
	int nColCount = 4;
	int nInterval = 10;

	int nCount = ::GetPrivateProfileInt( _T("init"), _T("count"), 0, _T("./Editor/MapBlock.ini") );
	for( int i=1; i <= nCount; ++i )
	{
		CString strKey;
		strKey.Format( _T("surfaceTex%d"), i );

		CString strTextureName;
		GetPrivateProfileString( _T("init"), strKey, _T(""), strTextureName.GetBuffer( MAX_PATH ), MAX_PATH, _T("./Editor/MapBlock.ini") );

		CRect rectDummy( 0, 0, nColWidth, nRowHeight );
		const DWORD dwStyle = WS_CHILD | WS_VISIBLE | SS_NOTIFY | WS_BORDER;
		CSurfaceTexStatic* pStatic = new CSurfaceTexStatic( strTextureName );
		if( !pStatic->Create( "", dwStyle, rectDummy, &m_kStatic, i ) )
			return -1;

		int nX = ( i - 1 ) % nColCount * ( nColWidth + nInterval ) + nOffsetX;
		int nY = ( i - 1 ) / nColCount * ( nRowHeight + nInterval ) + nOffsetY;
		pStatic->MoveWindow( nX, nY, nColWidth, nRowHeight, TRUE );

		m_listSurfaceTexWnd.push_back( pStatic );
	}

	return 0;
}

void CSelectSurfaceTextureDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}

void CSelectSurfaceTextureDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CSelectSurfaceTextureDlg::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// 此功能要求 Windows Vista 或更高版本。
	// _WIN32_WINNT 符号必须 >= 0x0600。
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnMouseHWheel(nFlags, zDelta, pt);
}


void CSelectSurfaceTextureDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}
