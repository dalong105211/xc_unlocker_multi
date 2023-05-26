// Password.cpp : 实现文件
//

#include "stdafx.h"
#include "RTK_Pack.h"
#include "Password.h"
#include "MainFrm.h"

// CPassword

IMPLEMENT_DYNCREATE(CPassword, CFormView)

CPassword::CPassword()
	: CFormView(CPassword::IDD)
{

}

CPassword::~CPassword()
{
}

void CPassword::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPassword, CFormView)
	ON_BN_CLICKED(IDOK, &CPassword::OnBnClickedOk)
END_MESSAGE_MAP()


// CPassword 诊断

#ifdef _DEBUG
void CPassword::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CPassword::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPassword 消息处理程序


void CPassword::OnBnClickedOk()
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
	CFormView::OnOK();

}
