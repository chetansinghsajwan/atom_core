#include "Atom/Logging.h"
#include "Window/GlfwWindow.h"

using namespace Atom::Logging;

namespace Atom::Engine
{
    GlfwWindow::GlfwWindow(const WindowProps& props):
        Window(_windowEventSource)
    {
        GlfwSWindowCoords glfwWindowSize = GlfwWindowCoordsConverter::ToGLFW(
            props.windowSize);

        // TODO: Requires encoding conversion.
        _glfwWindow = glfwCreateWindow(glfwWindowSize.x, glfwWindowSize.y,
            (const char*)props.windowName.data(), nullptr, nullptr);

        glfwMakeContextCurrent(_glfwWindow);
        glfwSetWindowUserPointer(_glfwWindow, this);

        glfwSetWindowPosCallback(_glfwWindow,
            [](GLFWwindow* glfwWindow, i32 xpos, i32 ypos)
            {
                GlfwWindow& window = *reinterpret_cast<GlfwWindow*>(
                    glfwGetWindowUserPointer(glfwWindow));

                SWindowCoords oldPos = window._windowPos;
                SWindowCoords newPos = GlfwWindowCoordsConverter::FromGLFW({ xpos, ypos });
                window._windowPos = newPos;

                window._windowEventSource.Dispatch(SWindowRepositionEvent(
                    newPos, newPos - oldPos));
            });

        glfwSetWindowSizeCallback(_glfwWindow,
            [](GLFWwindow* glfwWindow, i32 width, i32 height)
            {
                GlfwWindow& window = *reinterpret_cast<GlfwWindow*>(
                    glfwGetWindowUserPointer(glfwWindow));

                SWindowCoords oldSize = window._windowSize;
                SWindowCoords newSize = GlfwWindowCoordsConverter::FromGLFW({ width, height });
                window._windowSize = newSize;

                window._windowEventSource.Dispatch(SWindowResizeEvent(
                    newSize, newSize - oldSize));
            });

        glfwSetWindowCloseCallback(_glfwWindow,
            [](GLFWwindow* glfwWindow)
            {
                GlfwWindow& window = *reinterpret_cast<GlfwWindow*>(
                    glfwGetWindowUserPointer(glfwWindow));

                window._windowEventSource.Dispatch(SWindowCloseEvent());
            });

        UpdatePos();
        UpdateSize();
        SetVSync(true);
    }

    GlfwWindow::~GlfwWindow()
    {
        glfwDestroyWindow(_glfwWindow);
    }

    auto GlfwWindow::Update() -> void
    {
        glfwPollEvents();
        glfwSwapBuffers(_glfwWindow);
    }

    auto GlfwWindow::SetPos(SWindowCoords pos) -> void
    {
        GlfwSWindowCoords glfwPos = GlfwWindowCoordsConverter::ToGLFW(pos);

        glfwSetWindowPos(_glfwWindow, glfwPos.x, glfwPos.y);
        _windowPos = GlfwWindowCoordsConverter::FromGLFW(glfwPos);
    }

    auto GlfwWindow::GetPos() const -> SWindowCoords
    {
        return _windowPos;
    }

    auto GlfwWindow::UpdatePos() -> SWindowCoords
    {
        GlfwSWindowCoords glfwPos;
        glfwGetWindowPos(_glfwWindow, &glfwPos.x, &glfwPos.y);

        return GlfwWindowCoordsConverter::FromGLFW(glfwPos);
    }

    auto GlfwWindow::SetSize(SWindowCoords size) -> void
    {
        GlfwSWindowCoords glfwSize = GlfwWindowCoordsConverter::ToGLFW(size);

        glfwSetWindowSize(_glfwWindow, glfwSize.x, glfwSize.y);
        _windowSize = GlfwWindowCoordsConverter::FromGLFW(glfwSize);
    }

    auto GlfwWindow::GetSize() const -> SWindowCoords
    {
        return _windowSize;
    }

    auto GlfwWindow::UpdateSize() -> SWindowCoords
    {
        GlfwSWindowCoords glfwSize;
        glfwGetWindowSize(_glfwWindow, &glfwSize.x, &glfwSize.y);

        _windowSize = GlfwWindowCoordsConverter::FromGLFW(glfwSize);
        return _windowSize;
    }

    auto GlfwWindow::SetVSync(bool enable) -> void
    {
        glfwSwapInterval(enable ? 1 : 0);
        _windowVSync = enable;
    }

    auto GlfwWindow::GetVSync() const -> bool
    {
        return _windowVSync;
    }

    auto GlfwWindow::GetNative() const -> void*
    {
        return _glfwWindow;
    }
}