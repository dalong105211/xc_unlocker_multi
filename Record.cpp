// Record.cpp : 实现文件
//

#include "stdafx.h"
#include "RTK_Pack.h"
#include "Record.h"
#include ".\Record.h"

#include "MainFrm.h"
#include "GridCtrl.h"
#include "winspool.h"

#include "comdef.h"
#include <comutil.h>

// CRecord

IMPLEMENT_DYNCREATE(CRecord, CFormView)

CRecord::CRecord()
: CFormView(CRecord::IDD)
{
	m_pGridCtrl = NULL;
}

CRecord::~CRecord()
{
	if (m_pGridCtrl)
		delete m_pGridCtrl;
}

void CRecord::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
}

BEGIN_MESSAGE_MAP(CRecord, CFormView)
	ON_WM_DESTROY()

	ON_BN_CLICKED(IDC_BUTTON_SELECT, &CRecord::OnBnClickedButtonSelect)
	ON_CBN_SELCHANGE(IDC_COMBO_TAB, &CRecord::OnCbnSelchangeComboTab)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, &CRecord::OnBnClickedButtonExport)
END_MESSAGE_MAP()


// CRecord 诊断

#ifdef _DEBUG
void CRecord::AssertValid() const
{
	CFormView::AssertValid();
}

void CRecord::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

//初始化
void CRecord::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	CString Str;
	Str = "Record View " + ((CMainFrame*)AfxGetMainWnd())->GetFileVersion();
	GetParentFrame()->SetWindowText(Str);

	((CComboBox*)GetDlgItem(IDC_COMBO_TAB))->ResetContent();
	((CComboBox*)GetDlgItem(IDC_COMBO_TAB))->AddString("01_Record_device");
	((CComboBox*)GetDlgItem(IDC_COMBO_TAB))->AddString("02_Record_login");
	((CComboBox*)GetDlgItem(IDC_COMBO_TAB))->AddString("03_Record_user_manage");
	((CComboBox*)GetDlgItem(IDC_COMBO_TAB))->SetCurSel(-1);
	

}

// CRecord 消息处理程序
BOOL CRecord::PreTranslateMessage(MSG* pMsg)
{
	CString Str;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		switch (GetFocus()->GetDlgCtrlID())
		{
		default:
			return TRUE;

		}
	}
	return CFormView::PreTranslateMessage(pMsg);

}

void CRecord::OnDestroy()
{
	CFormView::OnDestroy();
	((CMainFrame*)AfxGetMainWnd())->m_Record = NULL; // 清空窗口指针
	((CMainFrame*)AfxGetMainWnd())->SetStatusBar(" ", " ", " ", " ");//清空状态栏

	// TODO: Add your message handler code here
}


void CRecord::OnBnClickedButtonSelect()
{
	CString sOutput;
	char sql[1024];
	CStringArray tab;
	// TODO:  在此添加控件通知处理程序代码
	CString str,user;
	GetDlgItem(IDC_COMBO_TAB)->GetWindowTextA(str);
	GetDlgItem(IDC_COMBO_KEY)->GetWindowTextA(user);
	if (str.GetLength() == 0)
	{
		sOutput.Format("Please select table first.");
		((CMainFrame*)AfxGetMainWnd())->m_Record->Msg(sOutput, false);
		Logger("RecordView", sOutput);
		return;
	}
	if (user.GetLength() == 0)
	{
		sOutput.Format("Please select user first.");
		((CMainFrame*)AfxGetMainWnd())->m_Record->Msg(sOutput, false);
		Logger("RecordView", sOutput);
		return;
	}
	//check login status
	if (!sql_check_login(sOutput))
	{
		((CMainFrame*)AfxGetMainWnd())->m_Record->Msg(sOutput, false);
		Logger("RecordView", sOutput);
		return;
	}
	//01_Record_device
	if (str.Find("01_") >= 0)
	{
		tab.Add("operation-function");
		tab.Add("operation-result");
		tab.Add("operation-datetime");
		tab.Add("operation-pc");
		tab.Add("operation-ip");
		sprintf(sql, "SELECT function,res,dt,pc,ip FROM `tab_record_cmd` WHERE user = '%s' order by dt DESC LIMIT 1000;", user);
		drawTab(tab, sql);
	}
	//02_Record_login
	else if (str.Find("02_") >= 0)
	{
		tab.Add("operation-datetime");
		tab.Add("operation-pc");
		tab.Add("operation-ip");
		sprintf(sql, "SELECT dt,pc,ip FROM `tab_record_login` WHERE user = '%s' order by dt DESC LIMIT 1000;",user);
		drawTab(tab, sql);
		//UpdateUserList();
	}
	//03_Record_user_manage
	else if (str.Find("03_") >= 0)
	{
		tab.Add("operation-type");
		tab.Add("operation-account");
		tab.Add("operation-datetime");
		tab.Add("operation-pc");
		tab.Add("operation-ip");
		tab.Add("user-type");
		tab.Add("access-module1");
		tab.Add("access-module2");
		tab.Add("access-module3");
		sprintf(sql, "SELECT action,op,dt,pc,ip,type,module1,module2,module3 FROM `tab_record_user` WHERE user = '%s' order by dt DESC LIMIT 1000;", user);
		drawTab(tab, sql);
		//UpdateUserList();
	}
	
}


