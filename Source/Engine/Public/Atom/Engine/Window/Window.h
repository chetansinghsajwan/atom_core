#pragma once
#include "Atom/Core.h"
#include "Atom/String.h"
#include "Atom/Memory.h"
#include "Atom/Invokable.h"

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

        Vector2 operator - (Vector2 rhs)
        {
            return { x - rhs.x, y - rhs.y };
        }
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
    enum struct EWindowEventType
    {
        Resize,
        Reposition,
        Close,
    };

    struct SWindowEvent
    {
        constexpr SWindowEvent(EWindowEventType eventType) noexcept:
            eventType(eventType) { }

        const EWindowEventType eventType;
    };

    struct SWindowResizeEvent: public SWindowEvent
    {
        constexpr SWindowResizeEvent(Vector2 size, Vector2 delta) noexcept:
            size(size), delta(delta),
            SWindowEvent(EWindowEventType::Resize) { }

        Vector2 size;
        Vector2 delta;
    };

    struct SWindowRepositionEvent: public SWindowEvent
    {
        constexpr SWindowRepositionEvent(Vector2 position, Vector2 delta) noexcept:
            position(position), delta(delta),
            SWindowEvent(EWindowEventType::Reposition) { }

        Vector2 position;
        Vector2 delta;
    };

    struct SWindowCloseEvent: public SWindowEvent
    {
        constexpr SWindowCloseEvent() noexcept:
            SWindowEvent(EWindowEventType::Close) { }
    };

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

    public:
        Event<const SWindowEvent&> OnEvent = _eventSource;

    protected:
        EventSource<const SWindowEvent&> _eventSource;
        Vector2 _dimensions;
    };
}