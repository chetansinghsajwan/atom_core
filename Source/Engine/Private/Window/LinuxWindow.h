#pragma once
#include "Window/GLFWWindow.h"

namespace Atom::Engine
{
    class LinuxWindow: public GLFWWindow
    {
    public:
        ctor LinuxWindow(const SWindowProps& props):
            GLFWWindow(props) { }
    };
}