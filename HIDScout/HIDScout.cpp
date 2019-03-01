#include "pch.h"
#include "HIDScout.h"
#include "window.h"

HINSTANCE Application::s_hInstance = 0;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE,
    _In_ LPWSTR,
    _In_ int)
{
    return Application(hInstance).Run();
}

Application::Application(HINSTANCE hInstance)
{
    s_hInstance = hInstance;
}

int Application::Run()
{
    MSG msg;

    MainWindow window(s_hInstance);
    window.Init();

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return static_cast<int>(msg.wParam);
}

