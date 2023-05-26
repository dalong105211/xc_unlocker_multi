// DeviceCmd.cpp :

#include "stdafx.h"
#include "DeviceCmd.h"

#include "MainFrm.h"
#include ".\DeviceCmd.h"
#include <comutil.h>
#include <Windows.h>
#include <iostream>
#include "comdef.h"
#include "Shlwapi.h"
#include "VerifyCode.h"

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <Windows.h>
#include <setupapi.h>
#pragma comment(lib, "setupapi.lib")

// CExport


/*
Unlocker Tool 串口通讯协议
Select Tool	Select Function	Request(0x58 0x43 0x05)	Response(0x58 0x43 0x06)
01_unlock_tool	Unlock	0x58 0x43 0x05 0x01 0x01	0x58 0x43 0x06 0x01 0x01
02_switch_mode_tool	Debug	0x58 0x43 0x05 0x02 0x01	0x58 0x43 0x06 0x02 0x01
Release	0x58 0x43 0x05 0x02 0x02	0x58 0x43 0x06 0x02 0x02
03_adb_tool	Lock	0x58 0x43 0x05 0x03 0x01	0x58 0x43 0x06 0x03 0x01
Unlock	0x58 0x43 0x05 0x03 0x02	0x58 0x43 0x06 0x03 0x02
04_pub	Delete PUK	0x58 0x43 0x05 0x04 0x01	0x58 0x43 0x06 0x04 0x01
05_general	Delete CustRes	0x58 0x43 0x05 0x05 0x01	0x58 0x43 0x06 0x05 0x01
说明：CMD固定byte[5]长的数组 -- Head:0x58 0x43 0x05 + Data:0xXX 0xXX

*/


#define AT_REQU_HEADER	"\x58\x43\x05 "
#define AT_RESP_HEADER	"58 43 06 "

HANDLE hMutex;
#define PORT_MTK_DESC		"MediaTek USB VCOM"
#define PORT_DEFAULT_DESC	"None"
//GUID g_mtk;

IMPLEMENT_DYNCREATE(CExport, CFormView)
CExport::CExport()
	: CFormView(CExport::IDD)
{
}

CExport::~CExport()
{
}

void CExport::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CExport, CFormView)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_START, &CExport::OnBnClickedButtonStart)
	ON_CBN_SELCHANGE(IDC_COMBO_COM, &CExport::OnCbnSelchangeComboCom)
	ON_CBN_SELCHANGE(IDC_COMBO_MODE, &CExport::OnCbnSelchangeComboMode)
	ON_MESSAGE(WM_MY_EVENT, OnMyEvent)
	ON_WM_CLOSE()
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDC_CHECK_ALL, &CExport::OnBnClickedCheckAll)
END_MESSAGE_MAP()


// CExport

#ifdef _DEBUG
void CExport::AssertValid() const
{
	CFormView::AssertValid();
}

void CExport::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

COleVariant
	covTrue((short)TRUE),
	covFalse((short)FALSE),
	covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);


void CExport::OnDestroy() 
{
	CString sOutput;
	CloseComport(sOutput);
	CFormView::OnDestroy();
	((CMainFrame*)AfxGetMainWnd())->m_Export=NULL;
}

