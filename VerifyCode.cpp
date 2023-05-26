// VerifyCode.cpp : 实现文件
//

#include "stdafx.h"
#include "RTK_Pack.h"
#include "VerifyCode.h"
#include "afxdialogex.h"
#include "Mainfrm.h"

// CVerifyCode 对话框

IMPLEMENT_DYNAMIC(CVerifyCode, CDialog)

CVerifyCode::CVerifyCode(CWnd* pParent /*=NULL*/)
	: CDialog(CVerifyCode::IDD, pParent)
{

}

CVerifyCode::~CVerifyCode()
{
}

void CVerifyCode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVerifyCode, CDialog)
	ON_BN_CLICKED(IDOK, &CVerifyCode::OnBnClickedOk)
END_MESSAGE_MAP()


// CVerifyCode 消息处理程序


void CVerifyCode::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	CString str, sResp, sOutput;
	((CComboBox*)GetDlgItem(IDC_EDIT_PASSWORD))->GetWindowTextA(str);
	if (str.Trim().GetLength() == 0)
	{
		AfxMessageBox("Code is empty", MB_OK | MB_ICONINFORMATION);
		return;
	}
	if (api_verify_code(str.Trim(), sResp) == FALSE)
	{
		AfxMessageBox("Query server fail", MB_OK | MB_ICONINFORMATION);
		return;
	}
	if (parseJson(sResp, "res", 0, 0, "", sOutput) == FALSE)
	{
		sOutput = "Parse res fail";
		AfxMessageBox(sOutput, MB_OK | MB_ICONINFORMATION);
		return;
	}
	if (sOutput != "1")
	{
		if (parseJson(sResp, "msg", 0, 0, "", sOutput) == FALSE)
		{
			sOutput = "Verify dynamic code fail";
		}
		AfxMessageBox(sOutput, MB_OK | MB_ICONINFORMATION);
		return;
	}
	CDialog::OnOK();
}
