#include "Atom/Logging.h"
#include "Window/GLFWWindow.h"

using namespace Atom::Logging;

namespace Atom::Engine
{
    ctor GLFWWindow::GLFWWindow(const SWindowProps& props):
        Window(_windowEventSource)
    {
        GLFW_SWindowCoords glfwWindowSize = GLFW_WindowCoordsConverter::ToGLFW(
            props.windowSize);

        // TODO: Requires encoding conversion.
        _glfwWindow = glfwCreateWindow(glfwWindowSize.x, glfwWindowSize.y,
            (const char*)props.windowName.Data(), nullptr, nullptr);

        glfwMakeContextCurrent(_glfwWindow);
        glfwSetWindowUserPointer(_glfwWindow, this);

        glfwSetWindowPosCallback(_glfwWindow,
            [](GLFWwindow* glfwWindow, int xpos, int ypos)
            {
                GLFWWindow& window = *reinterpret_cast<GLFWWindow*>(
                    glfwGetWindowUserPointer(glfwWindow));

                SWindowCoords oldPos = window._windowPos;
                SWindowCoords newPos = GLFW_WindowCoordsConverter::FromGLFW({ xpos, ypos });
                window._windowPos = newPos;

                window._windowEventSource.Dispatch(SWindowRepositionEvent(
                    newPos, newPos - oldPos));
            });

        glfwSetWindowSizeCallback(_glfwWindow,
            [](GLFWwindow* glfwWindow, int width, int height)
            {
                GLFWWindow& window = *reinterpret_cast<GLFWWindow*>(
                    glfwGetWindowUserPointer(glfwWindow));

                SWindowCoords oldSize = window._windowSize;
                SWindowCoords newSize = GLFW_WindowCoordsConverter::FromGLFW({ width, height });
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

    GLFWWindow::dtor GLFWWindow()
    {
        glfwDestroyWindow(_glfwWindow);
    }

    fn GLFWWindow::Update() -> void
    {
        glfwPollEvents();
        glfwSwapBuffers(_glfwWindow);
    }

    fn GLFWWindow::SetPos(SWindowCoords pos) -> void
    {
        GLFW_SWindowCoords glfwPos = GLFW_WindowCoordsConverter::ToGLFW(pos);

        glfwSetWindowPos(_glfwWindow, glfwPos.x, glfwPos.y);
        _windowPos = GLFW_WindowCoordsConverter::FromGLFW(glfwPos);
    }

    fn GLFWWindow::GetPos() const noex -> SWindowCoords
    {
        return _windowPos;
    }

    fn GLFWWindow::UpdatePos() -> SWindowCoords
    {
        GLFW_SWindowCoords glfwPos;
        glfwGetWindowPos(_glfwWindow, &glfwPos.x, &glfwPos.y);

        return GLFW_WindowCoordsConverter::FromGLFW(glfwPos);
    }

    fn GLFWWindow::SetSize(SWindowCoords size) -> void
    {
        GLFW_SWindowCoords glfwSize = GLFW_WindowCoordsConverter::ToGLFW(size);

        glfwSetWindowSize(_glfwWindow, glfwSize.x, glfwSize.y);
        _windowSize = GLFW_WindowCoordsConverter::FromGLFW(glfwSize);
    }

    fn GLFWWindow::GetSize() const noex -> SWindowCoords
    {
        return _windowSize;
    }

    fn GLFWWindow::UpdateSize() -> SWindowCoords
    {
        GLFW_SWindowCoords glfwSize;
        glfwGetWindowSize(_glfwWindow, &glfwSize.x, &glfwSize.y);

        _windowSize = GLFW_WindowCoordsConverter::FromGLFW(glfwSize);
        return _windowSize;
    }

    fn GLFWWindow::SetVSync(bool enable) -> void
    {
        glfwSwapInterval(enable ? 1 : 0);
        _windowVSync = enable;
    }

    fn GLFWWindow::GetVSync() const noex -> bool
    {
        return _windowVSync;
    }

    fn GLFWWindow::GetNative() const noex -> void*
    {
        return _glfwWindow;
    }
}