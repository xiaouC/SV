#pragma once
#include "SelectSurfaceTextureDlg.h"

// CNewMapBlockDlg �Ի���
class CNewMapBlockDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewMapBlockDlg)

public:
	CNewMapBlockDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNewMapBlockDlg();

// �Ի�������
	enum { IDD = IDD_NEW_MAP_BLOCK_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strBlockName;
	CString m_strMaterial;
	afx_msg void OnStnClickedStaticDefaultPng();
	afx_msg void OnBnClickedOk();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
};
