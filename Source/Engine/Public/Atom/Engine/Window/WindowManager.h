#pragma once
#include "Atom/Core.h"
#include "Atom/Memory.h"
#include "Atom/Engine/Window/Window.h"

namespace Atom::Engine
{
    class WindowProps
    {
    public:
        Str windowName;
        SWindowCoords windowSize;
    };

    class WindowManger
    {
    public:
        static fn CreateWindow(WindowProps props) -> Window*;

        static fn CloseWindow(Window* window) -> void;

    protected:
        static usize s_windowCount;
    };
}