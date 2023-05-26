#pragma once
#include "UserManage.h"


// CEditBoxNum �Ի���

class CEditBoxNum : public CDialog
{
	DECLARE_DYNAMIC(CEditBoxNum)

public:
	CEditBoxNum(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEditBoxNum();

// �Ի�������
	enum { IDD = IDD_DIALOG_EDIT_BOXNUM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

public:
	//int SpecifyNum;
	CString sSpecifyNum;
};
