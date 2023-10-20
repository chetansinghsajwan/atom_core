#pragma once
#include "BasicStackStr.h"
#include "DefaultEncoding.h"

namespace Atom
{
    template <usize size>
    using StackStr = BasicStackStr<CharEncoding, size>;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////
    //// Primitives Implementations
    //// These implementations could not be defined before due to circular depenedencies.
    ////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <size_t size>
    class IntStr: public StackStr<size>
    {
        using Base = StackStr<size>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    template <size_t size>
    class FloatStr: public StackStr<size>
    {
        using Base = StackStr<size>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    template <typename TSelf, typename TVal, typename TLimit>
    constexpr auto _IntImpl<TSelf, TVal, TLimit>::ToStr(TVal val) -> TStr
    {
        TStr str;
        Char* begin = _ToStr(str.mutData() + str.count() - 1, val);
        begin--;
        while (begin >= str.data())
        {
            *begin = ' ';
            begin--;
        }

        return str;
    }

    template <typename TSelf, typename TVal>
    constexpr auto _FloatImpl<TSelf, TVal>::ToStr(TVal val) -> TStr
    {
        TStr str;
        Char* begin = _ToStr(str.mutData() + str.count() - 1, val);
        begin--;
        while (begin >= str.data())
        {
            *begin = ' ';
            begin--;
        }

        return str;
    }
}
