//
//  TestXamlMfc.h
//  TestXamlMfc
//
//  created by yu2924 on 2024-11-04
//

#pragma once

#ifndef __AFXWIN_H__
#error "PCH に対してこのファイルをインクルードする前に 'pch.h' をインクルードしてください"
#endif

#include "resource.h"

class CTestXamlMfcApp : public CWinApp
{
public:
	// XAML Island
	winrt::Windows::UI::Xaml::Hosting::WindowsXamlManager windowsXamlManager = nullptr;
	CString xamlContentFilePath;

public:
	CTestXamlMfcApp() noexcept;
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CTestXamlMfcApp theApp;
