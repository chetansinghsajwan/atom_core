#pragma once
#include "Window/GlfwWindow.h"

namespace Atom::Engine
{
    class WindowsWindow: public GlfwWindow
    {
    public:
        ctor WindowsWindow(const WindowProps& props):
            GlfwWindow(props) { }
    };
}