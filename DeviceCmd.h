#pragma once
#include "RTK_Pack.h"
#include "afxwin.h"

#define PORT_LIST_LEN	8
UINT pAutoTest(LPVOID lpParam);

// CExport 窗体视图

typedef struct{
	CString port;
	CString rate;
	CString mode;
	CString cmd;
	CString func_name;
	CString res;
}S_INFO;

class CExport : public CFormView
{
	DECLARE_DYNCREATE(CExport)

protected:
	CExport();           // 动态创建所使用的受保护的构造函数
	virtual ~CExport();

public:
	enum { IDD = IDD_DIALOG_EXPORT };
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

public:
	CApplication Excel_app;
	CWorkbook Excel_workBook;   
	CWorksheet Excel_workSheet;   
	CWorksheets Excel_workSheets;   
	CWorkbooks Excel_workBooks;   
	CRange Excel_range;   
	CRange Excel_range1;   
	char Excel_path[MAX_PATH];
	Cnterior inter;

	CString m_sProject;
	BOOL PreTranslateMessage(MSG* pMsg);
	
	void SearchComport();
	void getSpecialComNumFromDevInfo();
	void getSpecialComNumFromDevInfo_ex();
	BOOL parseComport(CString str,int &port);
	CString parseCmd();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnCbnSelchangeComboCom();

	BOOL TestProcess(int port);


	HINSTANCE hDLL;
	BOOL InitComDll(CString &sOutput);
	BOOL OpenComport(UINT port, CString rate, CString &sOutput);
	BOOL ReadComport(CString &sOutput);
	BOOL WriteComport(CString sInput, CString &sOutput);
	BOOL CloseComport(CString &sOutput);

	BOOL CExport::getCmd(CString func,CString mode);
	BOOL CExport::needVerify();


	afx_msg void OnCbnSelchangeComboMode();
	
	afx_msg LRESULT OnMyEvent(WPARAM wParam, LPARAM lParam);

	BOOL ComOpen();
	BOOL ComClose();
	BOOL ComSend(CString cmd);

	char cmd[5];
	CString lastCmd;
	CString lastMsg;

	void CExport::printLog(CString msg, bool res);
	int check_login_status();

	BOOL isProcessing;

	

public:
	afx_msg void OnClose();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);

	CStringArray devicePortList;
	CStringArray workPortList;
	void workPortListAdd(CString str);
	void devicePortListAdd(CString str);
	void initWorkPortList(CString str);
	void initWorkPortShow();
	int GetRand(int MIN, int MAX);
	void freshDevicePortList();
	BOOL isExistInDeviceArray(CString c);
	BOOL isExistInWorkArray(CString c);
	afx_msg void OnBnClickedCheckAll();
	void  listTestEnterance(); 
	BOOL isComChecked(int index);
	void setStatus(int index,CString status);
	void setProcessing(BOOL s);
	BOOL getProcessing();

	S_INFO	g_info;

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CBrush m_brush[8];
};