void CExport::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	CString str;
	//str = "Augent Tool " + ((CMainFrame*)AfxGetMainWnd())->GetFileVersion();
	str = "v" + (((CMainFrame*)AfxGetMainWnd())->GetFileVersion()).Right(3);
	GetParentFrame()->SetWindowText(str);

	/*********************************************read config***********************************************************/
	CString sFileName;
	int tStatus;
	char key[128];
	GetCurrentDirectory(MAX_PATH, sFileName.GetBuffer(MAX_PATH));
	sFileName.Format("%s\\Config.ini", sFileName);

	//add comport rate
	((CComboBox*)GetDlgItem(IDC_COMBO_RATE))->ResetContent();
	for (int i = 0; i < 10; i++)
	{
		//load config
		str.Format("rate_%d", i);
		tStatus = GetPrivateProfileString("com", str, "", key, 128, sFileName);
		if (tStatus > 0)
		{
			((CComboBox*)GetDlgItem(IDC_COMBO_RATE))->AddString(key);
		}
	}
	((CComboBox*)GetDlgItem(IDC_COMBO_RATE))->SetCurSel(0);

	//add comport rate
	((CComboBox*)GetDlgItem(IDC_COMBO_RATE))->ResetContent();
	for (int i = 0; i < 10; i++)
	{
		//load config
		str.Format("rate_%d", i);
		tStatus = GetPrivateProfileString("com", str, "", key, 128, sFileName);
		if (tStatus > 0)
		{
			((CComboBox*)GetDlgItem(IDC_COMBO_RATE))->AddString(key);
		}
	}
	((CComboBox*)GetDlgItem(IDC_COMBO_RATE))->SetCurSel(0);

	CRTK_PackApp *app = (CRTK_PackApp *)AfxGetApp(); //生成指向应用程序类的指针
	((CComboBox*)GetDlgItem(IDC_COMBO_MODE))->ResetContent();
	//管理员全打开
	if (app->g_user.type == 1)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_MODE))->AddString("01_remove_tamper");
		((CComboBox*)GetDlgItem(IDC_COMBO_MODE))->AddString("02_switch_mode_tool");
		((CComboBox*)GetDlgItem(IDC_COMBO_MODE))->AddString("03_adb_tool");
		((CComboBox*)GetDlgItem(IDC_COMBO_MODE))->AddString("04_puk");
		((CComboBox*)GetDlgItem(IDC_COMBO_MODE))->AddString("05_close");
	}
	else
	{
		if (app->g_user.power.module1 == true)
			((CComboBox*)GetDlgItem(IDC_COMBO_MODE))->AddString("01_remove_tamper");
		if (app->g_user.power.module2 == true)
			((CComboBox*)GetDlgItem(IDC_COMBO_MODE))->AddString("02_switch_mode_tool");
		if (app->g_user.power.module3 == true)
			((CComboBox*)GetDlgItem(IDC_COMBO_MODE))->AddString("03_adb_tool");
		if (app->g_user.power.module4 == true)
			((CComboBox*)GetDlgItem(IDC_COMBO_MODE))->AddString("04_puk");
		if (app->g_user.power.module5 == true)
			((CComboBox*)GetDlgItem(IDC_COMBO_MODE))->AddString("05_custres");
		//((CComboBox*)GetDlgItem(IDC_COMBO_MODE))->AddString("05_close");
	}

	//((CComboBox*)GetDlgItem(IDC_COMBO_ACTIONS))->SetCurSel(0);

	CString sOutput;
	if (!(InitComDll(sOutput)))
	{
		return;
	}
	/*g_mtk.Data1 = { 0x4d36e978 };
	g_mtk.Data2 = { 0xe325 };
	g_mtk.Data3 = { 0x11ce };
	g_mtk.Data4[0] = { 0xbf };
	g_mtk.Data4[1] = { 0xc1 };
	g_mtk.Data4[2] = { 0x08 };
	g_mtk.Data4[3] = { 0x00 };
	g_mtk.Data4[4] = { 0x2b };
	g_mtk.Data4[5] = { 0xe1 };
	g_mtk.Data4[6] = { 0x03 };
	g_mtk.Data4[7] = { 0x18 };

*/
	
	//SearchComport();
	//getSpecialComNumFromDevInfo();
	initWorkPortList(PORT_DEFAULT_DESC);

	devicePortList.RemoveAll();
	getSpecialComNumFromDevInfo_ex();
	freshDevicePortList();
	initWorkPortShow();
	//((CComboBox*)GetDlgItem(IDC_COMBO_COM))->SetCurSel(0);
	//m_SerialPort.connectReadEvent(this);
	
}

void CExport::getSpecialComNumFromDevInfo()
{
	CString sOutput;
	HDEVINFO hDevInfo = SetupDiGetClassDevs(NULL, NULL, NULL, DIGCF_PRESENT | DIGCF_ALLCLASSES);
	if (hDevInfo == INVALID_HANDLE_VALUE)
	{
		sOutput.Format("SetupDiGetClassDevs Err:%d", GetLastError());
		printLog(sOutput, false);
		return ;
	};

	SP_CLASSIMAGELIST_DATA _spImageData = { 0 };
	_spImageData.cbSize = sizeof(SP_CLASSIMAGELIST_DATA);
	SetupDiGetClassImageList(&_spImageData);

	SP_DEVINFO_DATA spDevInfoData = { 0 };
	spDevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	for (DWORD i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &spDevInfoData); i++)
	{
		char  szBuf[MAX_PATH] = { 0 };
		int  wImageIdx = 0;
		short  wItem = 0;
		if (!SetupDiGetDeviceRegistryPropertyA(hDevInfo, &spDevInfoData, SPDRP_CLASS, NULL, (PBYTE)szBuf, MAX_PATH, 0))
		{
			continue;
		};
		if (strcmp(szBuf, "Ports") != 0) //只取端口
		{
			continue;
		}
		if (SetupDiGetClassImageIndex(&_spImageData, &spDevInfoData.ClassGuid, &wImageIdx))
		{
			char  szName[MAX_PATH] = { 0 };
			DWORD  dwRequireSize;

			if (!SetupDiGetClassDescription(&spDevInfoData.ClassGuid, (PSTR)szBuf, MAX_PATH, &dwRequireSize))
			{
				continue;
			};

			if (SetupDiGetDeviceRegistryProperty(hDevInfo, &spDevInfoData, SPDRP_FRIENDLYNAME, NULL, (PBYTE)szName, MAX_PATH - 1, 0))
			{
				//wprintf(L"FriendlyName:%s\r\n\r\n", szName);
				((CComboBox*)GetDlgItem(IDC_COMBO_COM))->AddString(szName);
			}
			else  if (SetupDiGetDeviceRegistryProperty(hDevInfo, &spDevInfoData, SPDRP_DEVICEDESC, NULL, (PBYTE)szName, MAX_PATH - 1, 0))
			{
				((CComboBox*)GetDlgItem(IDC_COMBO_COM))->AddString(szName);
			}
		}
	}
	SetupDiDestroyClassImageList(&_spImageData);
}


