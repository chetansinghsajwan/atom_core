#pragma once
#include "BasicStackString.h"
#include "DefaultEncoding.h"

namespace Atom
{
    template <usize size>
    using StackString = BasicStackString<CharEncoding, size>;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////
    //// Primitives Implementations
    //// These implementations could not be defined before due to circular depenedencies.
    ////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <size_t size>
    class IntString: public StackString<size>
    {
        using Base = StackString<size>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    template <size_t size>
    class FloatString: public StackString<size>
    {
        using Base = StackString<size>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    template <typename TSelf, typename TVal, typename TLimit>
    constexpr auto _IntImpl<TSelf, TVal, TLimit>::ToString(TVal val) -> TString
    {
        TString str;
        Char* begin = _ToString(val, str.mutData() + str.count() - 1);
        begin--;
        while (begin >= str.data())
        {
            *begin = ' ';
            begin--;
        }

        return str;
    }

    template <typename TSelf, typename TVal>
    constexpr auto _FloatImpl<TSelf, TVal>::ToString(TVal val) -> TString
    {
        TString str;
        Char* begin = _ToString(val, str.mutData() + str.count() - 1);
        begin--;
        while (begin >= str.data())
        {
            *begin = ' ';
            begin--;
        }

        return str;
    }
}
