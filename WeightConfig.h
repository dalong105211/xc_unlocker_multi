#pragma once


// CWeightConfig 对话框

class CWeightConfig : public CDialog
{
	DECLARE_DYNAMIC(CWeightConfig)

public:
	CWeightConfig(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWeightConfig();

// 对话框数据
	enum { IDD = IDD_DIALOG_WEIGHTCONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	double m_fWeight_Min,m_fWeight_Max;
	int m_iBalanceSel;

	afx_msg void OnCbnSelchangeComboBalance();
};
