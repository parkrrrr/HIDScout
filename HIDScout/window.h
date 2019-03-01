#pragma once

class MainWindow
{
public:
    MainWindow(HINSTANCE hInstance);

    void Init();
private:
    void Register();

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

    static const unsigned int MAX_LOADSTRING = 100;
    WCHAR m_TitleText[MAX_LOADSTRING];                  // The title bar text
    WCHAR m_WindowClassName[MAX_LOADSTRING];            // the main window class name

    HWND m_hWnd = nullptr;
    HINSTANCE m_hInstance;

    HWND m_xamlHwnd = nullptr;

    winrt::Windows::UI::Xaml::Hosting::DesktopWindowXamlSource m_xamlSource;
};