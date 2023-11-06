#pragma once
#include "Atom/Core.h"

namespace Atom
{
    template <typenae TVal_>
    class Ptr
    {
        using This = Ptr<TVal_>;

    public:
        using TVal = TVal_;

    public:
        constexpr Ptr(TVal* ptr):
            _ptr{ ptr } { }

        constexpr Ptr(const This& that) = default;
        constexpr Ptr& operator=(const This& that) = default;

        constexpr Ptr(This&& that) = default;
        constexpr Ptr& operator=(This&& that) = default;

        constexpr ~Ptr() = default;

    public:
        constexpr auto cval() const -> const TVal&
        {
            return *_ptr;
        }

        constexpr auto val() -> TVal&
        {
            return *_ptr;
        }

    private:
        TVal* _ptr;
    };

    template <typenae TVal>
    class MemPtr: public Ptr<TVal>
    {
        using This = MemPtr<TVal>;
        using Base = Ptr<TVal>;

    public:
        using Base::Base;
        using Base::operator=;

    public:
        constexpr auto operator+(usize n) const -> This
        {
            return _ptr + n.val();
        }

        constexpr auto operator-(usize n) const -> This
        {
            return _ptr - n.val();
        }
    };
}