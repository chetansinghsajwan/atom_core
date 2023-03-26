#pragma once
#include "Window/GLFWWindow.h"

namespace Atom::Engine
{
    class WindowsWindow: public GLFWWindow
    {
    public:
        WindowsWindow(const SWindowProps& props):
            GLFWWindow(props) { }
    };
}