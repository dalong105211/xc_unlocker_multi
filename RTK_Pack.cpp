// RTK_Pack.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "RTK_Pack.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include ".\rtk_pack.h"
#include "Login.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRTK_PackApp

BEGIN_MESSAGE_MAP(CRTK_PackApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_LOGIN, &CRTK_PackApp::OnLogin)
	ON_COMMAND(ID_LOGIN_LOGINOUT, &CRTK_PackApp::OnLoginLoginout)
END_MESSAGE_MAP()


// CRTK_PackApp 构造

CRTK_PackApp::CRTK_PackApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CRTK_PackApp 对象

CRTK_PackApp theApp;

// CRTK_PackApp 初始化

BOOL CRTK_PackApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControls()。否则，将无法创建窗口。
	InitCommonControls();

	CWinApp::InitInstance();

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	m_hMutex = ::CreateMutex(NULL, FALSE, _T("DEM"));//创建一个互斥对象，每一次程序打开的时候都会用            
	//CreateMutex函数创建
	//创建互斥对象，并以第三个参数命名（次命名为唯一标识），                                                 
	//如果存在则返回 ERROR_ALREADY_EXISTS

	if (GetLastError() == ERROR_ALREADY_EXISTS)

	{
		AfxMessageBox(_T("Application was already running"));
		return FALSE;
	}

	// 若要创建主窗口，此代码将创建新的框架窗口
	// 对象，然后将其设置为应用程序的主窗口对象
	CMDIFrameWnd* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// 创建主 MDI 框架窗口
	if (!pFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	// 试图加载共享 MDI 菜单和快捷键表
	//TODO: 添加附加成员变量，并加载对
	//	应用程序可能需要的附加菜单类型的调用
	HINSTANCE hInst = AfxGetResourceHandle();
	m_hMDIMenu_login = ::LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU_LOGIN));
	m_hMDIMenu_admin = ::LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU_AMDIN));
	m_hMDIMenu_user = ::LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU_USER));

	m_hMenu=::LoadMenu(hInst, MAKEINTRESOURCE(IDR_MAINFRAME));
	m_hMDIAccel = ::LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_RTK_PackTYPE));
	// 主窗口已初始化，因此显示它并对其进行更新

	pFrame->ShowWindow(SW_SHOW);
	CString Str;
	//Str = "Augent Tool " + ((CMainFrame*)AfxGetMainWnd())->GetFileVersion();
	Str = "Augent Tool v" + (((CMainFrame*)AfxGetMainWnd())->GetFileVersion()).Right(3);
	pFrame->SetWindowText(Str);
	//((CMainFrame*)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0,"状态: 未连接",TRUE);

	//pFrame->ShowWindow(m_nCmdShow);
	//pFrame->ShowWindow(SW_SHOWMAXIMIZED);
	pFrame->UpdateWindow();

	
	SetMenu(AfxGetMainWnd()->m_hWnd, m_hMDIMenu_login);

	return TRUE;
}


// CRTK_PackApp 消息处理程序

int CRTK_PackApp::ExitInstance() 
{
	CString sOutput;
	//((CMainFrame*)AfxGetMainWnd())->CloseComport(sOutput);
	//TODO: 处理可能已添加的附加资源
	if (m_hMDIMenu_login != NULL)
		FreeResource(m_hMDIMenu_login);
	if (m_hMDIMenu_admin != NULL)
		FreeResource(m_hMDIMenu_admin);
	if (m_hMDIMenu_user != NULL)
		FreeResource(m_hMDIMenu_user);
	if (m_hMDIAccel != NULL)
		FreeResource(m_hMDIAccel);

	return CWinApp::ExitInstance();
}

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CRTK_PackApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CRTK_PackApp 消息处理程序




void CRTK_PackApp::OnLogin()
{
	// TODO:  在此添加命令处理程序代码
	CLogin m_login;
	if (m_login.DoModal() == IDCANCEL)
	{
		return;
	}
	else
	{
		if (g_user.type == 1)
		{
			SetMenu(AfxGetMainWnd()->m_hWnd, m_hMDIMenu_admin);
			/*if (m_pMainWnd != NULL)
				m_pMainWnd->UpdateWindow();*/
		}
		else
			SetMenu(AfxGetMainWnd()->m_hWnd, m_hMDIMenu_user);


		//登录后默认打开操作界面
		CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
		if (pFrame == NULL)
			return;
		pFrame->OnFunOper();
	}
}



void CRTK_PackApp::OnLoginLoginout()
{
	// TODO:  在此添加命令处理程序代码
	if (m_hMDIMenu_login != NULL)
		FreeResource(m_hMDIMenu_login);
	if (m_hMDIMenu_admin != NULL)
		FreeResource(m_hMDIMenu_admin);
	if (m_hMDIMenu_user != NULL)
		FreeResource(m_hMDIMenu_user);
	if (m_hMDIAccel != NULL)
		FreeResource(m_hMDIAccel);
	
	if (IDYES == AfxMessageBox("Confirm Logout?", MB_YESNO | MB_ICONQUESTION))
	{
		/*if (((CMainFrame*)AfxGetMainWnd())->m_Carton != NULL)
		{
			((CMainFrame*)AfxGetMainWnd())->m_Carton->DestroyWindow();
		}*/

		if (((CMainFrame*)AfxGetMainWnd())->m_Export != NULL)
		{
			((CMainFrame*)AfxGetMainWnd())->m_Export->DestroyWindow();
		}

		/*if (((CMainFrame*)AfxGetMainWnd())->m_Record != NULL)
		{
			((CMainFrame*)AfxGetMainWnd())->m_Record->DestroyWindow();
		}*/
		SetMenu(AfxGetMainWnd()->m_hWnd, m_hMDIMenu_login);

		((CMainFrame*)AfxGetMainWnd())->SetStatusBar("None", "0", "0", "0", "0","0");
	}
}



void CRTK_PackApp::LogOut()
{
	// TODO:  在此添加命令处理程序代码
	if (m_hMDIMenu_login != NULL)
		FreeResource(m_hMDIMenu_login);
	if (m_hMDIMenu_admin != NULL)
		FreeResource(m_hMDIMenu_admin);
	if (m_hMDIMenu_user != NULL)
		FreeResource(m_hMDIMenu_user);
	if (m_hMDIAccel != NULL)
		FreeResource(m_hMDIAccel);

	if (((CMainFrame*)AfxGetMainWnd())->m_Export != NULL)
	{
		((CMainFrame*)AfxGetMainWnd())->m_Export->DestroyWindow();
	}

	SetMenu(AfxGetMainWnd()->m_hWnd, m_hMDIMenu_login);

	CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
	if (pFrame == NULL)
		return;
	pFrame->SetStatusBar("None", "0", "0", "0", "0", "0");	
}