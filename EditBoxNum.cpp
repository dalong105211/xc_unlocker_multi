// EditBoxNum.cpp : 实现文件
//

#include "stdafx.h"
#include "RTK_Pack.h"
#include "EditBoxNum.h"
#include ".\editboxnum.h"

// CEditBoxNum 对话框

IMPLEMENT_DYNAMIC(CEditBoxNum, CDialog)
CEditBoxNum::CEditBoxNum(CWnd* pParent /*=NULL*/)
	: CDialog(CEditBoxNum::IDD, pParent)
{
}

CEditBoxNum::~CEditBoxNum()
{
}

void CEditBoxNum::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEditBoxNum, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CEditBoxNum 消息处理程序

void CEditBoxNum::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString Str;

	GetDlgItem(IDC_EDIT_BOXNUM)->GetWindowText(Str);
	sSpecifyNum = Str;
	/*SpecifyNum = atoi(Str);
	if (SpecifyNum <= 0)
	{
		AfxMessageBox("请输入正确箱号",MB_OK);
		return;
	}*/
	if (sSpecifyNum.GetLength() == 0)
	{
		AfxMessageBox("请输入正确箱号",MB_OK);
		return;
	}

	OnOK();
}

void CEditBoxNum::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}