void CExport::getSpecialComNumFromDevInfo_ex()
{
	CString sOutput;
	HDEVINFO hDevInfo = SetupDiGetClassDevs(NULL, NULL, NULL, DIGCF_PRESENT | DIGCF_ALLCLASSES);
	if (hDevInfo == INVALID_HANDLE_VALUE)
	{
		sOutput.Format("SetupDiGetClassDevs Err:%d", GetLastError());
		printLog(sOutput, false);
		return;
	};

	SP_CLASSIMAGELIST_DATA _spImageData = { 0 };
	_spImageData.cbSize = sizeof(SP_CLASSIMAGELIST_DATA);
	SetupDiGetClassImageList(&_spImageData);

	SP_DEVINFO_DATA spDevInfoData = { 0 };
	spDevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	for (DWORD i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &spDevInfoData); i++)
	{
		char  szBuf[MAX_PATH] = { 0 };
		int  wImageIdx = 0;
		short  wItem = 0;
		if (!SetupDiGetDeviceRegistryPropertyA(hDevInfo, &spDevInfoData, SPDRP_CLASS, NULL, (PBYTE)szBuf, MAX_PATH, 0))
		{
			continue;
		};
		if (strcmp(szBuf, "Ports") != 0) //只取端口
		{
			continue;
		}
		if (SetupDiGetClassImageIndex(&_spImageData, &spDevInfoData.ClassGuid, &wImageIdx))
		{
			char  szName[MAX_PATH] = { 0 };
			DWORD  dwRequireSize;

			if (!SetupDiGetClassDescription(&spDevInfoData.ClassGuid, (PSTR)szBuf, MAX_PATH, &dwRequireSize))
			{
				continue;
			};

			if (SetupDiGetDeviceRegistryProperty(hDevInfo, &spDevInfoData, SPDRP_FRIENDLYNAME, NULL, (PBYTE)szName, MAX_PATH - 1, 0))
			{
				devicePortListAdd(szName);
			}
			else  if (SetupDiGetDeviceRegistryProperty(hDevInfo, &spDevInfoData, SPDRP_DEVICEDESC, NULL, (PBYTE)szName, MAX_PATH - 1, 0))
			{
				devicePortListAdd(szName);
			}
		}
	}
	SetupDiDestroyClassImageList(&_spImageData);
}

BOOL CExport::PreTranslateMessage(MSG* pMsg)
{
	CString Str;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		return TRUE;
	}
	if (pMsg->message == MY_THREAD_MSG)
	{
		printLog(lastMsg, false);
		Logger("DeviceCmd", lastMsg);
		return TRUE;
	}

	return CFormView::PreTranslateMessage(pMsg);

}

//register
void CExport::SearchComport()
{
	CString str;
	HKEY   hKey;
	((CComboBox*)GetDlgItem(IDC_COMBO_COM))->ResetContent();

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Hardware\\DeviceMap\\SerialComm"), NULL, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		TCHAR       szPortName[256], szComName[256];
		DWORD       dwLong, dwSize;
		int         nCount = 0;
		while (true)
		{
			dwLong = dwSize = 256;
			if (RegEnumValue(hKey, nCount, szPortName, &dwLong, NULL, NULL, (PUCHAR)szComName, &dwSize) == ERROR_NO_MORE_ITEMS)
				break;
		
			str.Format(_T("%s"), szComName);
			((CComboBox*)GetDlgItem(IDC_COMBO_COM))->AddString(str);
			nCount++;
			dwLong = sizeof(szPortName);
			dwSize = sizeof(szComName);
		}

		RegCloseKey(hKey);
	}
}


