// ChildFrm.cpp : CChildFrame 类的实现
//
#include "stdafx.h"
#include "RTK_Pack.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	ON_WM_SETFOCUS()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CChildFrame 构造/析构

CChildFrame::CChildFrame()
{
	// TODO: 在此添加成员初始化代码

	i_Line = 0;

}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或样式
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}


// CChildFrame 诊断

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG


// CChildFrame 消息处理程序
void CChildFrame::OnFileClose() 
{
	// 若要关闭框架，只需发送 WM_CLOSE
	//，这相当于从系统菜单中选择关闭。
	SendMessage(WM_CLOSE);
}

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// 创建一个视图以占用框架的工作区
	//if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, 
	//	CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	//{
	//	TRACE0("未能创建视图窗口\n");
	//	return -1;
	//}

	//创建对话条
	if(!m_wndBar.Create(this,IDD_DIALOG_MSG,WS_CHILD|WS_VISIBLE|
		CBRS_BOTTOM|CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | 
		CBRS_SIZE_DYNAMIC,IDD_DIALOG_MSG))
	{
		TRACE0("Failed to create dialogbar\n");
		return -1;      // fail to create
	}

	//隐藏和显示CListBox控件
	CListCtrl * pListCtrl=(CListCtrl *)m_wndBar.GetDlgItem(IDC_LIST_MSG);
	pListCtrl->SetWindowPos(NULL,0,0,GetSystemMetrics(SM_CXSCREEN)-11,132,SWP_NOMOVE|SWP_NOZORDER | SWP_NOACTIVATE|SWP_SHOWWINDOW);

	//操纵对话条上的CTabCtrl控件
	pListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT);
	pListCtrl->InsertColumn(0,"DateTime",LVCFMT_LEFT,140);
	pListCtrl->InsertColumn(1,"Status",LVCFMT_LEFT,750);

	pImagelist = new CImageList();
	pImagelist->Create(16,16,ILC_COLOR32,0,2);
	pImagelist->Add(AfxGetApp()->LoadIcon(IDI_ICON_PASS));
	pImagelist->Add(AfxGetApp()->LoadIcon(IDI_ICON_FAIL));

	CBitmap bmp;
	pImagelist->Add(&bmp,RGB(0,0,0));
	pListCtrl->SetImageList(pImagelist,LVSIL_SMALL);
	pListCtrl->SetImageList(pImagelist,LVSIL_SMALL);

	return 0;
}

void CChildFrame::OnSetFocus(CWnd* pOldWnd) 
{
	CMDIChildWnd::OnSetFocus(pOldWnd);

	//m_wndView.SetFocus();
}

BOOL CChildFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// 让视图第一次尝试该命令
	//if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
	//	return TRUE;
	
	// 否则，执行默认处理
	return CMDIChildWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CChildFrame::Msg(CString Str,BOOL Status) 
{
	CListCtrl * pListCtrl=(CListCtrl *)m_wndBar.GetDlgItem(IDC_LIST_MSG);
	CTime time = CTime::GetCurrentTime();
	if (Status)
	{
		pListCtrl->InsertItem(i_Line,time.Format("[%Y-%m-%d %H:%M:%S] "),0);
	}
	else
	{
		pListCtrl->InsertItem(i_Line,time.Format("[%Y-%m-%d %H:%M:%S] "),1);
	}
	pListCtrl->SetItemText(i_Line,1,Str);
	pListCtrl->EnsureVisible(i_Line,TRUE);
	i_Line++;
	/*if (pListCtrl)
		free(pListCtrl);*/
	return;
}