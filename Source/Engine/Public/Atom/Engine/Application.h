#pragma once
#include "Atom/Engine/Windows.h"

namespace Atom::Engine
{
    class Application
    {
    public:
        Application();
        virtual ~Application();

    public:
        virtual void Run();

        virtual void OnWindowEvent(const SWindowEvent& event);

    protected:
        Window* _window;
    };

    extern Application* CreateApplication();
}