void CExport::OnCbnSelchangeComboCom()
{
	CString sOutput;
	CloseComport(sOutput);
	/*((CComboBox*)GetDlgItem(IDC_COMBO_COM))->ResetContent();
	getSpecialComNumFromDevInfo();
	((CComboBox*)GetDlgItem(IDC_COMBO_COM))->SetCurSel(0);*/
}


UINT pAutoTest(LPVOID lpParam)
{
	WaitForSingleObject(hMutex, INFINITE);
	CExport* pToolForDlg = (CExport*)lpParam;

	pToolForDlg->listTestEnterance();

	ReleaseMutex(hMutex);
	return 0;

}

BOOL CExport::isComChecked(int index)
{
	BOOL isEnable = false;
	switch (index)
	{
		case 0:
			isEnable = (BOOL)((CButton *)GetDlgItem(IDC_CHECK_COM1))->GetCheck();
			break;
		case 1:
			isEnable = (BOOL)((CButton *)GetDlgItem(IDC_CHECK_COM2))->GetCheck();
			break;
		case 2:
			isEnable = (BOOL)((CButton *)GetDlgItem(IDC_CHECK_COM3))->GetCheck();
			break;
		case 3:
			isEnable = (BOOL)((CButton *)GetDlgItem(IDC_CHECK_COM4))->GetCheck();
			break;
		case 4:
			isEnable = (BOOL)((CButton *)GetDlgItem(IDC_CHECK_COM5))->GetCheck();
			break;
		case 5:
			isEnable = (BOOL)((CButton *)GetDlgItem(IDC_CHECK_COM6))->GetCheck();
			break;
		case 6:
			isEnable = (BOOL)((CButton *)GetDlgItem(IDC_CHECK_COM7))->GetCheck();
			break;
		case 7:
			isEnable = (BOOL)((CButton *)GetDlgItem(IDC_CHECK_COM8))->GetCheck();
			break;	
		default:
			isEnable = FALSE;
			break;
	}
	return isEnable;
}

void CExport::listTestEnterance()
{
	int port;
	((CComboBox*)GetDlgItem(IDC_COMBO_RATE))->GetWindowTextA(g_info.rate);
	((CComboBox*)GetDlgItem(IDC_COMBO_FUNCTION))->GetWindowTextA(g_info.func_name);
	((CComboBox*)GetDlgItem(IDC_COMBO_MODE))->GetWindowTextA(g_info.mode);


	if (getCmd(g_info.func_name,g_info.mode) == FALSE)
	{
		printLog("Parse comand error.", false);
		return;
	}
	//g_info.cmd = cmd;

	for (int i = 0; i < 8; i++)
	{
		if (isComChecked(i) == TRUE && workPortList[i].Find(PORT_MTK_DESC) > -1 && parseComport(workPortList[i], port) == TRUE)
		{
			TestProcess(port);
		}
	}
	((CComboBox*)GetDlgItem(IDC_BUTTON_START))->EnableWindow(true);
}


BOOL CExport::TestProcess(int port)
{
	CRTK_PackApp *app = (CRTK_PackApp *)AfxGetApp(); //生成指向应用程序类的指针
	CString user = app->g_user.name;
	CString str, fun_index, sOutput;
	CString sResp;

	if (!(OpenComport(port, g_info.rate, sOutput)))
	{
		printLog(sOutput, false);
		Logger("DeviceCmd", sOutput);
		goto end;
	}
	if (!(WriteComport(g_info.cmd, sOutput)))
	{
		if (!(CloseComport(sOutput)))
		{
			printLog(sOutput, false);
			Logger("DeviceCmd", sOutput);
		}
		goto end;
	}
	Sleep(200);
	if (!(ReadComport(sOutput)))
	{
		if (!(CloseComport(sOutput)))
		{
			printLog(sOutput, false);
			Logger("DeviceCmd", sOutput);
		}
		goto end;
	}
	else//处理log上传
	{
		bool res;
		CString out;
		if (strncmp(sOutput, g_info.cmd, 2) == 0)
		{
			res = true;
			out = "success";
		}
		else
		{
			res = false;
			out = "fail";
		}
		CString str;
		str.Format("%s %s", g_info.func_name, out);
		printLog(str, res);
		if (needVerify() == TRUE)
		{
			if (res == false)
				printLog("Switch to debug mode failed", res);
			else
				printLog("Switch to debug mode successed", res);
		}

		//CString sOutput;
		//CString sResp;
		//check login status

		if (api_insert_cmd_record(g_info.func_name, out, sResp) == FALSE)
		{
			printLog(sResp, false);
			Logger("DeviceCmd", sResp);
			goto end;
		}
		if (parseJson(sResp, "res", 0, 0, "", sOutput) == FALSE)
		{
			sOutput = "Parse res fail";
			printLog(sOutput, false);
			goto end;
		}
		if (sOutput != "1")
		{
			if (parseJson(sResp, "msg", 0, 0, "", sOutput) == FALSE)
			{
				sOutput = "Login fail,query error msg fail";
			}
			printLog(sOutput, false);
			goto end;
		}
	}
	
	//
	if (!(CloseComport(str)))
	{
		printLog(str, false);
		Logger("DeviceCmd", sOutput);
	}
	return TRUE;
end:
	CloseComport(sOutput);
	return FALSE;
}

