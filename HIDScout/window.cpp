#include "pch.h"
#include "Resource.h"
#include "window.h"
#include "HIDScout.h"
#include "TreeView.h"

MainWindow::MainWindow(HINSTANCE hInstance) :
	m_hInstance(hInstance)
{
}

void MainWindow::Init()
{
	::LoadString(m_hInstance, IDS_APP_TITLE, m_TitleText, MAX_LOADSTRING);
	::LoadString(m_hInstance, IDC_HIDSCOUT, m_WindowClassName, MAX_LOADSTRING);

	Register();

	m_hWnd = ::CreateWindow(m_WindowClassName, m_TitleText, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, m_hInstance, nullptr);

	if (!m_hWnd)
	{
		return;
	}

	winrt::init_apartment(winrt::apartment_type::single_threaded);

	winrt::Windows::UI::Xaml::Hosting::WindowsXamlManager::InitializeForCurrentThread();
	winrt::Windows::UI::Xaml::Hosting::DesktopWindowXamlSource xamlSource;
	auto nativeXamlSource = xamlSource.as<IDesktopWindowXamlSourceNative>();

	nativeXamlSource->AttachToWindow(m_hWnd);
	nativeXamlSource->get_WindowHandle(&m_xamlHwnd);

	RECT clientRect = {};
	::GetClientRect(m_hWnd, &clientRect);
	::SetWindowPos(m_xamlHwnd, nullptr, 0, 0, clientRect.right, clientRect.bottom, SWP_SHOWWINDOW);

	TreeView treeview;
	xamlSource.Content(treeview);

	::ShowWindow(m_hWnd, SW_SHOW);
	::UpdateWindow(m_hWnd);
}

void MainWindow::Register()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInstance;
	wcex.hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_HIDSCOUT));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_HIDSCOUT);
	wcex.lpszClassName = m_WindowClassName;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	::RegisterClassEx(&wcex);
}

LRESULT CALLBACK MainWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return Application::GetMainWindow()->InternalWndProc(hWnd, message, wParam, lParam);
}

LRESULT MainWindow::InternalWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			::DialogBox(m_hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), m_hWnd, About);
			break;
		case IDM_EXIT:
			::DestroyWindow(hWnd);
			break;
		default:
			return ::DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;

	case WM_DESTROY:
		::PostQuitMessage(0);
		break;
	default:
		return ::DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

INT_PTR CALLBACK MainWindow::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return static_cast<INT_PTR>(TRUE);

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			::EndDialog(hDlg, LOWORD(wParam));
			return static_cast<INT_PTR>(TRUE);
		}
		break;
	}
	return static_cast<INT_PTR>(FALSE);
}
