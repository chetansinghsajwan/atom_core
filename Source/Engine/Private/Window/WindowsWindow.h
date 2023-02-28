#pragma once
#include "Window/GLFWWindow.h"

namespace Atom::Engine
{
    class WindowsWindow: public GLFWWindow
    {
    public:
        WindowsWindow(const WindowProps& props):
            GLFWWindow(props) { }
    };
}