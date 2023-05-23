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
    Window* WindowManger::CreateWindow(SWindowProps props)
    {
        if (s_windowCount == 0)
        {
            int success = glfwInit();
            ATOM_ASSERT(success) << RuntimeException(
                TEXT("GLFW initialization failed."));

            glfwSetErrorCallback(
                [](int error_code, const char* description)
                {
                    // TODO: Fix this compilation error.
                    // LOG_FATAL(TEXT("GLFW Error: "), description);
                });
        }

        s_windowCount++;
        return new PlatformSelectedWindow(props);
    }

    void WindowManger::CloseWindow(Window* window)
    {
        ATOM_ASSERT(window != nullptr) << NullPointerException(
            TEXT("Cannot close NULL window."));

        delete window;
    }

    usize WindowManger::s_windowCount = 0;
}