// CreateSeamlessMapDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "StardewValley.h"
#include "CreateSeamlessMapDlg.h"
#include "afxdialogex.h"
#include "Map/TLSeamlessMap.h"
#include "SelectSurfaceTextureDlg.h"
#include "MainFrm.h"
#include "ChildView.h"

// CCreateSeamlessMapDlg �Ի���

IMPLEMENT_DYNAMIC(CCreateSeamlessMapDlg, CDialogEx)

CCreateSeamlessMapDlg::CCreateSeamlessMapDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCreateSeamlessMapDlg::IDD, pParent)
	, m_nBlockRow(64)
	, m_nBlockCol(64)
	, m_nGridWidth(32)
	, m_nGridHeight(32)
	, m_strFileName(_T(""))
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
	DDX_Control(pDX, IDC_STATIC_DEFAULT_PNG, m_kDefaultPNG);
	DDX_Text(pDX, IDC_EDIT_FILE_NAME, m_strFileName);
}


BEGIN_MESSAGE_MAP(CCreateSeamlessMapDlg, CDialogEx)
	ON_STN_CLICKED(IDC_STATIC_DEFAULT_PNG, &CCreateSeamlessMapDlg::OnStnClickedStaticDefaultPng)
	ON_BN_CLICKED(IDOK, &CCreateSeamlessMapDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCreateSeamlessMapDlg ��Ϣ�������


void CCreateSeamlessMapDlg::OnStnClickedStaticDefaultPng()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//AfxMessageBox( _T("feifei"), MB_OK );
	CSelectSurfaceTextureDlg dlg;
	if( dlg.DoModal() == IDOK )
	{
		m_strMaterial = dlg.getCurrentSelectedTexFile();
	}
}


void CCreateSeamlessMapDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();

	CString strTemp;
	strTemp.Format( "./map/%s", m_strFileName );
	if( TLSeamlessMap::newSeamlessMap( strTemp.GetBuffer(), m_nBlockRow, m_nBlockCol, m_nGridWidth, m_nGridHeight, m_strMaterial.GetBuffer() ) )
	{
		strTemp.Append( ".sm" );

		((CMainFrame*)AfxGetMainWnd())->openSeamlessMap( strTemp );

		CDialogEx::OnOK();
	}
	else
	{
		AfxMessageBox( _T("����ʧ�ܣ�"), MB_OK );
	}
}
