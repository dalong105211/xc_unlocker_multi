#pragma once
#include "GridCtrl.h"
#include "afxwin.h"
#include "BtnST.h"
#include "afxcmn.h"

// CRecord 窗体视图

class CRecord : public CFormView
{
	DECLARE_DYNCREATE(CRecord)

protected:
	CRecord();           // 动态创建所使用的受保护的构造函数
	virtual ~CRecord();

public:
	enum { IDD = IDD_DIALOG_OUTER };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	afx_msg void OnDestroy();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();

	BOOL PreTranslateMessage(MSG* pMsg);

public:
	CGridCtrl* m_pGridCtrl;
	CComboBox m_ComboBox_Product;
	GV_ITEM Item;
	afx_msg void OnBnClickedButtonSelect();

	bool drawTab(CStringArray &tab, CString sql);
	void exportTab(CStringArray &tab, CString sql, CString tabName);
	afx_msg void OnCbnSelchangeComboTab();
	void UpdateUserList(CString tab);
	afx_msg void OnBnClickedButtonExport();

	CApplication Excel_app;
	CWorkbook Excel_workBook;
	CWorksheet Excel_workSheet;
	CWorksheets Excel_workSheets;
	CWorkbooks Excel_workBooks;
	CRange Excel_range;
	CRange Excel_range1;
	Cnterior inter;
	int Progress_Pos;
	char Excel_path[MAX_PATH];
	CProgressCtrl m_Progress;
};


