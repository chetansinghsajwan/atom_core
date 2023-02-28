#pragma once
#include <initializer_list>

#include "Atom/Core.h"

namespace Atom
{
    template <typename TElement>
    using ConstArrayIterator = TElement*;

    template <typename TElement>
    class InitializerList
    {
        using StdInitializerList = ::std::initializer_list<TElement>;

    public:
        using TConstIterator = ConstArrayIterator<TElement>;
        using TIterator = ConstArrayIterator<TElement>;

    public:
        constexpr InitializerList(StdInitializerList initList) noexcept:
            _arr(initList.begin()), _count(initList.size()) { }

//// -----------------------------------------------------------------------------------------------
//// Iterator support.
//// -----------------------------------------------------------------------------------------------

    public:
        constexpr TConstIterator Begin() const noexcept
        {
            return TConstIterator(_arr);
        }

        constexpr TConstIterator ConstBegin() const noexcept
        {
            return TConstIterator(_arr);
        }

        constexpr TConstIterator End() const noexcept
        {
            return TConstIterator(_arr + _count);
        }

        constexpr TConstIterator ConstEnd() const noexcept
        {
            return TConstIterator(_arr + _count);
        }

//// -----------------------------------------------------------------------------------------------
//// STD Iterator support.
//// -----------------------------------------------------------------------------------------------

    public:
        constexpr TConstIterator begin() const noexcept
        {
            return Begin();
        }

        constexpr TConstIterator cbegin() const noexcept
        {
            return ConstBegin();
        }

        constexpr TConstIterator end() const noexcept
        {
            return End();
        }

        constexpr TConstIterator cend() const noexcept
        {
            return ConstEnd();
        }

    public:
        constexpr SizeT Count() const noexcept
        {
            return _count;
        }

        constexpr bool IsEmpty() const noexcept
        {
            return _count == 0;
        }

    protected:
        const TElement* _arr;
        const SizeT _count;
    };
}