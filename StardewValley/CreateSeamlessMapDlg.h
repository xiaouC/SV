#pragma once
#include "afxwin.h"
#include "SelectSurfaceTextureDlg.h"

// CCreateSeamlessMapDlg 对话框
class CCreateSeamlessMapDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCreateSeamlessMapDlg)

public:
	CCreateSeamlessMapDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCreateSeamlessMapDlg();

// 对话框数据
	enum { IDD = IDD_CREATE_SEAMLESS_MAP_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_nBlockRow;
	int m_nBlockCol;
	int m_nGridWidth;
	int m_nGridHeight;
	CString m_strFileName;
	CString m_strMaterial;
	afx_msg void OnStnClickedStaticDefaultPng();
	afx_msg void OnBnClickedOk();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
};
