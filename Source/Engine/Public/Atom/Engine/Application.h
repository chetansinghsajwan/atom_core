#pragma once
#include "Atom/Core.h"
#include "Atom/Engine/Window/Window.h"
#include "Atom/Engine/Window/WindowManager.h"

namespace Atom::Engine
{
    class ATOM_API Application
    {
    public:
        Application();
        virtual ~Application();

    public:
        virtual void Run();

    protected:
        UniquePtr<Window> _window;
    };

    extern Application* CreateApplication();
}