#include "Atom/Engine/Window/WindowManager.h"

namespace Atom::Engine
{
#if defined(ATOM_PLATFORM_LINUX)

    class ATOM_API LinuxWindow: public Window
    {
    public:
        LinuxWindow(const WindowProps& props)
        {
            _dimensions = props.dimensions;
        }
    };

#else
#error "Atom::Engine::Window is only supported for Linux platform for now."
#endif

    UniquePtr<Window> WindowManger::CreateWindow(WindowProps props)
    {
        return MakeUnique<LinuxWindow>(props);
    }

    void WindowManger::CloseWindow(UniquePtr<Window> window)
    {
        ASSERT<NullPointerException>(window != nullptr, "Cannot close NULL window.");
    }
}