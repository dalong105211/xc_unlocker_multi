// UserManage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RTK_Pack.h"
#include "UserManage.h"
#include ".\UserManage.h"

#include "MainFrm.h"
#include "GridCtrl.h"
#include "winspool.h"

// CCarton

IMPLEMENT_DYNCREATE(CCarton, CFormView)

CCarton::CCarton()
	: CFormView(CCarton::IDD)
{
	m_pGridCtrl = NULL;
}

CCarton::~CCarton()
{
	if (m_pGridCtrl)
		delete m_pGridCtrl;
}

void CCarton::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCarton, CFormView)
	ON_WM_DESTROY()
	
	ON_CBN_SELCHANGE(IDC_COMBO_FUNCTION, &CCarton::OnCbnSelchangeComboFunction)
	ON_BN_CLICKED(IDC_BUTTON_ACT, &CCarton::OnBnClickedButtonAct)
	ON_CBN_SELCHANGE(IDC_COMBO_USER, &CCarton::OnCbnSelchangeComboUser)
END_MESSAGE_MAP()


// CCarton

#ifdef _DEBUG
void CCarton::AssertValid() const
{
	CFormView::AssertValid();
}

void CCarton::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

//
void CCarton::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	CString Str;
	Str = "User Manage " + ((CMainFrame*)AfxGetMainWnd())->GetFileVersion();
	GetParentFrame()->SetWindowText(Str);

	CFont* m_font;
	m_font = new CFont;
	m_font->CreateFont(35,0,0,0,700,FALSE,FALSE,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_MODERN,"����");

	/*GetDlgItem(IDC_STATIC_PRODUCT)->SetFont(m_font);
	GetDlgItem(IDC_STATIC_CUSTOMER)->SetFont(m_font);
	GetDlgItem(IDC_STATIC_LINE)->SetFont(m_font);
	GetDlgItem(IDC_STATIC_COLOR)->SetFont(m_font);*/

	
	m_font = new CFont;
	m_font->CreateFont(25,0,0,0,700,FALSE,FALSE,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_MODERN,"����");

	//m_BtnST_Specify.SetFont(m_font);
	//m_BtnST_RePrint.SetIcon(IDI_ICON_REPRINT);

	((CComboBox*)GetDlgItem(IDC_COMBO_FUNCTION))->ResetContent();
	((CComboBox*)GetDlgItem(IDC_COMBO_FUNCTION))->AddString("01_Add Normal Account");
	((CComboBox*)GetDlgItem(IDC_COMBO_FUNCTION))->AddString("02_Delete Account");
	((CComboBox*)GetDlgItem(IDC_COMBO_FUNCTION))->AddString("03_Modify Password");
	((CComboBox*)GetDlgItem(IDC_COMBO_FUNCTION))->AddString("04_Add Administrator Account");
	((CComboBox*)GetDlgItem(IDC_COMBO_FUNCTION))->SetCurSel(0);
	GetDlgItem(IDC_COMBO_USER)->ShowWindow(0);
}

// CCarton
BOOL CCarton::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		return TRUE;
	}
	return CFormView::PreTranslateMessage(pMsg);

}

void CCarton::OnDestroy()
{
	CFormView::OnDestroy();
	((CMainFrame*)AfxGetMainWnd())->m_Carton=NULL; //
	((CMainFrame*)AfxGetMainWnd())->SetStatusBar(" ", " ", " ", " ");//

	// TODO: Add your message handler code here
}


