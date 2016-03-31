// DDEffectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "StardewValley.h"
#include "DDEffectDlg.h"
#include "afxdialogex.h"


// CDDEffectDlg 对话框

IMPLEMENT_DYNAMIC(CDDEffectDlg, CDialogEx)

CDDEffectDlg::CDDEffectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDDEffectDlg::IDD, pParent)
{

}

CDDEffectDlg::~CDDEffectDlg()
{
}

void CDDEffectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDDEffectDlg, CDialogEx)
END_MESSAGE_MAP()


// CDDEffectDlg 消息处理程序


BOOL CDDEffectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ModifyStyle( 0, WS_SIZEBOX );

	SetWindowLong( GetSafeHwnd(),GWL_EXSTYLE, GetWindowLong( GetSafeHwnd(), GWL_EXSTYLE ) ^ 0x80000 );
	HINSTANCE hInst = LoadLibrary( "User32.DLL" ); 
	if( hInst ) 
	{            
		typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD);          
		MYFUNC fun = (MYFUNC)GetProcAddress( hInst, "SetLayeredWindowAttributes" );
		if( fun )
			fun( GetSafeHwnd(), 0, 128 ,2 );   

		FreeLibrary( hInst ); 
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
