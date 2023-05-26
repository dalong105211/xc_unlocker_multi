// WeightConfig.cpp : 实现文件
//

#include "stdafx.h"
#include "RTK_Pack.h"
#include "WeightConfig.h"
#include ".\weightconfig.h"


// CWeightConfig 对话框

IMPLEMENT_DYNAMIC(CWeightConfig, CDialog)
CWeightConfig::CWeightConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CWeightConfig::IDD, pParent)
{
}

CWeightConfig::~CWeightConfig()
{
}

void CWeightConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWeightConfig, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO_BALANCE, OnCbnSelchangeComboBalance)
END_MESSAGE_MAP()


// CWeightConfig 消息处理程序
BOOL CWeightConfig::OnInitDialog()
{
	CDialog::OnInitDialog();
	

	return TRUE;
}

void CWeightConfig::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_iBalanceSel = ((CComboBox*)GetDlgItem(IDC_COMBO_BALANCE))->GetCurSel();

	CString Str;
	GetDlgItem(IDC_EDIT_WEIGHT_MIN)->GetWindowText(Str);
	if (Str == "")
	{
		AfxMessageBox("请输入重量下限",MB_ICONERROR|MB_TOPMOST);
		return;
	}
	m_fWeight_Min = atof(Str);
	GetDlgItem(IDC_EDIT_WEIGHT_MAX)->GetWindowText(Str);
	if (Str == "")
	{
		AfxMessageBox("请输入重量上限",MB_ICONERROR|MB_TOPMOST);
		return;
	}
	m_fWeight_Max = atof(Str);

	OnOK();
}

void CWeightConfig::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CWeightConfig::OnCbnSelchangeComboBalance()
{
	// TODO: 在此添加控件通知处理程序代码
}
