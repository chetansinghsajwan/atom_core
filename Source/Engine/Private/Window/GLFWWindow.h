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
        void OnUpdate() override;

        void EnableVSync();
        void DisableVSync();
        bool IsVSyncEnabled() const noexcept;

    protected:
        GLFWwindow* _glfwWindow;
        SVector2 _position;
        SVector2 _size;
        bool _vSyncEnabled;
    };
}