//0-正常，-1-异地登陆，-2-请求失败,-3-解析失败
int CExport::check_login_status()
{
	CString sResp, sOutput;
	if (api_check_login(sResp) == FALSE)
	{
		Logger("DeviceCmd", sResp);
		return -2;
	}
	if (parseJson(sResp, "res", 0, 0, "", sOutput) == FALSE)
	{
		sOutput = "Parse res fail";
		printLog(sOutput, false);
		return -3;
	}
	if (sOutput != "1")
	{
		if (parseJson(sResp, "msg", 0, 0, "", sOutput) == FALSE)
		{
			sOutput = "Login fail,query error msg fail";
		}
		printLog(sOutput, false);
		AfxMessageBox("Account has been logged in elsewhere,click to logout.", MB_OK | MB_ICONINFORMATION);
		return -1;
	}
	return 0;
}

void CExport::OnBnClickedButtonStart()
{
	//先disable发送按钮
	((CComboBox*)GetDlgItem(IDC_BUTTON_START))->EnableWindow(false);

	//check login
	int res = check_login_status();
	if (res == -1)	//异地登录
	{
		CRTK_PackApp *app = (CRTK_PackApp *)AfxGetApp();
		app->LogOut();
		return;
	}
	else if (res == -2 || res == -3)	//请求或解析失败
	{
		((CComboBox*)GetDlgItem(IDC_BUTTON_START))->EnableWindow(true);
		return;
	}
	CWinThread *pThread = AfxBeginThread(pAutoTest, this);
}

BOOL CExport::InitComDll(CString &sOutput)
{
	//动态调用DLL
	hDLL = NULL;  // establish a handle for executable module.
	//获取路径
	CString m_path;
	GetCurrentDirectory(MAX_PATH, m_path.GetBuffer(MAX_PATH));
	m_path.Format("%s\\ComObj00.dll", m_path);

	hDLL = LoadLibrary(m_path);  // load the dll program from location 
	if (hDLL == NULL)  // if dll program did load
	{
		AfxMessageBox("Init ComObj00.dll error", MB_ICONERROR | MB_TOPMOST);
		printLog("Init ComObj00.dll error", false);
		Logger("DeviceCmd",sOutput);
		return FALSE;
	}
	return TRUE;
}

BOOL CExport::OpenComport(UINT port, CString rate, CString &sOutput)
{
	//LoadLibrary
	typedef void(*OPENCOMPORT)(const CString sInput, CString &sOutput);
	typedef long(*CLEARCOMPORT)(const CString sInput, CString &sOutput);
	typedef long(*SETCOMSTATE)(const CString sInput, CString &sOutput);

	OPENCOMPORT OpenComPort;  // establish an object for interface to dll
	SETCOMSTATE SetComState;  // establish an object for interface to dll
	CLEARCOMPORT ClearComPort;  // establish an object for interface to dll


	//OpenComport
	OpenComPort = (OPENCOMPORT)GetProcAddress(hDLL, "OpenComPort");  // get process address for dll
	if (OpenComPort == NULL)  // if successful in getting process address 
	{
		sOutput = "Init OpenComPort error";
		return FALSE;
	}


	if (port <= 0)
	{
		sOutput = "Comport is invalid";
		return FALSE;
	}
	CString Str;
	Str.Format("%d", port);
	OpenComPort(Str, sOutput);

	//ClearComPort
	ClearComPort = (CLEARCOMPORT)GetProcAddress(hDLL, "ClearComPort");  // get process address for dll
	if (ClearComPort == NULL)  // if successful in getting process address 
	{
		sOutput = "Init ClearComPort error";
		return FALSE;
	}
	if (ClearComPort(NULL, sOutput) == 0)
	{
		sOutput.Format("ClearComPort error:%s", sOutput);
		return FALSE;
	}

	//SetComState
	SetComState = (SETCOMSTATE)GetProcAddress(hDLL, "SetComState");  // get process address for dll
	if (SetComState == NULL)  // if successful in getting process address 
	{
		sOutput.Format("Init SetComState error");
		return FALSE;
	}
	Str.Format("%s,0,8,0", rate);
	if (SetComState(Str, sOutput) == 0)
	{
		sOutput.Format("SetComState error:%s",sOutput);
		return FALSE;
	}
	return TRUE;
}

