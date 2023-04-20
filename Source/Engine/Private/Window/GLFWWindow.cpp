#include "Atom/Logging.h"
#include "Window/GLFWWindow.h"

using namespace Atom::Logging;

namespace Atom::Engine
{
    GLFWWindow::GLFWWindow(const SWindowProps& props):
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

    GLFWWindow::~GLFWWindow()
    {
        glfwDestroyWindow(m_glfwWindow);
    }

    void GLFWWindow::Update()
    {
        glfwPollEvents();
        glfwSwapBuffers(m_glfwWindow);
    }

    void GLFWWindow::SetPos(SWindowCoords pos)
    {
        GLFW_SWindowCoords glfwPos = GLFW_WindowCoordsConverter::ToGLFW(pos);

        glfwSetWindowPos(m_glfwWindow, glfwPos.x, glfwPos.y);
        m_windowPos = GLFW_WindowCoordsConverter::FromGLFW(glfwPos);
    }

    SWindowCoords GLFWWindow::GetPos() const noexcept
    {
        return m_windowPos;
    }

    SWindowCoords GLFWWindow::UpdatePos()
    {
        GLFW_SWindowCoords glfwPos;
        glfwGetWindowPos(m_glfwWindow, &glfwPos.x, &glfwPos.y);

        return GLFW_WindowCoordsConverter::FromGLFW(glfwPos);
    }

    void GLFWWindow::SetSize(SWindowCoords size)
    {
        GLFW_SWindowCoords glfwSize = GLFW_WindowCoordsConverter::ToGLFW(size);

        glfwSetWindowSize(m_glfwWindow, glfwSize.x, glfwSize.y);
        m_windowSize = GLFW_WindowCoordsConverter::FromGLFW(glfwSize);
    }

    SWindowCoords GLFWWindow::GetSize() const noexcept
    {
        return m_windowSize;
    }

    SWindowCoords GLFWWindow::UpdateSize()
    {
        GLFW_SWindowCoords glfwSize;
        glfwGetWindowSize(m_glfwWindow, &glfwSize.x, &glfwSize.y);

        m_windowSize = GLFW_WindowCoordsConverter::FromGLFW(glfwSize);
        return m_windowSize;
    }

    void GLFWWindow::SetVSync(bool enable)
    {
        glfwSwapInterval(enable ? 1 : 0);
        m_windowVSync = enable;
    }

    bool GLFWWindow::GetVSync() const noexcept
    {
        return m_windowVSync;
    }

    void* GLFWWindow::GetNative() const noexcept
    {
        return m_glfwWindow;
    }
}