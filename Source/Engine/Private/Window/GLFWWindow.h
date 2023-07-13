#pragma once
#include "GLFW/glfw3.h"
#include "Atom/Engine/Window/WindowManager.h"

#include "Atom/Math.h"

namespace Atom::Engine
{
    class GLFW_SWindowCoords
    {
        pub int x;
        pub int y;
    };

    class GLFW_WindowCoordsConverter
    {
        pub static cexpr fn ToGLFW(SWindowCoords coords) -> GLFW_SWindowCoords
        {
            coords.x = Math::Clamp<int>(coords.x, NumLimits<int>::min(), NumLimits<int>::max());
            coords.y = Math::Clamp<int>(coords.y, NumLimits<int>::min(), NumLimits<int>::max());

            return { (int)coords.x, (int)coords.y };
        };

        pub static cexpr fn FromGLFW(GLFW_SWindowCoords coords) -> SWindowCoords
        {
            coords.x = Math::Clamp<int>(coords.x, NumLimits<int>::min(), NumLimits<int>::max());
            coords.y = Math::Clamp<int>(coords.y, NumLimits<int>::min(), NumLimits<int>::max());

            return { (int)coords.x, (int)coords.y };
        };
    };

    class GLFWWindow: public Window
    {
        pub ctor GLFWWindow(const WindowProps& props);
        pub dtor GLFWWindow();

        pub virtual fn Update() -> void override;

        pub virtual fn SetPos(SWindowCoords size) -> void override;
        pub virtual fn GetPos() const noex -> SWindowCoords override;
        pub virtual fn UpdatePos() -> SWindowCoords;

        pub virtual fn SetSize(SWindowCoords size) -> void override;
        pub virtual fn GetSize() const noex -> SWindowCoords override;
        pub virtual fn UpdateSize() -> SWindowCoords;

        pub virtual fn GetNative() const noex -> void* ofinal;

        pub fn SetVSync(bool enable) -> void;
        pub fn GetVSync() const noex -> bool;

        pub fn GetNativeGLFW() const noex -> GLFWwindow*
        {
            return _glfwWindow;
        }

        pro GLFWwindow* _glfwWindow;
        pro SWindowCoords _windowPos;
        pro SWindowCoords _windowSize;
        pro bool _windowVSync;

        pro EventSource<const SWindowEvent&> _windowEventSource;
    };
}