#pragma once
#include "Atom/Engine/Windows.h"

namespace Atom::Engine
{
    class Application
    {
        pub ctor Application();
        pub virtual dtor Application();

        pub virtual fn Run() -> void;

        pub virtual fn OnWindowEvent(const SWindowEvent& event) -> void;

        pro Window* _window;
    };

    extern Application* CreateApplication();
}