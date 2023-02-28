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
    UniquePtr<Window> WindowManger::CreateWindow(WindowProps props)
    {
        if (s_windowCount == 0)
        {
            int success = glfwInit();
            ASSERT(success, "GLFW initialization failed.");
        }

        return MakeUnique<PlatformSelectedWindow>(props);
    }

    void WindowManger::CloseWindow(UniquePtr<Window> window)
    {
        ASSERT<NullPointerException>(window != nullptr, "Cannot close NULL window.");
    }

    SizeT WindowManger::s_windowCount = 0;
}