void CCarton::OnCbnSelchangeComboFunction()
{
	// TODO:
	CString str;
	GetDlgItem(IDC_COMBO_FUNCTION)->GetWindowTextA(str);

	//01_Add Normal Account
	if (str.Find("01_") >= 0)
	{
		GetDlgItem(IDC_STATIC_SELECT_USER)->ShowWindow(0);
		GetDlgItem(IDC_COMBO_USER)->ShowWindow(0);
		GetDlgItem(IDC_STATIC_USER)->ShowWindow(1);
		GetDlgItem(IDC_EDIT_USER)->ShowWindow(1);
		GetDlgItem(IDC_EDIT_USER)->EnableWindow(1);
		GetDlgItem(IDC_STATIC_PW)->ShowWindow(1);
		GetDlgItem(IDC_EDIT_PASSWORD)->ShowWindow(1);
		GetDlgItem(IDC_STATIC_PW_CFM)->ShowWindow(1);
		GetDlgItem(IDC_EDIT_PASSWORD_CFM)->ShowWindow(1);
		GetDlgItem(IDC_STATIC_ACCESS)->ShowWindow(1);
		GetDlgItem(IDC_CHECK_FUN_1)->ShowWindow(1);
		GetDlgItem(IDC_CHECK_FUN_2)->ShowWindow(1);
		GetDlgItem(IDC_CHECK_FUN_3)->ShowWindow(1);
		GetDlgItem(IDC_EDIT_USER)->SetWindowTextA("");
		GetDlgItem(IDC_EDIT_PASSWORD)->SetWindowTextA("");
		GetDlgItem(IDC_EDIT_PASSWORD_CFM)->SetWindowTextA("");
	}
	//02_Delete Account
	else if (str.Find("02_") >= 0)
	{
		GetDlgItem(IDC_STATIC_SELECT_USER)->ShowWindow(1);
		GetDlgItem(IDC_COMBO_USER)->ShowWindow(1);
		GetDlgItem(IDC_STATIC_USER)->ShowWindow(0);
		GetDlgItem(IDC_EDIT_USER)->ShowWindow(0);
		GetDlgItem(IDC_STATIC_PW)->ShowWindow(0);
		GetDlgItem(IDC_EDIT_PASSWORD)->ShowWindow(0);
		GetDlgItem(IDC_STATIC_PW_CFM)->ShowWindow(0);
		GetDlgItem(IDC_EDIT_PASSWORD_CFM)->ShowWindow(0);
		GetDlgItem(IDC_STATIC_ACCESS)->ShowWindow(0);
		GetDlgItem(IDC_CHECK_FUN_1)->ShowWindow(0);
		GetDlgItem(IDC_CHECK_FUN_2)->ShowWindow(0);
		GetDlgItem(IDC_CHECK_FUN_3)->ShowWindow(0);
		GetDlgItem(IDC_EDIT_USER)->SetWindowTextA("");
		GetDlgItem(IDC_EDIT_PASSWORD)->SetWindowTextA("");
		GetDlgItem(IDC_EDIT_PASSWORD_CFM)->SetWindowTextA("");
		UpdateUserList();
	}
	//03_Modify Password
	else if (str.Find("03_") >= 0)
	{
		GetDlgItem(IDC_STATIC_SELECT_USER)->ShowWindow(1);
		GetDlgItem(IDC_COMBO_USER)->ShowWindow(1);
		GetDlgItem(IDC_STATIC_USER)->ShowWindow(1);
		GetDlgItem(IDC_EDIT_USER)->ShowWindow(1);
		GetDlgItem(IDC_EDIT_USER)->EnableWindow(0);
		GetDlgItem(IDC_STATIC_PW)->ShowWindow(1);
		GetDlgItem(IDC_EDIT_PASSWORD)->ShowWindow(1);
		GetDlgItem(IDC_STATIC_PW_CFM)->ShowWindow(1);
		GetDlgItem(IDC_EDIT_PASSWORD_CFM)->ShowWindow(1);
		GetDlgItem(IDC_STATIC_ACCESS)->ShowWindow(0);
		GetDlgItem(IDC_CHECK_FUN_1)->ShowWindow(0);
		GetDlgItem(IDC_CHECK_FUN_2)->ShowWindow(0);
		GetDlgItem(IDC_CHECK_FUN_3)->ShowWindow(0);
		GetDlgItem(IDC_EDIT_USER)->SetWindowTextA("");
		GetDlgItem(IDC_EDIT_PASSWORD)->SetWindowTextA("");
		GetDlgItem(IDC_EDIT_PASSWORD_CFM)->SetWindowTextA("");
		UpdateUserList();
	}
	//04_Add Administrator Account
	else if (str.Find("04_") >= 0)
	{
		GetDlgItem(IDC_STATIC_SELECT_USER)->ShowWindow(0);
		GetDlgItem(IDC_COMBO_USER)->ShowWindow(0);
		GetDlgItem(IDC_STATIC_USER)->ShowWindow(1);
		GetDlgItem(IDC_EDIT_USER)->ShowWindow(1);
		GetDlgItem(IDC_EDIT_USER)->EnableWindow(1);
		GetDlgItem(IDC_STATIC_PW)->ShowWindow(1);
		GetDlgItem(IDC_EDIT_PASSWORD)->ShowWindow(1);
		GetDlgItem(IDC_STATIC_PW_CFM)->ShowWindow(1);
		GetDlgItem(IDC_EDIT_PASSWORD_CFM)->ShowWindow(1);
		GetDlgItem(IDC_STATIC_ACCESS)->ShowWindow(0);
		GetDlgItem(IDC_CHECK_FUN_1)->ShowWindow(0);
		GetDlgItem(IDC_CHECK_FUN_2)->ShowWindow(0);
		GetDlgItem(IDC_CHECK_FUN_3)->ShowWindow(0);
		GetDlgItem(IDC_EDIT_USER)->SetWindowTextA("");
		GetDlgItem(IDC_EDIT_PASSWORD)->SetWindowTextA("");
		GetDlgItem(IDC_EDIT_PASSWORD_CFM)->SetWindowTextA("");
	}
}


