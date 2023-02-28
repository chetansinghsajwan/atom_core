#pragma once
#include "Atom/Core.h"
#include "Atom/String.h"
#include "Atom/Memory.h"

namespace Atom
{
    struct Vector1
    {
        SizeT x;
    };

    struct Vector2
    {
        SizeT x;
        SizeT y;
    };

    struct Vector3
    {
        SizeT x;
        SizeT y;
        SizeT z;
    };
}

namespace Atom::Engine
{
    class Window
    {
    public:
        virtual ~Window() = default;

    public:
        virtual void OnUpdate() = 0;

        Vector2 GetDimensions() const noexcept
        {
            return _dimensions;
        }

    protected:
        Vector2 _dimensions;
    };
}