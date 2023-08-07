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
    fn WindowManger::CreateWindow(WindowProps props) -> Window*
    {
        if (s_windowCount == 0)
        {
            i32 success = glfwInit();
            ATOM_ASSERT(success) << RuntimeException(
                "GLFW initialization failed.");

            glfwSetErrorCallback(
                [](i32 error_code, const char* description)
                {
                    // TODO: Fix this compilation error.
                    // LOG_FATAL("GLFW Error: ", description);
                });
        }

        s_windowCount++;
        return new PlatformSelectedWindow(props);
    }

    fn WindowManger::CloseWindow(Window* window) -> void
    {
        ATOM_ASSERT(window != nullptr) << NullPointerException(
            "Cannot close NULL window.");

        delete window;
    }

    usize WindowManger::s_windowCount = 0;
}