void CCarton::OnBnClickedButtonAct()
{
	CString str,sOutput;
	GetDlgItem(IDC_COMBO_FUNCTION)->GetWindowTextA(str);

	//check login status
	if (!sql_check_login(sOutput))
	{
		((CMainFrame*)AfxGetMainWnd())->m_Carton->Msg(sOutput, false);
		Logger("UserManage", sOutput);
		return;
	}

	//01_Add Normal Account
	if (str.Find("01_") >= 0)
	{
		if (AddNormalUser())
		{
			GetDlgItem(IDC_EDIT_USER)->SetWindowTextA("");
			GetDlgItem(IDC_EDIT_PASSWORD)->SetWindowTextA("");
			GetDlgItem(IDC_EDIT_PASSWORD_CFM)->SetWindowTextA("");
			((CButton *)GetDlgItem(IDC_CHECK_FUN_1))->SetCheck(0);
			((CButton *)GetDlgItem(IDC_CHECK_FUN_2))->SetCheck(0);
			((CButton *)GetDlgItem(IDC_CHECK_FUN_3))->SetCheck(0);
		}
	}
	//02_Delete Account
	else if (str.Find("02_") >= 0)
	{
		if (DeleteUser())
		{
			GetDlgItem(IDC_EDIT_USER)->SetWindowTextA("");
			
			UpdateUserList();
		}
		
	}
	//03_Modify Password
	else if (str.Find("03_") >= 0)
	{
		if (ModifyPassword())
		{
			GetDlgItem(IDC_EDIT_USER)->SetWindowTextA("");
			GetDlgItem(IDC_EDIT_PASSWORD)->SetWindowTextA("");
			GetDlgItem(IDC_EDIT_PASSWORD_CFM)->SetWindowTextA("");
		}
	}
	//04_Add Administrator Account
	else if (str.Find("04_") >= 0)
	{
		if (AddAdminUser())
		{
			GetDlgItem(IDC_EDIT_USER)->SetWindowTextA("");
			GetDlgItem(IDC_EDIT_PASSWORD)->SetWindowTextA("");
			GetDlgItem(IDC_EDIT_PASSWORD_CFM)->SetWindowTextA("");
		}
	}
	return;
}

