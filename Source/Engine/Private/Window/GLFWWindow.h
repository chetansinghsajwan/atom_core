#pragma once
#include "GLFW/glfw3.h"
#include "Atom/Engine/Window/WindowManager.h"

#include "Atom/Math.h"

namespace Atom::Engine
{
    struct GLFW_SWindowCoords
    {
        int x;
        int y;
    };

    struct GLFW_WindowCoordsConverter
    {
        static constexpr fn ToGLFW(SWindowCoords coords) -> GLFW_SWindowCoords
        {
            coords.x = Math::Clamp<int>(coords.x, NumLimits<int>::min(), NumLimits<int>::max());
            coords.y = Math::Clamp<int>(coords.y, NumLimits<int>::min(), NumLimits<int>::max());

            return { (int)coords.x, (int)coords.y };
        };

        static constexpr fn FromGLFW(GLFW_SWindowCoords coords) -> SWindowCoords
        {
            coords.x = Math::Clamp<int>(coords.x, NumLimits<int>::min(), NumLimits<int>::max());
            coords.y = Math::Clamp<int>(coords.y, NumLimits<int>::min(), NumLimits<int>::max());

            return { (int)coords.x, (int)coords.y };
        };
    };

    class GLFWWindow: public Window
    {
    public:
        ctor GLFWWindow(const SWindowProps& props);
        dtor GLFWWindow();

    public:
        virtual fn Update() -> void override;

        virtual fn SetPos(SWindowCoords size) -> void override;
        virtual fn GetPos() const noex -> SWindowCoords override;
        virtual fn UpdatePos() -> SWindowCoords;

        virtual fn SetSize(SWindowCoords size) -> void override;
        virtual fn GetSize() const noex -> SWindowCoords override;
        virtual fn UpdateSize() -> SWindowCoords;

        virtual fn GetNative() const noex -> void* ofinal;

        fn SetVSync(bool enable) -> void;
        fn GetVSync() const noex -> bool;

        fn GetNativeGLFW() const noex -> GLFWwindow*
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