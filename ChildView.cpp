//
//  ChildView.cpp
//  TestXamlMfc
//
//  created by yu2924 on 2024-11-04
//

#include "pch.h"
#include "framework.h"
#include "TestXamlMfc.h"
#include "ChildView.h"

#include <msxml6.h>
#pragma comment(lib, "msxml6.lib")
#include <winrt/windows.ui.xaml.Markup.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}

BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs)
{
	if(!CWnd::PreCreateWindow(cs)) return FALSE;
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, ::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), nullptr);
	return TRUE;
}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CWnd::OnCreate(lpCreateStruct) == -1) return -1;
	// XAML hosting
	desktopWindowXamlSource = winrt::Windows::UI::Xaml::Hosting::DesktopWindowXamlSource();
	auto interop = desktopWindowXamlSource.as<IDesktopWindowXamlSourceNative>();
	if(!interop) { ATLTRACE(L"[XAML Island] ERROR: IDesktopWindowXamlSourceNative is null\n"); }
	HRESULT r = interop->AttachToWindow(m_hWnd);
	if(FAILED(r)) { ATLTRACE(L"[XAML Island] ERROR: IDesktopWindowXamlSourceNative::AttachToWindow() failed\n"); }
	interop->get_WindowHandle(&hWndXamlIsland);
	::SetWindowPos(hWndXamlIsland, NULL, 0, 0, lpCreateStruct->cx, lpCreateStruct->cy, SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);
	// XAML content
	bool async = false;
	if(async) LoadXamlContentAsync(theApp.xamlContentFilePath);
	else LoadXamlContent(theApp.xamlContentFilePath);
	return 0;
}

void CChildView::LoadXamlContent(LPCWSTR xamlpath)
{
	winrt::Windows::UI::Xaml::Controls::Panel xamlcontainer = nullptr;
	CComPtr<IXMLDOMDocument> xdoc;
	if(SUCCEEDED(xdoc.CoCreateInstance(CLSID_DOMDocument60, nullptr, CLSCTX_INPROC_SERVER)))
	{
		VARIANT_BOOL rload = VARIANT_FALSE;
		if((xdoc->load(CComVariant(xamlpath), &rload) == S_OK) && rload)
		{
			CComBSTR xml; xdoc->get_xml(&xml);
			xamlcontainer = winrt::Windows::UI::Xaml::Markup::XamlReader::Load(winrt::hstring(xml)).as<winrt::Windows::UI::Xaml::Controls::Panel>();
		}
	}
	SetXamlContent(xamlcontainer);
}

winrt::Windows::Foundation::IAsyncAction CChildView::LoadXamlContentAsync(LPCWSTR xamlpath)
{
	auto xfile = co_await winrt::Windows::Storage::StorageFile::GetFileFromPathAsync(winrt::hstring(xamlpath));
	auto xdoc = co_await winrt::Windows::Data::Xml::Dom::XmlDocument::LoadFromFileAsync(xfile);
	winrt::hstring xml = xdoc.GetXml();
	winrt::Windows::UI::Xaml::Controls::Panel xamlcontainer = winrt::Windows::UI::Xaml::Markup::XamlReader::Load(xml).as<winrt::Windows::UI::Xaml::Controls::Panel>();
	SetXamlContent(xamlcontainer);
}

void CChildView::SetXamlContent(winrt::Windows::UI::Xaml::Controls::Panel const& xamlcontainer)
{
	winrt::Windows::UI::Xaml::Controls::Panel xamlContainer = xamlcontainer;
	if(!xamlContainer)
	{
		xamlContainer = winrt::Windows::UI::Xaml::Controls::StackPanel();
		xamlContainer.Background(winrt::Windows::UI::Xaml::Media::SolidColorBrush{ winrt::Windows::UI::Colors::LightGray() });
		winrt::Windows::UI::Xaml::Controls::TextBlock textblock;
		textblock.Text(L"XAML not found");
		textblock.VerticalAlignment(winrt::Windows::UI::Xaml::VerticalAlignment::Center);
		textblock.HorizontalAlignment(winrt::Windows::UI::Xaml::HorizontalAlignment::Center);
		textblock.FontSize(20);
		xamlContainer.Children().Append(textblock);
	}
	xamlContainer.UpdateLayout();
	desktopWindowXamlSource.Content(xamlContainer);
}

void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if(hWndXamlIsland)
	{
		::SetWindowPos(hWndXamlIsland, NULL, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOACTIVATE);
	}
}

void CChildView::OnPaint()
{
	CPaintDC dc(this);
	CRect rc; GetClientRect(&rc);
	dc.DrawText(CString(L"if you see this, something is wrong"), rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}
