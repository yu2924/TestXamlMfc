//
//  TestXamlMfc.cpp
//  TestXamlMfc
//
//  created by yu2924 on 2024-11-04
//


#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "TestXamlMfc.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ================================================================================

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// ================================================================================

BEGIN_MESSAGE_MAP(CTestXamlMfcApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CTestXamlMfcApp::OnAppAbout)
END_MESSAGE_MAP()

CTestXamlMfcApp::CTestXamlMfcApp() noexcept
{
	// TODO: 下のアプリケーション ID 文字列を一意の ID 文字列で置換します。推奨される
	// 文字列の形式は CompanyName.ProductName.SubProduct.VersionInformation です
	SetAppID(_T("TestXamlMfc.AppID.NoVersion"));
}

CTestXamlMfcApp theApp;

BOOL CTestXamlMfcApp::InitInstance()
{
	// XAML Island
	winrt::init_apartment(winrt::apartment_type::single_threaded);
	windowsXamlManager = winrt::Windows::UI::Xaml::Hosting::WindowsXamlManager::InitializeForCurrentThread();
	WCHAR xamlpath[MAX_PATH]{};
	GetModuleFileNameW(NULL, xamlpath, _countof(xamlpath));
	PathRemoveFileSpecW(xamlpath);
	PathAppendW(xamlpath, L"Content.xaml");
	xamlContentFilePath = xamlpath;
	if(!PathFileExistsW(xamlContentFilePath))
	{
		HRSRC hx = FindResourceW(NULL, L"Content.xaml", L"XAML");
		DWORD cbx = SizeofResource(NULL, hx);
		const void* px = LockResource(LoadResource(NULL, hx));
		CFile file(xamlContentFilePath, CFile::typeBinary | CFile::modeWrite | CFile::modeCreate);
		file.Write(px, cbx);
	}
	// standard init
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	CWinApp::InitInstance();
	EnableTaskbarInteraction(FALSE);
	SetRegistryKey(_T("yu2924"));
	CFrameWnd* pFrame = new CMainFrame;
	if(!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, nullptr,
		nullptr);
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	return TRUE;
}

int CTestXamlMfcApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}

void CTestXamlMfcApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}