bool CRecord::drawTab(CStringArray &tab,CString sql)
{
	CString str, sOutput, tab_name;
	GetDlgItem(IDC_COMBO_TAB)->GetWindowTextA(tab_name);
	MYSQL_RES *result;
	MYSQL_ROW row;
	if (mysql_query(((CMainFrame*)AfxGetMainWnd())->conn, sql))
	{
		sOutput.Format("Error:%s", mysql_error(((CMainFrame*)AfxGetMainWnd())->conn));
		((CMainFrame*)AfxGetMainWnd())->m_Record->Msg(sOutput, false);
		Logger("RecordView", sOutput);
		return false;
	}
	result = mysql_store_result(((CMainFrame*)AfxGetMainWnd())->conn);

	if (m_pGridCtrl != NULL)
	{
		m_pGridCtrl->CloseWindow();
	}
	
	int tab_row = result->row_count + 1;
	int tab_col = tab.GetSize();
	int width = 120;
	int index_width = 25;
	// Create the Gridctrl object
	m_pGridCtrl = new CGridCtrl;
	if (!m_pGridCtrl)
		return false;

	// Create the Gridctrl window
	CRect rect;
	GetClientRect(rect);
	rect.left += 11;
	//rect.top += 70;
	rect.top += 100;
	rect.right = width * tab_col + rect.left + index_width;
	rect.bottom = 420;
	m_pGridCtrl->Create(rect, this, 100);

	// fill it up with stuff
	m_pGridCtrl->SetEditable(TRUE);
	m_pGridCtrl->EnableDragAndDrop(TRUE);

	try {
		m_pGridCtrl->SetRowCount(tab_row);
		m_pGridCtrl->SetColumnCount(tab_col + 1);
		m_pGridCtrl->SetFixedRowCount(1);
		m_pGridCtrl->SetFixedColumnCount(1);
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}

	//表头
	Item.mask = GVIF_TEXT | GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = DT_LEFT | DT_WORDBREAK;
	m_pGridCtrl->SetColumnWidth(0, index_width);
	for (int i = 0; i < tab.GetSize(); i++)
	{
		Item.col = i + 1;
		Item.strText = tab[i];
		m_pGridCtrl->SetItem(&Item);
		m_pGridCtrl->SetColumnWidth(i + 1, width);
	}
	
	for (int irow = 0; irow < result->row_count; irow++)
	{
		row = mysql_fetch_row(result);
		if (row == NULL)
		{
			break;
		}
		Item.row = irow + 1;
		Item.col = 0;
		str.Format("%d", irow);
		Item.strText = str;
		m_pGridCtrl->SetItem(&Item);

		str = "";

		for (int i = 0; i < tab.GetSize(); i++)
		{
			if (i == 0 && tab_name.Find("03_") >= 0)
			{
				//if user manage record
				if (1 == atoi(row[i]))
					str.Format("ADD");
				else if (2 == atoi(row[i]))
					str.Format("DELETE");
				else if (3 == atoi(row[i]))
					str.Format("MODIFY");
				else
					str.Format("%s", row[i]);
			}
			else
			{
				//str.Format("%s", row[i]);
				str = row[i];
			}

			Item.col = i + 1;
			Item.strText = str;
			m_pGridCtrl->SetItem(&Item);
		}
	}
	
	mysql_free_result(result);
	CFont* m_font;
	m_font = new CFont;
	m_font->CreateFont(15, 0, 0, 0, 400, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_MODERN, "Arial");
	m_pGridCtrl->SetFont(m_font, TRUE);
	//m_pGridCtrl->AutoSize();
	return true;
}

