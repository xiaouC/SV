#pragma once
#include "SelectSurfaceTextureDlg.h"

// CNewMapBlockDlg 对话框
class CNewMapBlockDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewMapBlockDlg)

public:
	CNewMapBlockDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNewMapBlockDlg();

// 对话框数据
	enum { IDD = IDD_NEW_MAP_BLOCK_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strBlockName;
	CString m_strMaterial;
	afx_msg void OnStnClickedStaticDefaultPng();
	afx_msg void OnBnClickedOk();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
};
