#pragma once


// CDDEffectDlg �Ի���

class CDDEffectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDDEffectDlg)

public:
	CDDEffectDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDDEffectDlg();

// �Ի�������
	enum { IDD = IDD_DD_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
