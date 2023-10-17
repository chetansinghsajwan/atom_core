#pragma once
#include "BasicStackStr.h"
#include "DefaultEncoding.h"

namespace Atom
{
    template <usize size>
    using StackStr = BasicStackStr<CharEncoding, size>;

    template <_usize size>
    class NumStr: public StackStr<size>
    {
        using Base = StackStr<size>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    template <typename T, typename TLimits>
    constexpr auto Num<T, TLimits>::toStr() -> TStr
    {
        TStr str;
        Char* begin = _Impl::ToString(str.mutData() + str.count() - 1, _val);
        begin--;
        while (begin >= str.data())
            *begin = ' ';

        return str;
    }
}
