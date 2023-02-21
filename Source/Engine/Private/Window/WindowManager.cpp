#include "GLFW/glfw3.h"

#include "Atom/Logging.h"
#include "Atom/Engine/Window/WindowManager.h"

using namespace Atom::Logging;

namespace Atom::Engine
{
#if defined(ATOM_PLATFORM_LINUX)

    class ATOM_API LinuxWindow: public Window
    {
    public:
        LinuxWindow(const WindowProps& props)
        {
            _dimensions = props.dimensions;

            _window = glfwCreateWindow(_dimensions.x, _dimensions.y,
                props.windowName.data(), nullptr, nullptr);

            glfwMakeContextCurrent(_window);
            glfwSetWindowUserPointer(_window, &_data);

            EnableVSync();
        }

        ~LinuxWindow()
        {
            glfwDestroyWindow(_window);
        }

    public:
        void OnUpdate() override
        {
            glfwPollEvents();
            glfwSwapBuffers(_window);
        }

        void EnableVSync()
        {
            glfwSwapInterval(1);
            _vSyncEnabled = true;
        }

        void DisableVSync()
        {
            glfwSwapInterval(0);
            _vSyncEnabled = false;
        }

        bool IsVSyncEnabled() const noexcept
        {
            return _vSyncEnabled;
        }

    protected:
        GLFWwindow* _window;
        bool _vSyncEnabled;

        struct Data
        {
            int value;
        };

        Data _data;
    };

#else
#error "Atom::Engine::Window is only supported for Linux platform for now."
#endif

    UniquePtr<Window> WindowManger::CreateWindow(WindowProps props)
    {
        if (s_windowCount == 0)
        {
            int success = glfwInit();
            ASSERT(success, "GLFW initialization failed.");
        }

        return MakeUnique<LinuxWindow>(props);
    }

    void WindowManger::CloseWindow(UniquePtr<Window> window)
    {
        ASSERT<NullPointerException>(window != nullptr, "Cannot close NULL window.");
    }

    SizeT WindowManger::s_windowCount = 0;
}