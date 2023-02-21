#pragma once
#include "Atom/Core.h"
#include "Atom/Memory.h"
#include "Atom/Engine/Window/Window.h"

namespace Atom::Engine
{
    struct WindowProps
    {
        String windowName;
        Vector2 dimensions;
    };

    class ATOM_API WindowManger
    {
    public:
        static UniquePtr<Window> CreateWindow(WindowProps props);

        static void CloseWindow(UniquePtr<Window> window);
    };
}