bool CCarton::AddNormalUser()
{
	CString str, user, passw, passw_cfm, sOutput;
	bool fun_1, fun_2, fun_3;
	char sql[1024];
	GetDlgItem(IDC_EDIT_USER)->GetWindowTextA(str);
	user = str.Trim();
	GetDlgItem(IDC_EDIT_PASSWORD)->GetWindowTextA(passw);
	GetDlgItem(IDC_EDIT_PASSWORD_CFM)->GetWindowTextA(passw_cfm);
	fun_1 = ((CButton *)GetDlgItem(IDC_CHECK_FUN_1))->GetCheck();
	fun_2 = ((CButton *)GetDlgItem(IDC_CHECK_FUN_2))->GetCheck();
	fun_3 = ((CButton *)GetDlgItem(IDC_CHECK_FUN_3))->GetCheck();
	if (user == "")
	{
		sOutput.Format("User name is empty.");
		((CMainFrame*)AfxGetMainWnd())->m_Carton->Msg(sOutput, false);
		Logger("UserManage", sOutput);
		return false;
	}
	if (passw.GetLength() == 0)
	{
		sOutput.Format("New password is empty.");
		((CMainFrame*)AfxGetMainWnd())->m_Carton->Msg(sOutput, false);
		Logger("UserManage", sOutput);
		return false;
	}
	if (passw_cfm.GetLength() == 0)
	{
		sOutput.Format("Confirm password is empty.");
		((CMainFrame*)AfxGetMainWnd())->m_Carton->Msg(sOutput, false);
		Logger("UserManage", sOutput);
		return false;
	}
	if (passw_cfm != passw)
	{
		sOutput.Format("New password and confirm password is not same.");
		((CMainFrame*)AfxGetMainWnd())->m_Carton->Msg(sOutput, false);
		Logger("UserManage", sOutput);
		return false;
	}
	if (passw.GetLength() < 8 || passw.GetLength() > 20)
	{
		sOutput.Format("Password length should be 8 to 20 and assembly by number and letter [0-9]&[A-z].");
		((CMainFrame*)AfxGetMainWnd())->m_Carton->Msg(sOutput, false);
		Logger("UserManage", sOutput);
		return false;
	}
	if (!(fun_1 || fun_2 || fun_3))
	{
		sOutput.Format("At least select one access.");
		((CMainFrame*)AfxGetMainWnd())->m_Carton->Msg(sOutput, false);
		Logger("UserManage", sOutput);
		return false;
	}
	int p = ((int)fun_1) * 1 + ((int)fun_2) * 2 + ((int)fun_3) * 4;
	sprintf(sql, "INSERT INTO `tab_user` (`user`, `passw`, `type`, `access`, `dt`) VALUES ('%s', '%s', '0', %d, '%s');", user, Md5(passw), p, getDT());
	if (false == sql_update(sql, sOutput))
	{
		((CMainFrame*)AfxGetMainWnd())->m_Carton->Msg(sOutput, false);
		Logger("UserManage", sOutput);
		return false;
	}
	//ִ
	CString m_strLocalIP; //
	CString m_strLocalName; //
	GetLocalHostIPName(m_strLocalName, m_strLocalIP);//
	CRTK_PackApp *app = (CRTK_PackApp *)AfxGetApp();
	sprintf(sql, "INSERT INTO `tab_record_user` (`action`, `user`, `op`, `dt`, `pc`, `ip`, `type`, `module1`, `module2`, `module3`) VALUES (1, '%s', '%s', '%s', '%s', '%s', 'normal', '%d', '%d', '%d');", app->g_user.name, user, getDT(), m_strLocalName, m_strLocalIP, fun_1, fun_2, fun_3);
	if (false == sql_update(sql, sOutput))
	{
		((CMainFrame*)AfxGetMainWnd())->m_Carton->Msg(sOutput, false);
		Logger("UserManage", sOutput);
	}
	sOutput.Format("Add normal account %s success.", user);
	((CMainFrame*)AfxGetMainWnd())->m_Carton->Msg(sOutput, true);
	Logger("UserManage", sOutput);
	return true;
}


