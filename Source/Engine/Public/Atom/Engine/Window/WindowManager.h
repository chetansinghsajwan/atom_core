#pragma once
#include "Atom/Core.h"
#include "Atom/Memory.h"
#include "Atom/Engine/Window/Window.h"

namespace Atom::Engine
{
    struct SWindowProps
    {
        String windowName;
        SWindowCoords windowSize;
    };

    class ATOM_API WindowManger
    {
    public:
        static Window* CreateWindow(SWindowProps props);

        static void CloseWindow(Window* window);

    protected:
        static SizeT s_windowCount;
    };
}