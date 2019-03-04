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

    m_hWnd = ::CreateWindowW(m_WindowClassName, m_TitleText, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, m_hInstance, nullptr);

    if (!m_hWnd)
    {
        return;
    }


    winrt::init_apartment();

    auto manager = winrt::Windows::UI::Xaml::Hosting::WindowsXamlManager::InitializeForCurrentThread();
    auto nativeXamlSource = m_xamlSource.as<IDesktopWindowXamlSourceNative>();

    nativeXamlSource->AttachToWindow(m_hWnd);
    nativeXamlSource->get_WindowHandle(&m_xamlHwnd);

    TreeView treeview;
    m_xamlSource.Content(treeview);

    ::ShowWindow(m_hWnd, SW_SHOWDEFAULT);
    ::UpdateWindow(m_hWnd);

    RECT clientRect = {};
    ::GetClientRect(m_hWnd, &clientRect);
    ::SetWindowPos(m_xamlHwnd, 0, 0, 0, clientRect.right, clientRect.bottom, SWP_SHOWWINDOW);
}

void MainWindow::Register()
{

    WNDCLASS wc{};
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hInstance = m_hInstance;
    wc.lpszClassName = m_WindowClassName;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    RegisterClass(&wc);
}

LRESULT CALLBACK MainWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {

    case WM_PAINT:
    {
        PAINTSTRUCT paint{};
        BeginPaint(hWnd, &paint);
        EndPaint(hWnd, &paint);
        return 0;
    }


    case WM_DESTROY:
        ::PostQuitMessage(0);
        break;
    default:
        return ::DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

