#include "Atom/Logging.h"
#include "Window/GLFWWindow.h"

using namespace Atom::Logging;

namespace Atom::Engine
{
    GLFWWindow::GLFWWindow(const WindowProps& props):
        Window(_windowEventSource)
    {
        _glfwWindow = glfwCreateWindow(props.windowSize.x, props.windowSize.y,
            props.windowName.data(), nullptr, nullptr);

        glfwMakeContextCurrent(_glfwWindow);
        glfwSetWindowUserPointer(_glfwWindow, this);

        glfwSetWindowPosCallback(_glfwWindow,
            [](GLFWwindow* glfwWindow, int xpos, int ypos)
            {
                GLFWWindow& window = *reinterpret_cast<GLFWWindow*>(
                    glfwGetWindowUserPointer(glfwWindow));

                SVector2 oldPos = window._windowPos;
                SVector2 newPos = { (float)xpos, (float)ypos };
                window._windowPos = newPos;

                window._windowEventSource.Dispatch(SWindowRepositionEvent(
                    newPos, newPos - oldPos));
            });

        glfwSetWindowSizeCallback(_glfwWindow,
            [](GLFWwindow* glfwWindow, int width, int height)
            {
                GLFWWindow& window = *reinterpret_cast<GLFWWindow*>(
                    glfwGetWindowUserPointer(glfwWindow));

                SVector2 newSize = { (float)width, (float)height };
                SVector2 oldSize = window._windowSize;
                window._windowSize = newSize;

                window._windowEventSource.Dispatch(SWindowResizeEvent(
                    newSize, newSize - oldSize));
            });

        glfwSetWindowCloseCallback(_glfwWindow,
            [](GLFWwindow* glfwWindow)
            {
                GLFWWindow& window = *reinterpret_cast<GLFWWindow*>(
                    glfwGetWindowUserPointer(glfwWindow));

                window._windowEventSource.Dispatch(SWindowCloseEvent());
            });

        UpdatePos();
        UpdateSize();
        SetVSync(true);
    }

    GLFWWindow::~GLFWWindow()
    {
        glfwDestroyWindow(_glfwWindow);
    }

    void GLFWWindow::Update()
    {
        glfwPollEvents();
        glfwSwapBuffers(_glfwWindow);
    }

    void GLFWWindow::SetPos(SVector2 pos)
    {
        glfwSetWindowPos(_glfwWindow, pos.x, pos.y);
        _windowPos = pos;
    }

    SVector2 GLFWWindow::GetPos() const noexcept
    {
        return _windowPos;
    }

    SVector2 GLFWWindow::UpdatePos()
    {
        int xpos, ypos;
        glfwGetWindowPos(_glfwWindow, &xpos, &ypos);
        _windowPos = { (float)xpos, (float)ypos };
        return _windowPos;
    }

    void GLFWWindow::SetSize(SVector2 size)
    {
        glfwSetWindowSize(_glfwWindow, size.x, size.y);
        _windowSize = size;
    }

    SVector2 GLFWWindow::GetSize() const noexcept
    {
        return _windowSize;
    }

    SVector2 GLFWWindow::UpdateSize()
    {
        int xpos, ypos;
        glfwGetWindowSize(_glfwWindow, &xpos, &ypos);
        _windowSize = { (float)xpos, (float)ypos };
        return _windowSize;
    }

    void GLFWWindow::SetVSync(bool enable)
    {
        glfwSwapInterval(enable ? 1 : 0);
        _windowVSync = enable;
    }

    bool GLFWWindow::GetVSync() const noexcept
    {
        return _windowVSync;
    }

    void* GLFWWindow::GetNative() const noexcept
    {
        return _glfwWindow;
    }
}