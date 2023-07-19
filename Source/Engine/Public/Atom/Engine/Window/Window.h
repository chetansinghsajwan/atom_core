#pragma once
#include "Atom/Core.h"
#include "Atom/Str.h"
#include "Atom/Memory.h"
#include "Atom/Invokable.h"
#include "Atom/Math.h"

namespace Atom::Engine
{
    class SWindowCoords
    {
        pub int x;
        pub int y;
    };

    cexpr fn op-(const SWindowCoords& lhs, const SWindowCoords& rhs) noex -> SWindowCoords
    {
        return { lhs.x - rhs.x, lhs.y - rhs.y };
    }

    enum class EWindowEventType
    {
        Resize,
        Reposition,
        Close,
    };

    class SWindowEvent
    {
        pub cexpr ctor SWindowEvent(EWindowEventType eventType) noex:
            eventType(eventType) { }

        pub const EWindowEventType eventType;
    };

    class SWindowResizeEvent extends SWindowEvent
    {
        pub cexpr ctor SWindowResizeEvent(SWindowCoords size, SWindowCoords delta) noex:
            size(size), delta(delta),
            SWindowEvent(EWindowEventType::Resize) { }

        pub SWindowCoords size;
        pub SWindowCoords delta;
    };

    class SWindowRepositionEvent extends SWindowEvent
    {
        pub cexpr ctor SWindowRepositionEvent(SWindowCoords position, SWindowCoords delta) noex:
            position(position), delta(delta),
            SWindowEvent(EWindowEventType::Reposition) { }

        pub SWindowCoords position;
        pub SWindowCoords delta;
    };

    class SWindowCloseEvent extends SWindowEvent
    {
        pub cexpr ctor SWindowCloseEvent() noex:
            SWindowEvent(EWindowEventType::Close) { }
    };

    class Window
    {
        pub ctor Window(IEvent<const SWindowEvent&>& event) noex:
            OnEvent{ event } { }

        pub virtual dtor Window() = default;

        pub virtual fn Update() -> void abstract;

        pub virtual fn GetSize() const noex -> SWindowCoords abstract;
        pub virtual fn SetSize(SWindowCoords size) -> void abstract;

        pub virtual fn GetPos() const noex -> SWindowCoords abstract;
        pub virtual fn SetPos(SWindowCoords pos) -> void abstract;

        pub virtual fn GetNative() const noex -> void* abstract;

        pub IEvent<const SWindowEvent&>& OnEvent;
    };
}