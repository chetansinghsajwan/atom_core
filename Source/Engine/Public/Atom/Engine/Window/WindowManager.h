#pragma once
#include "Atom/Core.h"
#include "Atom/Memory.h"
#include "Atom/Engine/Window/Window.h"

namespace Atom::Engine
{
    class WindowProps
    {
        pub Str windowName;
        pub SWindowCoords windowSize;
    };

    class WindowManger
    {
        pub static fn CreateWindow(WindowProps props) -> Window*;

        pub static fn CloseWindow(Window* window) -> void;

        prot static usize s_windowCount;
    };
}