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

    struct Window
    {
    public:
        Window(IEvent<const SWindowEvent&>& event) noexcept:
            OnEvent{ event } { }

        virtual ~Window() = default;

    public:
        virtual void Update() = 0;

        virtual SVector2 GetSize() const noexcept = 0;
        virtual void SetSize(SVector2 size) = 0;

        virtual SVector2 GetPos() const noexcept = 0;
        virtual void SetPos(SVector2 pos) = 0;

        virtual void* GetNative() const noexcept = 0;

    public:
        IEvent<const SWindowEvent&>& OnEvent;
    };
}