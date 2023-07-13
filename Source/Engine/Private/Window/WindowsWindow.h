#pragma once
#include "Window/GLFWWindow.h"

namespace Atom::Engine
{
    class WindowsWindow: public GLFWWindow
    {
    public:
        ctor WindowsWindow(const WindowProps& props):
            GLFWWindow(props) { }
    };
}