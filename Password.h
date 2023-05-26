#pragma once



// CPassword 窗体视图

class CPassword : public CFormView
{
	DECLARE_DYNCREATE(CPassword)

protected:
	CPassword();           // 动态创建所使用的受保护的构造函数
	virtual ~CPassword();

public:
	enum { IDD = IDD_DIALOG_PW_MODIFY };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};


