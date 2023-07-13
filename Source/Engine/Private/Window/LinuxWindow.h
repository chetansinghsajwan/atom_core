#pragma once
#include "Window/GLFWWindow.h"

namespace Atom::Engine
{
    class LinuxWindow extends GLFWWindow
    {
        pub ctor LinuxWindow(const WindowProps& props):
            GLFWWindow(props) { }
    };
}