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
        Window(IEvent<const SWindowEvent&>& event) noex:
            OnEvent{ event } { }

        virtual ~Window() = default;

    public:
        virtual void Update() = 0;

        virtual SWindowCoords GetSize() const noex = 0;
        virtual void SetSize(SWindowCoords size) = 0;

        virtual SWindowCoords GetPos() const noex = 0;
        virtual void SetPos(SWindowCoords pos) = 0;

        virtual void* GetNative() const noex = 0;

    public:
        IEvent<const SWindowEvent&>& OnEvent;
    };
}