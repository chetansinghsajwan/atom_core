#pragma once
#include "Window/GlfwWindow.h"

namespace Atom::Engine
{
    class LinuxWindow extends GlfwWindow
    {
    public:
        ctor LinuxWindow(const WindowProps& props):
            GlfwWindow(props) { }
    };
}