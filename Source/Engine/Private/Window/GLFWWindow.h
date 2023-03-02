#pragma once
#include "GLFW/glfw3.h"
#include "Atom/Engine/Window/WindowManager.h"

namespace Atom::Engine
{
    class GLFWWindow: public Window
    {
    public:
        GLFWWindow(const WindowProps& props);
        ~GLFWWindow();

    public:
        virtual void Update() override;

        virtual void SetPos(SVector2 size) override;
        virtual SVector2 GetPos() const noexcept override;
        virtual SVector2 UpdatePos();

        virtual void SetSize(SVector2 size) override;
        virtual SVector2 GetSize() const noexcept override;
        virtual SVector2 UpdateSize();

        void SetVSync(bool enable);
        bool GetVSync() const noexcept;

        virtual void* GetNative() const noexcept override final;

        GLFWwindow* GetNativeGLFW() const noexcept
        {
            return _glfwWindow;
        }

    protected:
        GLFWwindow* _glfwWindow;
        SVector2 _windowPos;
        SVector2 _windowSize;
        bool _windowVSync;
    };
}