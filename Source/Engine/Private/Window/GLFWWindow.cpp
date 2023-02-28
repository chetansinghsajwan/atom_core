#include "Atom/Logging.h"
#include "Window/GLFWWindow.h"

using namespace Atom::Logging;

namespace Atom::Engine
{
    GLFWWindow::GLFWWindow(const WindowProps& props)
    {
        glfwSetErrorCallback(
            [](int error_code, const char* description)
            {
                // TODO: Fix this compilation error.
                // LOG_FATAL("GLFW Error: ", description);
            });

        _dimensions = props.dimensions;

        _glfwWindow = glfwCreateWindow(_dimensions.x, _dimensions.y,
            props.windowName.data(), nullptr, nullptr);

        glfwMakeContextCurrent(_glfwWindow);
        glfwSetWindowUserPointer(_glfwWindow, this);

        EnableVSync();
    }

    GLFWWindow::~GLFWWindow()
    {
        glfwDestroyWindow(_glfwWindow);
    }

    void GLFWWindow::OnUpdate()
    {
        glfwPollEvents();
        glfwSwapBuffers(_glfwWindow);
    }

    void GLFWWindow::EnableVSync()
    {
        glfwSwapInterval(1);
        _vSyncEnabled = true;
    }

    void GLFWWindow::DisableVSync()
    {
        glfwSwapInterval(0);
        _vSyncEnabled = false;
    }

    bool GLFWWindow::IsVSyncEnabled() const noexcept
    {
        return _vSyncEnabled;
    }
}