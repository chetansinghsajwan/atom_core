#pragma once
#include "Window/GlfwWindow.h"

namespace Atom::Engine
{
    class LinuxWindow extends GlfwWindow
    {
        pub ctor LinuxWindow(const WindowProps& props):
            GlfwWindow(props) { }
    };
}