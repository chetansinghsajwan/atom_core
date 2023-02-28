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

        glfwSetWindowPosCallback(_glfwWindow,
            [](GLFWwindow* window, int xpos, int ypos)
            {
                GLFWWindow* _this = reinterpret_cast<GLFWWindow*>(
                    glfwGetWindowUserPointer(window));

                Vector2 oldPos = _this->_position;
                Vector2 newPos = { (SizeT)xpos, (SizeT)ypos };
                _this->_position = newPos;

                _this->_eventSource.Dispatch(SWindowRepositionEvent(
                    newPos, newPos - oldPos));
            });

        glfwSetWindowSizeCallback(_glfwWindow,
            [](GLFWwindow* window, int width, int height)
            {
                GLFWWindow* _this = reinterpret_cast<GLFWWindow*>(
                    glfwGetWindowUserPointer(window));

                Vector2 newSize = { (SizeT)width, (SizeT)height };
                Vector2 oldSize = _this->_size;
                _this->_size = newSize;

                _this->_eventSource.Dispatch(SWindowResizeEvent(
                    newSize, newSize - oldSize));
            });

        glfwSetWindowCloseCallback(_glfwWindow,
            [](GLFWwindow* window)
            {
                GLFWWindow* _this = reinterpret_cast<GLFWWindow*>(
                    glfwGetWindowUserPointer(window));

                _this->_eventSource.Dispatch(SWindowCloseEvent());
            });

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