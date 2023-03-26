#pragma once
#include "Window/GLFWWindow.h"

namespace Atom::Engine
{
    class LinuxWindow: public GLFWWindow
    {
    public:
        LinuxWindow(const SWindowProps& props):
            GLFWWindow(props) { }
    };
}