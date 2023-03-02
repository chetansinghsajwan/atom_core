#pragma once
#include "Atom/Core.h"
#include "Atom/String.h"
#include "Atom/Memory.h"
#include "Atom/Invokable.h"
#include "Atom/Math.h"

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
        constexpr SWindowResizeEvent(SVector2 size, SVector2 delta) noexcept:
            size(size), delta(delta),
            SWindowEvent(EWindowEventType::Resize) { }

        SVector2 size;
        SVector2 delta;
    };

    struct SWindowRepositionEvent: public SWindowEvent
    {
        constexpr SWindowRepositionEvent(SVector2 position, SVector2 delta) noexcept:
            position(position), delta(delta),
            SWindowEvent(EWindowEventType::Reposition) { }

        SVector2 position;
        SVector2 delta;
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

        SVector2 GetDimensions() const noexcept
        {
            return _dimensions;
        }

    public:
        Event<const SWindowEvent&> OnEvent = _eventSource;

    protected:
        EventSource<const SWindowEvent&> _eventSource;
        SVector2 _dimensions;
    };
}