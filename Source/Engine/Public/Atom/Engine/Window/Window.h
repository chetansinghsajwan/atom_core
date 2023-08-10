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
    public:
        i32 x;
        i32 y;
    };

    inline fn operator-(const SWindowCoords& lhs, const SWindowCoords& rhs) -> SWindowCoords
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
    public:
        SWindowEvent(EWindowEventType eventType):
            eventType(eventType) { }

    public:
        const EWindowEventType eventType;
    };

    class SWindowResizeEvent : public SWindowEvent
    {
    public:
        SWindowResizeEvent(SWindowCoords size, SWindowCoords delta):
            size(size), delta(delta),
            SWindowEvent(EWindowEventType::Resize) { }

    public:
        SWindowCoords size;
        SWindowCoords delta;
    };

    class SWindowRepositionEvent : public SWindowEvent
    {
    public:
        SWindowRepositionEvent(SWindowCoords position, SWindowCoords delta):
            position(position), delta(delta),
            SWindowEvent(EWindowEventType::Reposition) { }

    public:
        SWindowCoords position;
        SWindowCoords delta;
    };

    class SWindowCloseEvent : public SWindowEvent
    {
    public:
        SWindowCloseEvent():
            SWindowEvent(EWindowEventType::Close) { }
    };

    class Window
    {
    public:
        Window(IEvent<const SWindowEvent&>& event):
            OnEvent{ event } { }

    public:
        virtual ~Window() = default;

        virtual fn Update() -> void abstract;

        virtual fn GetSize() const -> SWindowCoords abstract;
        virtual fn SetSize(SWindowCoords size) -> void abstract;

        virtual fn GetPos() const -> SWindowCoords abstract;
        virtual fn SetPos(SWindowCoords pos) -> void abstract;

        virtual fn GetNative() const -> void* abstract;

    public:
        IEvent<const SWindowEvent&>& OnEvent;
    };
}