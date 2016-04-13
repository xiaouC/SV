// NewMapBlockDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "StardewValley.h"
#include "NewMapBlockDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "ChildView.h"

// CNewMapBlockDlg 对话框
IMPLEMENT_DYNAMIC(CNewMapBlockDlg, CDialogEx)
CNewMapBlockDlg::CNewMapBlockDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNewMapBlockDlg::IDD, pParent)
	, m_strBlockName(_T(""))
	, m_strMaterial(_T(""))
{
}

CNewMapBlockDlg::~CNewMapBlockDlg()
{
}

void CNewMapBlockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_BLOCK_NAME, m_strBlockName);
}

BEGIN_MESSAGE_MAP(CNewMapBlockDlg, CDialogEx)
	ON_STN_CLICKED(IDC_STATIC_DEFAULT_PNG, &CNewMapBlockDlg::OnStnClickedStaticDefaultPng)
	ON_BN_CLICKED(IDOK, &CNewMapBlockDlg::OnBnClickedOk)
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CNewMapBlockDlg 消息处理程序
void CNewMapBlockDlg::OnStnClickedStaticDefaultPng()
{
	// TODO: 在此添加控件通知处理程序代码
	CSelectSurfaceTextureDlg dlg;
	dlg.m_strInitSelectedFileName = m_strMaterial;
	if( dlg.DoModal() == IDOK )
	{
		m_strMaterial = dlg.getCurrentSelectedTexFile();

		CStatic* pStatic = (CStatic*)GetDlgItem( IDC_STATIC_DEFAULT_PNG );
		if( pStatic != NULL )
		{
			CString strTemp;
			strTemp.Format( "images/%s", m_strMaterial );

			CImage image;
			image.Load( strTemp );
			pStatic->SetBitmap( image.Detach() );
		}
	}
}

void CNewMapBlockDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();

	CDialogEx::OnOK();
}

int CNewMapBlockDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	return 0;
}

BOOL CNewMapBlockDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	GetPrivateProfileString( _T("init"), "surfaceTex1", _T(""), m_strMaterial.GetBuffer( MAX_PATH ), MAX_PATH, _T("./Editor/MapBlock.ini") );

	CStatic* pStatic = (CStatic*)GetDlgItem( IDC_STATIC_DEFAULT_PNG );
	if( pStatic != NULL )
	{
		CString strTemp;
		strTemp.Format( "images/%s", m_strMaterial );

		CImage image;
		image.Load( strTemp );
		pStatic->SetBitmap( image.Detach() );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}