BOOL CExport::ReadComport(CString &sOutput)
{
	typedef long(*REECEIVESTR)(const CString sInput, CString &sOutput);
	//typedef long(*CLEARCOMPORT)(const CString sInput, CString &sOutput);
	REECEIVESTR ReceiveStr;  // establish an object for interface to dll
	//CLEARCOMPORT ClearComPort;  // establish an object for interface to dll

	ReceiveStr = (REECEIVESTR)GetProcAddress(hDLL, "ReceiveHex");
	if (ReceiveStr == NULL)  // if successful in getting process address 
	{
		sOutput.Format("Init ReceiveStr error");
		return FALSE;
	}
	if (ReceiveStr("", sOutput) == 0)
	{
		sOutput.Format("ReceiveStr error:%s", sOutput);
		return FALSE;
	}
	return TRUE;
}


BOOL CExport::WriteComport(CString str, CString &sOutput)
{

	typedef long(*SENDSTR)(const CString sInput, CString &sOutput);


	SENDSTR SendStr;

	//SEND STR
	SendStr = (SENDSTR)GetProcAddress(hDLL, "SendHex");  // get process address for dll
	if (SendStr == NULL)  // if successful in getting process address 
	{
		sOutput.Format("Init SendStr error");
		return FALSE;
	}
	if (SendStr(str, sOutput) == 0)
	{
		sOutput.Format("SendStr error:%s", sOutput);
		return FALSE;
	}

	return TRUE;
}
BOOL CExport::CloseComport(CString &sOutput)
{
	typedef long(*CLOSECOMPORT)(const CString sInput, CString &sOutput);
	CLOSECOMPORT CloseComPort;  // establish an object for interface to dll

	CloseComPort = (CLOSECOMPORT)GetProcAddress(hDLL, "CloseComPort");
	if (CloseComPort == NULL)  // if successful in getting process address 
	{
		sOutput.Format("Init CloseComPort error");
		return FALSE;
	}
	if (CloseComPort(NULL, sOutput) == 0)  // if successful in getting process address 
	{
		sOutput.Format("CloseComPort error:%s", sOutput);
		return FALSE;
	}
	return TRUE;
}

void CExport::OnCbnSelchangeComboMode()
{
	// TODO:  在此添加控件通知处理程序代码
	CString sFileName,str,sec;
	int tStatus;
	char key[128];
	((CComboBox*)GetDlgItem(IDC_COMBO_MODE))->GetWindowTextA(sec);
	GetCurrentDirectory(MAX_PATH, sFileName.GetBuffer(MAX_PATH));
	sFileName.Format("%s\\Config.ini", sFileName);

	//add comport rate
	((CComboBox*)GetDlgItem(IDC_COMBO_FUNCTION))->ResetContent();
	for (int i = 0; i < 10; i++)
	{
		//load config
		str.Format("func_0%d", i);
		tStatus = GetPrivateProfileString(sec, str, "", key, 128, sFileName);
		if (tStatus > 0)
		{
			((CComboBox*)GetDlgItem(IDC_COMBO_FUNCTION))->AddString(key);
		}
	}
	//((CComboBox*)GetDlgItem(IDC_COMBO_FUNCTION))->SetCurSel(0);
}


