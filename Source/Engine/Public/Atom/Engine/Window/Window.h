#pragma once
#include "Atom/Core.h"
#include "Atom/Str.h"
#include "Atom/Memory.h"
#include "Atom/Invokable.h"
#include "Atom/Math.h"

namespace Atom::Engine
{
    struct SWindowCoords
    {
        int x;
        int y;
    };

    constexpr SWindowCoords operator - (const SWindowCoords& lhs, const SWindowCoords& rhs) noex
    {
        return { lhs.x - rhs.x, lhs.y - rhs.y };
    }

    enum struct EWindowEventType
    {
        Resize,
        Reposition,
        Close,
    };

    struct SWindowEvent
    {
        constexpr SWindowEvent(EWindowEventType eventType) noex:
            eventType(eventType) { }

        const EWindowEventType eventType;
    };

    struct SWindowResizeEvent: public SWindowEvent
    {
        constexpr SWindowResizeEvent(SWindowCoords size, SWindowCoords delta) noex:
            size(size), delta(delta),
            SWindowEvent(EWindowEventType::Resize) { }

        SWindowCoords size;
        SWindowCoords delta;
    };

    struct SWindowRepositionEvent: public SWindowEvent
    {
        constexpr SWindowRepositionEvent(SWindowCoords position, SWindowCoords delta) noex:
            position(position), delta(delta),
            SWindowEvent(EWindowEventType::Reposition) { }

        SWindowCoords position;
        SWindowCoords delta;
    };

    struct SWindowCloseEvent: public SWindowEvent
    {
        constexpr SWindowCloseEvent() noex:
            SWindowEvent(EWindowEventType::Close) { }
    };

    struct Window
    {
    public:
        ctor Window(IEvent<const SWindowEvent&>& event) noex:
            OnEvent{ event } { }

        virtual dtor Window() = default;

    public:
        virtual fn Update() -> void abstract;

        virtual fn GetSize() const noex -> SWindowCoords abstract;
        virtual fn SetSize(SWindowCoords size) -> void abstract;

        virtual fn GetPos() const noex -> SWindowCoords abstract;
        virtual fn SetPos(SWindowCoords pos) -> void abstract;

        virtual fn GetNative() const noex -> void* abstract;

    public:
        IEvent<const SWindowEvent&>& OnEvent;
    };
}