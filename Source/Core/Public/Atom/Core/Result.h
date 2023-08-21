#pragma once
#include "Variant.h"

namespace Atom
{
    template <typename T, typename... TErrs>
    class Result: public Variant<T, TErrs...>
    {
        using Base = Variant<T, TErrs...>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    class _ResultVoid {};

    template <typename... TErrs>
    class Result<void, TErrs...>: public Variant<_ResultVoid, TErrs...>
    {
        using Base = Variant<_ResultVoid, TErrs...>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    template <typename... TErrs>
    constexpr auto Success() -> Result<void, TErrs...>
    {
        return { _ResultVoid{} };
    }
}