void CRecord::OnCbnSelchangeComboTab()
{
	// TODO:  在此添加控件通知处理程序代码
	CString str;
	GetDlgItem(IDC_COMBO_TAB)->GetWindowTextA(str);

	//01_Record_device
	if (str.Find("01_") >= 0)
	{
		UpdateUserList("tab_record_cmd");
	}
	//02_Record_login
	else if (str.Find("02_") >= 0)
	{
		
		UpdateUserList("tab_record_login");
	}
	//03_Record_user_manage
	else if (str.Find("03_") >= 0)
	{
		
		UpdateUserList("tab_record_user");
	}
}

void CRecord::UpdateUserList(CString tab)
{
	((CComboBox*)GetDlgItem(IDC_COMBO_KEY))->ResetContent();
	CString str, sOutput;
	MYSQL_RES *result;
	MYSQL_ROW row;
	char sql[1024];
	sprintf(sql, "SELECT user FROM `%s` GROUP BY user;", tab);
	if (mysql_query(((CMainFrame*)AfxGetMainWnd())->conn, sql))
	{
		sOutput.Format("Error:%s", mysql_error(((CMainFrame*)AfxGetMainWnd())->conn));
		((CMainFrame*)AfxGetMainWnd())->m_Record->Msg(sOutput, false);
		Logger("RecordView", sOutput);
		return;
	}
	result = mysql_store_result(((CMainFrame*)AfxGetMainWnd())->conn);
	do
	{
		row = mysql_fetch_row(result);
		if (row == NULL)
		{
			break;
		}
		str.Format("%s", row[0]);
		((CComboBox *)GetDlgItem(IDC_COMBO_KEY))->AddString(str);
	} while (TRUE);
	mysql_free_result(result);
}

void CRecord::OnBnClickedButtonExport()
{
	// TODO:  在此添加控件通知处理程序代码
	CString sOutput,fileName;
	char sql[1024];
	CStringArray tab;
	// TODO:  在此添加控件通知处理程序代码
	CString str, user;
	GetDlgItem(IDC_COMBO_TAB)->GetWindowTextA(str);
	GetDlgItem(IDC_COMBO_KEY)->GetWindowTextA(user);
	if (str.GetLength() == 0)
	{
		sOutput.Format("Please select table first.");
		((CMainFrame*)AfxGetMainWnd())->m_Record->Msg(sOutput, false);
		Logger("RecordView", sOutput);
		return;
	}
	if (user.GetLength() == 0)
	{
		sOutput.Format("Please select user first.");
		((CMainFrame*)AfxGetMainWnd())->m_Record->Msg(sOutput, false);
		Logger("RecordView", sOutput);
		return;
	}
	//check login status
	if (!sql_check_login(sOutput))
	{
		((CMainFrame*)AfxGetMainWnd())->m_Record->Msg(sOutput, false);
		Logger("RecordView", sOutput);
		return;
	}

	fileName.Format("%s_%s", user, str);

	//01_Record_device
	if (str.Find("01_") >= 0)
	{
		tab.Add("operation-function");
		tab.Add("operation-result");
		tab.Add("operation-datetime");
		tab.Add("operation-pc");
		tab.Add("operation-ip");
		sprintf(sql, "SELECT function,res,dt,pc,ip FROM `tab_record_cmd` WHERE user = '%s' order by dt DESC;", user);
		exportTab(tab, sql, fileName);
	}
	//02_Record_login
	else if (str.Find("02_") >= 0)
	{
		tab.Add("operation-datetime");
		tab.Add("operation-pc");
		tab.Add("operation-ip");
		sprintf(sql, "SELECT dt,pc,ip FROM `tab_record_login` WHERE user = '%s' order by dt DESC;", user);
		exportTab(tab, sql, fileName);
		//UpdateUserList();
	}
	//03_Record_user_manage
	else if (str.Find("03_") >= 0)
	{
		tab.Add("operation-type");
		tab.Add("operation-account");
		tab.Add("operation-datetime");
		tab.Add("operation-pc");
		tab.Add("operation-ip");
		tab.Add("user-type");
		tab.Add("access-module1");
		tab.Add("access-module2");
		tab.Add("access-module3");
		sprintf(sql, "SELECT action,op,dt,pc,ip,type,module1,module2,module3 FROM `tab_record_user` WHERE user = '%s' order by dt DESC;", user);
		exportTab(tab, sql, fileName);
		//UpdateUserList();
	}
}


