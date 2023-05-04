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
    public:
        static constexpr GLFW_SWindowCoords ToGLFW(SWindowCoords coords)
        {
            coords.x = Math::Clamp<int>(coords.x, NumLimits<int>::min(), NumLimits<int>::max());
            coords.y = Math::Clamp<int>(coords.y, NumLimits<int>::min(), NumLimits<int>::max());

            return { (int)coords.x, (int)coords.y };
        };

        static constexpr SWindowCoords FromGLFW(GLFW_SWindowCoords coords)
        {
            coords.x = Math::Clamp<int>(coords.x, NumLimits<int>::min(), NumLimits<int>::max());
            coords.y = Math::Clamp<int>(coords.y, NumLimits<int>::min(), NumLimits<int>::max());

            return { (int)coords.x, (int)coords.y };
        };
    };

    class GLFWWindow: public Window
    {
    public:
        GLFWWindow(const SWindowProps& props);
        ~GLFWWindow();

    public:
        virtual void Update() override;

        virtual void SetPos(SWindowCoords size) override;
        virtual SWindowCoords GetPos() const noexcept override;
        virtual SWindowCoords UpdatePos();

        virtual void SetSize(SWindowCoords size) override;
        virtual SWindowCoords GetSize() const noexcept override;
        virtual SWindowCoords UpdateSize();

        void SetVSync(bool enable);
        bool GetVSync() const noexcept;

        virtual void* GetNative() const noexcept override final;

        GLFWwindow* GetNativeGLFW() const noexcept
        {
            return m_glfwWindow;
        }

    protected:
        GLFWwindow* m_glfwWindow;
        SWindowCoords m_windowPos;
        SWindowCoords m_windowSize;
        bool m_windowVSync;

        EventSource<const SWindowEvent&> m_windowEventSource;
    };
}