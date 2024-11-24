//
//  ChildView.h
//  TestXamlMfc
//
//  created by yu2924 on 2024-11-04
//

#pragma once

class CChildView : public CWnd
{
public:
	// XAML Island
	CString xamlFilePath;
	winrt::Windows::UI::Xaml::Hosting::DesktopWindowXamlSource desktopWindowXamlSource = nullptr;
	HWND hWndXamlIsland = NULL;
	void LoadXamlContent(LPCWSTR xamlpath);
	winrt::Windows::Foundation::IAsyncAction LoadXamlContentAsync(LPCWSTR xamlpath);
	void SetXamlContent(winrt::Windows::UI::Xaml::Controls::Panel const& xamlcontainer);

public:
	CChildView();
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	virtual ~CChildView();
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
};
