#pragma once


// CLineConfig 对话框

class CLineConfig : public CDialog
{
	DECLARE_DYNAMIC(CLineConfig)

public:
	CLineConfig(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLineConfig();

// 对话框数据
	enum { IDD = IDD_DIALOG_LINECONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	CString m_sLine;
};