bool CCarton::AddAdminUser()
{
	CString str, user, passw, passw_cfm, sOutput;
	char sql[1024];
	GetDlgItem(IDC_EDIT_USER)->GetWindowTextA(str);
	user = str.Trim();
	GetDlgItem(IDC_EDIT_PASSWORD)->GetWindowTextA(passw);
	GetDlgItem(IDC_EDIT_PASSWORD_CFM)->GetWindowTextA(passw_cfm);
	if (user == "")
	{
		sOutput.Format("User name is empty.");
		((CMainFrame*)AfxGetMainWnd())->m_Carton->Msg(sOutput, false);
		Logger("UserManage", sOutput);
		return false;
	}
	if (passw.GetLength() == 0)
	{
		sOutput.Format("New password is empty.");
		((CMainFrame*)AfxGetMainWnd())->m_Carton->Msg(sOutput, false);
		Logger("UserManage", sOutput);
		return false;
	}
	if (passw_cfm.GetLength() == 0)
	{
		sOutput.Format("Confirm password is empty.");
		((CMainFrame*)AfxGetMainWnd())->m_Carton->Msg(sOutput, false);
		Logger("UserManage", sOutput);
		return false;
	}
	if (passw_cfm != passw)
	{
		sOutput.Format("New password and confirm password is not same.");
		((CMainFrame*)AfxGetMainWnd())->m_Carton->Msg(sOutput, false);
		Logger("UserManage", sOutput);
		return false;
	}
	if (passw.GetLength() < 8 || passw.GetLength() > 20)
	{
		sOutput.Format("Password length should be 8 to 20 and assembly by number and letter [0-9]&[A-z].");
		((CMainFrame*)AfxGetMainWnd())->m_Carton->Msg(sOutput, false);
		Logger("UserManage", sOutput);
		return false;
	}

	sprintf(sql, "INSERT INTO `tab_user` (`user`, `passw`, `type`, `dt`) VALUES ('%s', '%s', '1', '%s');", user, Md5(passw), getDT());
	if (false == sql_update(sql, sOutput))
	{
		((CMainFrame*)AfxGetMainWnd())->m_Carton->Msg(sOutput, false);
		Logger("UserManage", sOutput);
		return false;
	}
	//
	CString m_strLocalIP; //
	CString m_strLocalName; //
	GetLocalHostIPName(m_strLocalName, m_strLocalIP);//
	CRTK_PackApp *app = (CRTK_PackApp *)AfxGetApp();
	sprintf(sql, "INSERT INTO `tab_record_user` (`action`, `user`, `op`, `dt`, `pc`, `ip`,`type`) VALUES (1, '%s', '%s', '%s', '%s', '%s','admin');", app->g_user.name, user, getDT(), m_strLocalName, m_strLocalIP);
	if (false == sql_update(sql, sOutput))
	{
		((CMainFrame*)AfxGetMainWnd())->m_Carton->Msg(sOutput, false);
		Logger("UserManage", sOutput);
	}
	sOutput.Format("Add admin account %s success.", user);
	((CMainFrame*)AfxGetMainWnd())->m_Carton->Msg(sOutput, true);
	Logger("UserManage", sOutput);
	return true;
}


bool CCarton::ModifyPassword()
{
	CString str, user, passw, passw_cfm, sOutput;
	char sql[1024];
	GetDlgItem(IDC_EDIT_USER)->GetWindowTextA(str);
	user = str.Trim();
	GetDlgItem(IDC_EDIT_PASSWORD)->GetWindowTextA(passw);
	GetDlgItem(IDC_EDIT_PASSWORD_CFM)->GetWindowTextA(passw_cfm);
	if (user == "")
	{
		sOutput.Format("User name is empty.");
		((CMainFrame*)AfxGetMainWnd())->m_Carton->Msg(sOutput, false);
		Logger("UserManage", sOutput);
		return false;
	}
	if (passw.GetLength() == 0)
	{
		sOutput.Format("New password is empty.");
		((CMainFrame*)AfxGetMainWnd())->m_Carton->Msg(sOutput, false);
		Logger("UserManage", sOutput);
		return false;
	}
	if (passw_cfm.GetLength() == 0)
	{
		sOutput.Format("Confirm password is empty.");
		((CMainFrame*)AfxGetMainWnd())->m_Carton->Msg(sOutput, false);
		Logger("UserManage", sOutput);
		return false;
	}
	if (passw_cfm != passw)
	{
		sOutput.Format("New password and confirm password is not same.");
		((CMainFrame*)AfxGetMainWnd())->m_Carton->Msg(sOutput, false);
		Logger("UserManage", sOutput);
		return false;
	}
	if (passw.GetLength() < 8 || passw.GetLength() > 20)
	{
		sOutput.Format("Password length should be 8 to 20 and assembly by number and letter [0-9]&[A-z].");
		((CMainFrame*)AfxGetMainWnd())->m_Carton->Msg(sOutput, false);
		Logger("UserManage", sOutput);
		return false;
	}

	sprintf(sql, "UPDATE `tab_user` SET `passw`= '%s', dt = '%s' where user = '%s';", Md5(passw), getDT(), user);
	if (false == sql_update(sql, sOutput))
	{
		((CMainFrame*)AfxGetMainWnd())->m_Carton->Msg(sOutput, false);
		Logger("UserManage", sOutput);
		return false;
	}
	//
	CString m_strLocalIP; //
	CString m_strLocalName; //
	GetLocalHostIPName(m_strLocalName, m_strLocalIP);//
	CRTK_PackApp *app = (CRTK_PackApp *)AfxGetApp();
	sprintf(sql, "INSERT INTO `tab_record_user` (`action`, `user`, `op`, `dt`, `pc`, `ip`) VALUES (3, '%s', '%s', '%s', '%s', '%s');", app->g_user.name, user, getDT(), m_strLocalName, m_strLocalIP);
	if (false == sql_update(sql, sOutput))
	{
		((CMainFrame*)AfxGetMainWnd())->m_Carton->Msg(sOutput, false);
		Logger("UserManage", sOutput);
	}
	sOutput.Format("Modify password of %s success.", user);
	((CMainFrame*)AfxGetMainWnd())->m_Carton->Msg(sOutput, true);
	Logger("UserManage", sOutput);
	return true;
}

