#pragma once


// CDDEffectDlg 对话框

class CDDEffectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDDEffectDlg)

public:
	CDDEffectDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDDEffectDlg();

// 对话框数据
	enum { IDD = IDD_DD_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
