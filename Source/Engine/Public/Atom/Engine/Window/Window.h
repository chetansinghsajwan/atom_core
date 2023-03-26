#pragma once
#include "Atom/Core.h"
#include "Atom/String.h"
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

    constexpr SWindowCoords operator - (const SWindowCoords& lhs, const SWindowCoords& rhs) noexcept
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
        constexpr SWindowEvent(EWindowEventType eventType) noexcept:
            eventType(eventType) { }

        const EWindowEventType eventType;
    };

    struct SWindowResizeEvent: public SWindowEvent
    {
        constexpr SWindowResizeEvent(SWindowCoords size, SWindowCoords delta) noexcept:
            size(size), delta(delta),
            SWindowEvent(EWindowEventType::Resize) { }

        SWindowCoords size;
        SWindowCoords delta;
    };

    struct SWindowRepositionEvent: public SWindowEvent
    {
        constexpr SWindowRepositionEvent(SWindowCoords position, SWindowCoords delta) noexcept:
            position(position), delta(delta),
            SWindowEvent(EWindowEventType::Reposition) { }

        SWindowCoords position;
        SWindowCoords delta;
    };

    struct SWindowCloseEvent: public SWindowEvent
    {
        constexpr SWindowCloseEvent() noexcept:
            SWindowEvent(EWindowEventType::Close) { }
    };

    struct Window
    {
    public:
        Window(IEvent<const SWindowEvent&>& event) noexcept:
            OnEvent{ event } { }

        virtual ~Window() = default;

    public:
        virtual void Update() = 0;

        virtual SWindowCoords GetSize() const noexcept = 0;
        virtual void SetSize(SWindowCoords size) = 0;

        virtual SWindowCoords GetPos() const noexcept = 0;
        virtual void SetPos(SWindowCoords pos) = 0;

        virtual void* GetNative() const noexcept = 0;

    public:
        IEvent<const SWindowEvent&>& OnEvent;
    };
}