BOOL CExport::parseComport(CString str,int &port)
{
	
	int index = str.MakeUpper().Find("(COM");

	if (index > 0)
	{
		for (int i = 0; i < str.GetLength(); i++)
		{
			/*CString temp;
			temp = str[index + 4 + i];*/
			if (str[index + 4 + i] >= '0' && str[index + 4 + i] <= '9')
			{
				continue;
			}
			else
			{
				str = str.Mid(index + 1,3+i);
				break;
			}
		}
		/*str = str.Right(str.GetLength() - str.Find("(COM"));
		str = str.Left(str.Find(")"));
		str.Replace("(", "");
		str.Replace(")", "");*/
		str.Replace("COM", "");
		port = atoi(str);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// 父窗口发来的消息，进行响应
LRESULT CExport::OnMyEvent(WPARAM wParam, LPARAM lParam)
{
	if (TRUE == isProcessing)
	{
		Logger("DeviceCmd", "It is in process. ignor new comport insert or mount.");
		return 0;
	}
	//BOOL status = (BOOL)wParam;
	Logger("DeviceCmd", "Get main message to refresh comport list");
	//((CComboBox*)GetDlgItem(IDC_COMBO_COM))->ResetContent();
	//getSpecialComNumFromDevInfo();
	devicePortList.RemoveAll();
	getSpecialComNumFromDevInfo_ex();
	freshDevicePortList();
	initWorkPortShow();
	//((CComboBox*)GetDlgItem(IDC_COMBO_COM))->SetCurSel(0);
	return 0;
}


/*
void CExport::onReadEvent(const char *portName, unsigned int readBufferLen)
{
	if (readBufferLen > 0)
	{
		char *data = new char[readBufferLen + 1]; // '\0'

		if (data)
		{
			int recLen = m_SerialPort.readData(data, readBufferLen);

			if (recLen > 0)
			{
				data[recLen] = '\0';
				bool res;
				CString out;
				if (strncmp(data, cmd, 2) == 0)
				{
					res = true;
					out = "success";
				}
				else
				{
					res = false;
					out = "fail";
				}
				CString func,str;
				((CComboBox*)GetDlgItem(IDC_COMBO_FUNCTION))->GetWindowTextA(func);
				str.Format("%s %s", func, out);
				printLog(str, res);
				if (needVerify() == TRUE)
				{
					if (res == false)
						printLog("Switch to debug mode failed", res);
					else
						printLog("Switch to debug mode successed", res);
				}

				CString sOutput;
				CString sResp;
				//check login status
				
				if (api_insert_cmd_record(func, out, sResp) == FALSE)
				{
					printLog(sResp, false);
					Logger("DeviceCmd", sResp);
					return;
				}
				if (parseJson(sResp, "res", 0, 0, "", sOutput) == FALSE)
				{
					sOutput = "Parse res fail";
					printLog(sOutput, false);
					return;
				}
				if (sOutput != "1")
				{
					if (parseJson(sResp, "msg", 0, 0, "", sOutput) == FALSE)
					{
						sOutput = "Login fail,query error msg fail";
					}
					printLog(sOutput, false);
					return;
				}
			}

			delete[] data;
			data = NULL;
		}
	}
}
*/
BOOL CExport::getCmd(CString func, CString mode)
{
	if (mode == "")
	{
		printLog("Please select mode", false);
		return FALSE;
	}
	if (func == "")
	{
		printLog("Please select func", false);
		return FALSE;
	}
	/*cmd[0] = { 0x58};
	cmd[1] = { 0x43};
	cmd[2] = { 0x05};*/
	g_info.cmd = "58 43 05 ";
	if (mode.Find("01_") > -1 && func.Find("01_") > -1)
	{
		/*cmd[3] = { 0x01 };
		cmd[4] = { 0x01 };*/
		g_info.cmd += "01 01";
	}
	else if (mode.Find("02_") > -1 && func.Find("01_") > -1)
	{
		/*cmd[3] = { 0x02 };
		cmd[4] = { 0x01 };*/
		g_info.cmd += "02 01";
	}
	else if (mode.Find("02_") > -1 && func.Find("02_") > -1)
	{
		/*cmd[3] = { 0x02 };
		cmd[4] = { 0x02 };*/
		g_info.cmd += "02 02";
	}
	else if (mode.Find("03_") > -1 && func.Find("01_") > -1)
	{
		/*cmd[3] = { 0x03 };
		cmd[4] = { 0x01 };*/
		g_info.cmd += "03 01";
	}
	else if (mode.Find("03_") > -1 && func.Find("02_") > -1)
	{
		/*cmd[3] = { 0x03 };
		cmd[4] = { 0x02 };*/
		g_info.cmd += "03 02";
	}
	else if (mode.Find("04_") > -1 && func.Find("01_") > -1)
	{
		/*cmd[3] = { 0x04 };
		cmd[4] = { 0x01 };*/
		g_info.cmd += "04 01";
	}
	else if (mode.Find("05_") > -1 && func.Find("01_") > -1)
	{
		/*cmd[3] = { 0x05 };
		cmd[4] = { 0x01 };*/
		g_info.cmd += "05 01";
	}
	else
	{
		printLog("Unknown cmd", false);
		return FALSE;
	}
	return TRUE;
}


BOOL CExport::needVerify()
{
	CString func, mode;
	((CComboBox*)GetDlgItem(IDC_COMBO_MODE))->GetWindowTextA(mode);
	((CComboBox*)GetDlgItem(IDC_COMBO_FUNCTION))->GetWindowTextA(func);
	if (mode == "")
	{
		printLog("Please select mode", false);
		return FALSE;
	}
	if (func == "")
	{
		printLog("Please select func", false);
		return FALSE;
	}
	
	if (mode.Find("02_") > -1 && func.Find("01_") > -1)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


void CExport::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CString sOutput;
	CloseComport(sOutput);
}


void CExport::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	__super::OnSysCommand(nID, lParam);
}



void CExport::printLog(CString msg,bool res)
{
	CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
	if (pFrame == NULL)
		return;

	pFrame->m_Export->Msg(msg, res);	
}


void CExport::workPortListAdd(CString str)
{
	for (int i = 0; i < workPortList.GetCount(); i++)
	{
		if (workPortList[i] == PORT_DEFAULT_DESC)
		{
			workPortList.SetAt(i, str);
			break;
		}

	}
}

void CExport::devicePortListAdd(CString str)
{
	if (str.Find(PORT_MTK_DESC) > -1)
		devicePortList.Add(str);
}


void CExport::initWorkPortList(CString str)
{
	for (int i = 0; i < PORT_LIST_LEN; i++)
		workPortList.Add(str);
}

BOOL CExport::isExistInDeviceArray(CString c)
{
	for (int i = 0; i < devicePortList.GetSize(); i++)
	{
		if (c == devicePortList[i])
			return TRUE;
	}

	return FALSE;
}

BOOL CExport::isExistInWorkArray(CString c)
{
	for (int i = 0; i < workPortList.GetSize(); i++)
	{
		if (c == workPortList[i])
			return TRUE;
	}

	return FALSE;
}
void CExport::freshDevicePortList()
{
	//workPortList不在devicePortList的，要移除。
	for (int i = 0; i < workPortList.GetSize(); i++)
	{
		if (workPortList[i] != PORT_DEFAULT_DESC && isExistInDeviceArray(workPortList[i]) == FALSE)
			workPortList.SetAt(i, PORT_DEFAULT_DESC);
	}

	//devicePortList不在workPortList的要增加。
	for (int i = 0; i < devicePortList.GetSize(); i++)
	{
		if (isExistInWorkArray(devicePortList[i]) == FALSE)
			workPortListAdd(devicePortList[i]);
	}
}

void CExport::initWorkPortShow()
{
	for (int i = 0; i < PORT_LIST_LEN; i++)
	{
		switch (i)
		{
		case 0:
			GetDlgItem(IDC_CHECK_COM1)->SetWindowTextA(workPortList[i]);
			break;
		case 1:
			GetDlgItem(IDC_CHECK_COM2)->SetWindowTextA(workPortList[i]);
			break;
		case 2:
			GetDlgItem(IDC_CHECK_COM3)->SetWindowTextA(workPortList[i]);
			break;
		case 3:
			GetDlgItem(IDC_CHECK_COM4)->SetWindowTextA(workPortList[i]);
			break;
		case 4:
			GetDlgItem(IDC_CHECK_COM5)->SetWindowTextA(workPortList[i]);
			break;
		case 5:
			GetDlgItem(IDC_CHECK_COM6)->SetWindowTextA(workPortList[i]);
			break;
		case 6:
			GetDlgItem(IDC_CHECK_COM7)->SetWindowTextA(workPortList[i]);
			break;
		case 7:
			GetDlgItem(IDC_CHECK_COM8)->SetWindowTextA(workPortList[i]);
			break;
		}
	}
}

void CExport::OnBnClickedCheckAll()
{
	// TODO:  在此添加控件通知处理程序代码
	bool isEnable = ((CButton *)GetDlgItem(IDC_CHECK_ALL))->GetCheck();
	if (isEnable)
	{
		((CButton *)GetDlgItem(IDC_CHECK_COM1))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_CHECK_COM2))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_CHECK_COM3))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_CHECK_COM4))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_CHECK_COM5))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_CHECK_COM6))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_CHECK_COM7))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_CHECK_COM8))->SetCheck(1);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_CHECK_COM1))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_CHECK_COM2))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_CHECK_COM3))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_CHECK_COM4))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_CHECK_COM5))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_CHECK_COM6))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_CHECK_COM7))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_CHECK_COM8))->SetCheck(0);
	}
	
}
