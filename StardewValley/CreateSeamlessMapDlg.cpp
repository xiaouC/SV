// CreateSeamlessMapDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "StardewValley.h"
#include "CreateSeamlessMapDlg.h"
#include "afxdialogex.h"
#include "Map/TLSeamlessMap.h"
#include "MainFrm.h"
#include "ChildView.h"

// CCreateSeamlessMapDlg 对话框
IMPLEMENT_DYNAMIC(CCreateSeamlessMapDlg, CDialogEx)
CCreateSeamlessMapDlg::CCreateSeamlessMapDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCreateSeamlessMapDlg::IDD, pParent)
	, m_nBlockRow(32)
	, m_nBlockCol(32)
	, m_nGridWidth(32)
	, m_nGridHeight(32)
	, m_strFileName(_T(""))
	, m_strBlockName(_T(""))
	, m_strMaterial(_T(""))
{
}

CCreateSeamlessMapDlg::~CCreateSeamlessMapDlg()
{
}

void CCreateSeamlessMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ROW, m_nBlockRow);
	DDX_Text(pDX, IDC_EDIT_COL, m_nBlockCol);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_nGridWidth);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_nGridHeight);
	DDX_Text(pDX, IDC_EDIT_FILE_NAME, m_strFileName);
	DDX_Text(pDX, IDC_EDIT_BLOCK_NAME, m_strBlockName);
}

BEGIN_MESSAGE_MAP(CCreateSeamlessMapDlg, CDialogEx)
	ON_STN_CLICKED(IDC_STATIC_DEFAULT_PNG, &CCreateSeamlessMapDlg::OnStnClickedStaticDefaultPng)
	ON_BN_CLICKED(IDOK, &CCreateSeamlessMapDlg::OnBnClickedOk)
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CCreateSeamlessMapDlg 消息处理程序
void CCreateSeamlessMapDlg::OnStnClickedStaticDefaultPng()
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

void CCreateSeamlessMapDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();

	if( m_strFileName.IsEmpty() )
	{
		AfxMessageBox( _T("地图名不能为空！"), MB_OK );

		return;
	}

	CString strTemp;
	strTemp.Format( "./map/%s", m_strFileName );
	if( TLSeamlessMap::newSeamlessMap( strTemp.GetBuffer(), m_strBlockName.GetBuffer(), m_nBlockRow, m_nBlockCol, m_nGridWidth, m_nGridHeight, m_strMaterial.GetBuffer() ) )
	{
		strTemp.Append( ".sm" );

		((CMainFrame*)AfxGetMainWnd())->openSeamlessMap( strTemp );

		CDialogEx::OnOK();
	}
	else
	{
		AfxMessageBox( _T("创建失败！"), MB_OK );
	}
}

int CCreateSeamlessMapDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	return 0;
}

BOOL CCreateSeamlessMapDlg::OnInitDialog()
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
