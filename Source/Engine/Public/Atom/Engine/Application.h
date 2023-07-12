#pragma once
#include "Atom/Engine/Windows.h"

namespace Atom::Engine
{
    class Application
    {
    public:
        ctor Application();
        virtual dtor Application();

    public:
        virtual fn Run() -> void;

        virtual fn OnWindowEvent(const SWindowEvent& event) -> void;

    protected:
        Window* _window;
    };

    extern Application* CreateApplication();
}