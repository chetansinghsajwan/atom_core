#pragma once
#include "RangeReq.h"

namespace Atom
{
    template <typename TInIter, typename TInIterEnd>
    struct Range
    {
    public:
        using TIter = TInIter;
        using TIterEnd = TInIterEnd;

    public:
        constexpr Range(TIter begin, TIterEnd end) noexcept:
            _begin{ begin }, _end{ end } { }

    public:
        constexpr TIter Begin() const noexcept
        {
            return _begin;
        }

        constexpr TIterEnd End() const noexcept
        {
            return _end;
        }

        constexpr TIter begin() const noexcept
        {
            return Begin();
        }

        constexpr TIterEnd end() const noexcept
        {
            return End();
        }

    protected:
        TIter _begin;
        TIterEnd _end;
    };
}