#pragma once
#include "Atom/String/BasicChar.h"
#include "Atom/Containers/StaticArray.h"

namespace Atom
{
    template <typename TEncoding, usize size>
    class BasicStaticString: public StaticArray<BasicChar<TEncoding>, size>
    {
        using TBase = StaticArray<BasicChar<TEncoding>, size>;

    public:
        using TChar = BasicChar<TEncoding>;
        using TIterator = typename TBase::TIterator;
        using TConstIterator = typename TBase::TConstIterator;
    };
}
