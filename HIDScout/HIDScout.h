#pragma once

#include "resource.h"

class MainWindow;

class Application
{
public:
	Application(HINSTANCE hInstance);
	static std::shared_ptr<MainWindow> GetMainWindow()
	{
		return s_mainWindow;
	}

	void Init();
	int Run();
private:
	HINSTANCE m_hInstance;
	static std::shared_ptr<MainWindow> s_mainWindow;
};