// NewMapBlockDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "StardewValley.h"
#include "NewMapBlockDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "ChildView.h"

// CNewMapBlockDlg �Ի���
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

// CNewMapBlockDlg ��Ϣ�������
void CNewMapBlockDlg::OnStnClickedStaticDefaultPng()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();

	CDialogEx::OnOK();
}

int CNewMapBlockDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������

	return 0;
}

BOOL CNewMapBlockDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
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
	// �쳣: OCX ����ҳӦ���� FALSE
}