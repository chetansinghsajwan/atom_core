#pragma once

/// # To Do
///
/// - Refactor this.
#undef operator
#include "GLFW/glfw3.h"

#include "Atom/Engine/Window/WindowManager.h"
#include "Atom/Math.h"

namespace Atom::Engine
{
    class GlfwSWindowCoords
    {
    public:
        i32 x;
        i32 y;
    };

    class GlfwWindowCoordsConverter
    {
    public:
        static constexpr auto ToGLFW(SWindowCoords coords) -> GlfwSWindowCoords
        {
            constexpr i32 min = NumLimits<i32>::min();
            constexpr i32 max = NumLimits<i32>::max();

            coords.x = Math::Clamp<i32>(coords.x, min, max);
            coords.y = Math::Clamp<i32>(coords.y, min, max);

            return { coords.x, coords.y };
        };

        static constexpr auto FromGLFW(GlfwSWindowCoords coords) -> SWindowCoords
        {
            constexpr i32 min = NumLimits<i32>::min();
            constexpr i32 max = NumLimits<i32>::max();

            coords.x = Math::Clamp<i32>(coords.x, min, max);
            coords.y = Math::Clamp<i32>(coords.y, min, max);

            return { coords.x, coords.y };
        };
    };

    class GlfwWindow: public Window
    {
    public:
        GlfwWindow(const WindowProps& props);
        ~GlfwWindow();

        virtual auto Update() -> void override;

        virtual auto SetPos(SWindowCoords size) -> void override;
        virtual auto GetPos() const -> SWindowCoords override;
        virtual auto UpdatePos() -> SWindowCoords;

        virtual auto SetSize(SWindowCoords size) -> void override;
        virtual auto GetSize() const -> SWindowCoords override;
        virtual auto UpdateSize() -> SWindowCoords;

        virtual auto GetNative() const -> void* override final;

        auto SetVSync(bool enable) -> void;
        auto GetVSync() const -> bool;

        auto GetNativeGLFW() const -> GLFWwindow*
        {
            return _glfwWindow;
        }

    protected:
        GLFWwindow* _glfwWindow;
        SWindowCoords _windowPos;
        SWindowCoords _windowSize;
        bool _windowVSync;

        EventSource<const SWindowEvent&> _windowEventSource;
    };
}
