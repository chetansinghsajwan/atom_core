#include "Atom/Logging.h"
#include "Window/GLFWWindow.h"

using namespace Atom::Logging;

namespace Atom::Engine
{
    ctor GLFWWindow::GLFWWindow(const SWindowProps& props):
        Window(m_windowEventSource)
    {
        GLFW_SWindowCoords glfwWindowSize = GLFW_WindowCoordsConverter::ToGLFW(
            props.windowSize);

        // TODO: Requires encoding conversion.
        m_glfwWindow = glfwCreateWindow(glfwWindowSize.x, glfwWindowSize.y,
            (const char*)props.windowName.Data(), nullptr, nullptr);

        glfwMakeContextCurrent(m_glfwWindow);
        glfwSetWindowUserPointer(m_glfwWindow, this);

        glfwSetWindowPosCallback(m_glfwWindow,
            [](GLFWwindow* glfwWindow, int xpos, int ypos)
            {
                GLFWWindow& window = *reinterpret_cast<GLFWWindow*>(
                    glfwGetWindowUserPointer(glfwWindow));

                SWindowCoords oldPos = window.m_windowPos;
                SWindowCoords newPos = GLFW_WindowCoordsConverter::FromGLFW({ xpos, ypos });
                window.m_windowPos = newPos;

                window.m_windowEventSource.Dispatch(SWindowRepositionEvent(
                    newPos, newPos - oldPos));
            });

        glfwSetWindowSizeCallback(m_glfwWindow,
            [](GLFWwindow* glfwWindow, int width, int height)
            {
                GLFWWindow& window = *reinterpret_cast<GLFWWindow*>(
                    glfwGetWindowUserPointer(glfwWindow));

                SWindowCoords oldSize = window.m_windowSize;
                SWindowCoords newSize = GLFW_WindowCoordsConverter::FromGLFW({ width, height });
                window.m_windowSize = newSize;

                window.m_windowEventSource.Dispatch(SWindowResizeEvent(
                    newSize, newSize - oldSize));
            });

        glfwSetWindowCloseCallback(m_glfwWindow,
            [](GLFWwindow* glfwWindow)
            {
                GLFWWindow& window = *reinterpret_cast<GLFWWindow*>(
                    glfwGetWindowUserPointer(glfwWindow));

                window.m_windowEventSource.Dispatch(SWindowCloseEvent());
            });

        UpdatePos();
        UpdateSize();
        SetVSync(true);
    }

    GLFWWindow::dtor GLFWWindow()
    {
        glfwDestroyWindow(m_glfwWindow);
    }

    fn GLFWWindow::Update() -> void
    {
        glfwPollEvents();
        glfwSwapBuffers(m_glfwWindow);
    }

    fn GLFWWindow::SetPos(SWindowCoords pos) -> void
    {
        GLFW_SWindowCoords glfwPos = GLFW_WindowCoordsConverter::ToGLFW(pos);

        glfwSetWindowPos(m_glfwWindow, glfwPos.x, glfwPos.y);
        m_windowPos = GLFW_WindowCoordsConverter::FromGLFW(glfwPos);
    }

    fn GLFWWindow::GetPos() const noex -> SWindowCoords
    {
        return m_windowPos;
    }

    fn GLFWWindow::UpdatePos() -> SWindowCoords
    {
        GLFW_SWindowCoords glfwPos;
        glfwGetWindowPos(m_glfwWindow, &glfwPos.x, &glfwPos.y);

        return GLFW_WindowCoordsConverter::FromGLFW(glfwPos);
    }

    fn GLFWWindow::SetSize(SWindowCoords size) -> void
    {
        GLFW_SWindowCoords glfwSize = GLFW_WindowCoordsConverter::ToGLFW(size);

        glfwSetWindowSize(m_glfwWindow, glfwSize.x, glfwSize.y);
        m_windowSize = GLFW_WindowCoordsConverter::FromGLFW(glfwSize);
    }

    fn GLFWWindow::GetSize() const noex -> SWindowCoords
    {
        return m_windowSize;
    }

    fn GLFWWindow::UpdateSize() -> SWindowCoords
    {
        GLFW_SWindowCoords glfwSize;
        glfwGetWindowSize(m_glfwWindow, &glfwSize.x, &glfwSize.y);

        m_windowSize = GLFW_WindowCoordsConverter::FromGLFW(glfwSize);
        return m_windowSize;
    }

    fn GLFWWindow::SetVSync(bool enable) -> void
    {
        glfwSwapInterval(enable ? 1 : 0);
        m_windowVSync = enable;
    }

    fn GLFWWindow::GetVSync() const noex -> bool
    {
        return m_windowVSync;
    }

    fn GLFWWindow::GetNative() const noex -> void*
    {
        return m_glfwWindow;
    }
}