#pragma once


// CVerifyCode 对话框

class CVerifyCode : public CDialog
{
	DECLARE_DYNAMIC(CVerifyCode)

public:
	CVerifyCode(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVerifyCode();

// 对话框数据
	enum { IDD = IDD_DIALOG_PASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