void CRecord::exportTab(CStringArray &tab, CString sql, CString fileName)
{
	CString str, sOutput, tab_name;
	GetDlgItem(IDC_COMBO_TAB)->GetWindowTextA(tab_name);
	MYSQL_RES *result;
	MYSQL_ROW row;
	if (mysql_query(((CMainFrame*)AfxGetMainWnd())->conn, sql))
	{
		sOutput.Format("Error:%s", mysql_error(((CMainFrame*)AfxGetMainWnd())->conn));
		((CMainFrame*)AfxGetMainWnd())->m_Record->Msg(sOutput, false);
		Logger("RecordView", sOutput);
		return ;
	}
	result = mysql_store_result(((CMainFrame*)AfxGetMainWnd())->conn);

	//row = mysql_fetch_row(result);
	//long tOtal = atoi(row[0]);
	long tOtal = result->row_count;
	if (tOtal == 0)
	{
		str.Format("No data selected.");
		((CMainFrame*)AfxGetMainWnd())->m_Record->Msg(str, FALSE);
		return;
	}

	if (!Excel_app.CreateDispatch("Excel.Application", NULL))
	{
		//MessageBox("创建Excel服务失败！", "信息提示", MB_OK); 
		((CMainFrame*)AfxGetMainWnd())->m_Record->Msg("创建Excel服务失败", FALSE);
		return;
	}

	CFileDialog   FileDlg(TRUE, "xlsx", fileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"Worksheet Files (*.xlsx)|*.xlsx|Data Files (*.xlc;*.xlsx)|*.xlc; *.xlsx|All Files (*.*)|*.*||");
	FileDlg.m_ofn.lpstrTitle = "Save File";
	if (FileDlg.DoModal() == IDCANCEL)
	{
		return;
	}

	strncpy(Excel_path, (LPCTSTR)FileDlg.GetPathName(), sizeof(Excel_path));
	if (PathFileExists(Excel_path))
	{
		if ((MessageBox("File has exsited,do you want to replace it", "Warning", MB_OKCANCEL)) == IDCANCEL)
		{
			return;
		}
	}

	Excel_app.put_Visible(FALSE);
	Excel_app.put_UserControl(FALSE);
	Excel_workBooks.AttachDispatch(Excel_app.get_Workbooks());
	Excel_workBook.AttachDispatch(Excel_workBooks.Add(covOptional));
	Excel_workSheets.AttachDispatch(Excel_workBook.get_Worksheets());
	Excel_workSheet.AttachDispatch(Excel_workSheets.get_Item(_variant_t("sheet1")));
	Excel_range.AttachDispatch(Excel_workSheet.get_Cells());
	Excel_range.put_NumberFormat(_variant_t("@"));//设置单元格格式为文本
	//上标
	for (int i = 0; i < tab.GetSize(); i++)
	{
		Excel_range.put_Item(_variant_t((long)(1)), _variant_t((long)(i + 1)), _variant_t(tab[i]));
	}
	
	//写入数据
	int rowflag = 2;
	((CMainFrame*)AfxGetMainWnd())->m_Record->Msg("Data is exporting...", TRUE);
	HWND hWnd = ::FindWindow(NULL, "RTD");
	if (hWnd)
	{
		SwitchToThisWindow(hWnd, TRUE);
	}
	m_Progress.ShowWindow(SW_NORMAL);
	while ((row = mysql_fetch_row(result)) != NULL)
	{
		for (int i = 0; i < tab.GetSize(); i++)
		{
			Excel_range.put_Item(_variant_t((long)(rowflag)), _variant_t((long)(i+1)), _variant_t(row[i]));
		}
		rowflag++;
		Progress_Pos = 100 * rowflag / tOtal;
		m_Progress.SetPos(Progress_Pos);
		((CMainFrame*)AfxGetMainWnd())->DoEvents();
	}
	m_Progress.ShowWindow(SW_HIDE);
	Excel_workBook.SaveAs(_variant_t(Excel_path), covOptional, covOptional, covOptional, covOptional, covOptional
		, 0, covOptional, covOptional, covOptional, covOptional, covOptional);
	Excel_workBook.Close(covOptional, COleVariant(Excel_path), covOptional);
	Excel_workBooks.Close();
	Excel_app.Quit();
	mysql_free_result(result);
	((CMainFrame*)AfxGetMainWnd())->m_Record->Msg("Export data successed", TRUE);
	return ;
}