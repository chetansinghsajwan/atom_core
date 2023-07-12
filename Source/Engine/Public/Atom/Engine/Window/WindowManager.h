#pragma once
#include "Atom/Core.h"
#include "Atom/Memory.h"
#include "Atom/Engine/Window/Window.h"

namespace Atom::Engine
{
    struct SWindowProps
    {
        Str windowName;
        SWindowCoords windowSize;
    };

    class ATOM_API WindowManger
    {
    public:
        static fn CreateWindow(SWindowProps props) -> Window*;

        static fn CloseWindow(Window* window) -> void;

    protected:
        static usize s_windowCount;
    };
}