// MainFrm.h :
//
#include "ChildFrm.h"
#include "RTK_Pack.h"

#include "UserManage.h"
#include "DeviceCmd.h"
#include "Record.h"
#include "locale.h"
#include "rapidjson/document.h"  
using namespace rapidjson;//命名空间
typedef GenericDocument<UTF8<> > WDocument; //定义16位Unicode的Document
typedef GenericValue<UTF8<> > WValue; // 定义16位Unicode的Value

//#include "CHttp.h"

#pragma once

//2017 04 01 zhaolong 
extern COleVariant
covTrue,
covFalse,
covOptional;


BOOL Print_PST(CString txt);
BOOL Print_ZEBRA(CString txt,int nums);

void Log(CString Str);
void Logger(CString head,CString Str);
bool WriteToFile(CString path, CString sn, CString wt);
TCHAR CalcMEIDCheckDigit(CString meid);

BOOL PostSubmit(CString strUrl, CString strPara, CString&strContent);


//01_ login
BOOL api_login(CString username, CString passw, CString & sOutput);
//02_ update password
BOOL api_edit_password(CString username, CString passw, CString newNassw, CString & sOutput);
//03_ insert cmd and result
BOOL api_insert_cmd_record(CString cmd, CString result, CString & sOutput);
//04_ verify dycode
BOOL api_verify_code(CString code, CString & sOutput);
//05_ check last login info to verify logined in other place
BOOL api_check_login(CString & sOutput);

BOOL parseJson(CString content, CString sKey, bool isArray, int size, CString sKey2, CString & sOutput);

bool InitConfig(CString &sOutput);

DWORD GetHash(BYTE *pbData, DWORD dwDataLen, ALG_ID algId, LPTSTR pszHash);
CString Md5(CString str);
int calcDays(CString dt);
CString getDT();
int GetLocalHostIPName(CString &sLocalName, CString &sIpAddress);




class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();


public:


public:
	//CChildFrame *m_Carton;
	CChildFrame *m_Export;
	//CChildFrame *m_Record;
	

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);


public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  
	CToolBar    m_wndToolBar;

	


protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	
public:

	void SetStatusBar(CString user, CString unlock, CString switchMode, CString adb, CString puk, CString custres);

	
	CString MEID_Hex2Dec(CString m_MEID_Hex);
	CString MEID_Dec2Hex(CString m_MEID_Dec);
	CString GetFileVersion();
	
	CString g_host;
	CString g_port;
	CString g_protocol;

	//MYSQL *conn;
	CStatusBar  m_wndStatusBar;
	HINSTANCE hDLL;
	
	BOOL InitDll();
	HINSTANCE m_pstHandle;

	//afx_msg void OnUserManage();
	afx_msg void OnFunOper();
	//afx_msg void OnRecordReview();

	afx_msg BOOL OnDeviceChange(UINT nEventType, DWORD dwData);
	
	void DoEvents()
	{
		MSG	msg;
		BOOL results;

		while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			results = ::GetMessage(&msg, NULL, 0, 0);
			if (results == 0)
			{
				::PostQuitMessage(msg.wParam);
				break;
			}
			else if (results == -1)
			{

			}
			else
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}
	}
};