bool CCarton::DeleteUser()
{
	CString str, user, sOutput;
	char sql[1024];
	GetDlgItem(IDC_COMBO_USER)->GetWindowTextA(str);
	user = str.Trim();
	/*if (user == "")
	{
		sOutput.Format("User name is empty.");
		return false;
	}*/
	sprintf(sql, "DELETE FROM `tab_user` WHERE `user`='%s' limit 1", user);
	if (false == sql_update(sql, sOutput))
	{
		((CMainFrame*)AfxGetMainWnd())->m_Carton->Msg(sOutput, false);
		Logger("UserManage", sOutput);
		return false;
	}
	//
	CString m_strLocalIP; //
	CString m_strLocalName; //
	GetLocalHostIPName(m_strLocalName, m_strLocalIP);//
	CRTK_PackApp *app = (CRTK_PackApp *)AfxGetApp();
	sprintf(sql, "INSERT INTO `tab_record_user` (`action`, `user`, `op`, `dt`, `pc`, `ip`) VALUES (2, '%s', '%s', '%s', '%s', '%s');", app->g_user.name, user, getDT(), m_strLocalName, m_strLocalIP);
	if (false == sql_update(sql, sOutput))
	{
		((CMainFrame*)AfxGetMainWnd())->m_Carton->Msg(sOutput, false);
		Logger("UserManage", sOutput);
		return false;
	}
	sOutput.Format("Delete user %s success.", user);
	((CMainFrame*)AfxGetMainWnd())->m_Carton->Msg(sOutput, true);
	Logger("UserManage", sOutput);
	return true;
}

void CCarton::UpdateUserList()
{
	((CComboBox*)GetDlgItem(IDC_COMBO_USER))->ResetContent();
	CString str,sOutput;
	MYSQL_RES *result;
	MYSQL_ROW row;
	char sql[1024];
	sprintf(sql, "SELECT user from `tab_user`;");
	if (mysql_query(((CMainFrame*)AfxGetMainWnd())->conn, sql))
	{
		sOutput.Format("Error:%s", mysql_error(((CMainFrame*)AfxGetMainWnd())->conn));
		((CMainFrame*)AfxGetMainWnd())->m_Carton->Msg(sOutput, false);
		Logger("UserManage", sOutput);
		return;
	}
	result = mysql_store_result(((CMainFrame*)AfxGetMainWnd())->conn);
	CRTK_PackApp *app = (CRTK_PackApp *)AfxGetApp();
	int j = 0;
	do
	{
		row = mysql_fetch_row(result);
		if (row == NULL)
		{
			break;
		}
		str.Format("%s", row[0]);
		if (str != app->g_user.name && str != "ADMIN")	//
		{
			((CComboBox *)GetDlgItem(IDC_COMBO_USER))->AddString(str);
		}
		j++;
	} while (TRUE);
	mysql_free_result(result);
}

void CCarton::OnCbnSelchangeComboUser()
{
	// TODO:
	CString str;
	GetDlgItem(IDC_COMBO_USER)->GetWindowTextA(str);
	GetDlgItem(IDC_EDIT_USER)->SetWindowTextA(str);
}
