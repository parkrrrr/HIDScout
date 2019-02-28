#include "pch.h"
#include "HIDScout.h"
#include "window.h"

std::shared_ptr<MainWindow> Application::s_mainWindow;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE,
	_In_ LPWSTR,
	_In_ int)
{
	return Application(hInstance).Run();
}

Application::Application(HINSTANCE hInstance) :
	m_hInstance(hInstance)
{
	Init();
}

void Application::Init()
{
	s_mainWindow = std::make_shared<MainWindow>(m_hInstance);
	s_mainWindow->Init();
}

int Application::Run()
{
	Init();

    HACCEL hAccelTable = LoadAccelerators(m_hInstance, MAKEINTRESOURCE(IDC_HIDSCOUT));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
	return static_cast<int>(msg.wParam);
}

