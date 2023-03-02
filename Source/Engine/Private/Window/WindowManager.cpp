#include "Atom/Logging.h"

#if defined(ATOM_PLATFORM_LINUX)
    #include "Window/LinuxWindow.h"
    using PlatformSelectedWindow = Atom::Engine::LinuxWindow;

#elif defined(ATOM_PLATFORM_WINDOWS)
    #include "Window/WindowsWindow.h"
    using PlatformSelectedWindow = Atom::Engine::WindowsWindow;

#else
    #error "Atom::Engine::Window is only supported for Linux and Windows platform for now."

#endif

using namespace Atom::Logging;

namespace Atom::Engine
{
    Window* WindowManger::CreateWindow(WindowProps props)
    {
        if (s_windowCount == 0)
        {
            int success = glfwInit();
            ASSERT(success, "GLFW initialization failed.");

            glfwSetErrorCallback(
                [](int error_code, const char* description)
                {
                    // TODO: Fix this compilation error.
                    // LOG_FATAL("GLFW Error: ", description);
                });
        }

        s_windowCount++;
        return new PlatformSelectedWindow(props);
    }

    void WindowManger::CloseWindow(Window* window)
    {
        ASSERT<NullPointerException>(window != nullptr, "Cannot close NULL window.");
        delete window;
    }

    SizeT WindowManger::s_windowCount = 0;
}