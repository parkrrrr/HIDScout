#pragma once

#include "resource.h"

class MainWindow;

class Application
{
public:
    Application(HINSTANCE hInstance);
    static HINSTANCE GetInstance()
    {
        return s_hInstance;
    }

    int Run();
private:
    static HINSTANCE s_hInstance;
};