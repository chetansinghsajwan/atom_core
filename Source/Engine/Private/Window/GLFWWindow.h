#pragma once

/// # To Do
/// 
/// - Refactor this.
#undef op
#include "GLFW/glfw3.h"
#define op operator

#include "Atom/Engine/Window/WindowManager.h"
#include "Atom/Math.h"

namespace Atom::Engine
{
    class GLFW_SWindowCoords
    {
        pub i32 x;
        pub i32 y;
    };

    class GLFW_WindowCoordsConverter
    {
        pub static cexpr fn ToGLFW(SWindowCoords coords) -> GLFW_SWindowCoords
        {
            static constexpr i32 min = NumLimits<i32>::min();
            static constexpr i32 max = NumLimits<i32>::max();

            coords.x = Math::Clamp<i32>(coords.x, min, max);
            coords.y = Math::Clamp<i32>(coords.y, min, max);

            return { coords.x, coords.y };
        };

        pub static cexpr fn FromGLFW(GLFW_SWindowCoords coords) -> SWindowCoords
        {
            static constexpr i32 min = NumLimits<i32>::min();
            static constexpr i32 max = NumLimits<i32>::max();

            coords.x = Math::Clamp<i32>(coords.x, min, max);
            coords.y = Math::Clamp<i32>(coords.y, min, max);

            return { coords.x, coords.y };
        };
    };

    class GLFWWindow: public Window
    {
        pub ctor GLFWWindow(const WindowProps& props);
        pub dtor GLFWWindow();

        pub virtual fn Update() -> void override;

        pub virtual fn SetPos(SWindowCoords size) -> void override;
        pub virtual fn GetPos() const -> SWindowCoords override;
        pub virtual fn UpdatePos() -> SWindowCoords;

        pub virtual fn SetSize(SWindowCoords size) -> void override;
        pub virtual fn GetSize() const -> SWindowCoords override;
        pub virtual fn UpdateSize() -> SWindowCoords;

        pub virtual fn GetNative() const -> void* ofinal;

        pub fn SetVSync(bool enable) -> void;
        pub fn GetVSync() const -> bool;

        pub fn GetNativeGLFW() const -> GLFWwindow*
        {
            return _glfwWindow;
        }

        prot GLFWwindow* _glfwWindow;
        prot SWindowCoords _windowPos;
        prot SWindowCoords _windowSize;
        prot bool _windowVSync;

        prot EventSource<const SWindowEvent&> _windowEventSource;
    };
}