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
    class GlfwSWindowCoords
    {
      public:
        i32 x;
        i32 y;
    };

    class GlfwWindowCoordsConverter
    {
      public:
        static constexpr fn ToGLFW(SWindowCoords coords)->GlfwSWindowCoords
        {
            static constexpr i32 min = NumLimits<i32>::min();
            static constexpr i32 max = NumLimits<i32>::max();

            coords.x = Math::Clamp<i32>(coords.x, min, max);
            coords.y = Math::Clamp<i32>(coords.y, min, max);

            return { coords.x, coords.y };
        };

        static constexpr fn FromGLFW(GlfwSWindowCoords coords)->SWindowCoords
        {
            static constexpr i32 min = NumLimits<i32>::min();
            static constexpr i32 max = NumLimits<i32>::max();

            coords.x = Math::Clamp<i32>(coords.x, min, max);
            coords.y = Math::Clamp<i32>(coords.y, min, max);

            return { coords.x, coords.y };
        };
    };

    class GlfwWindow: public Window
    {
      public:
        ctor GlfwWindow(const WindowProps& props);
        dtor GlfwWindow();

        virtual fn Update()->void override;

        virtual fn SetPos(SWindowCoords size)->void override;
        virtual fn GetPos() const->SWindowCoords override;
        virtual fn UpdatePos()->SWindowCoords;

        virtual fn SetSize(SWindowCoords size)->void override;
        virtual fn GetSize() const->SWindowCoords override;
        virtual fn UpdateSize()->SWindowCoords;

        virtual fn GetNative() const->void* ofinal;

        fn SetVSync(bool enable)->void;
        fn GetVSync() const->bool;

        fn GetNativeGLFW() const->GLFWwindow*
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
