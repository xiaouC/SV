#pragma once
#include "afxwin.h"


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
	int m_nBlockRow;
public:
	int m_nBlockCol;
	int m_nGridWidth;
	int m_nGridHeight;
	CString m_strMaterial;
	CStatic m_kDefaultPNG;
	afx_msg void OnStnClickedStaticDefaultPng();
	afx_msg void OnBnClickedOk();
	CString m_strFileName;
};
