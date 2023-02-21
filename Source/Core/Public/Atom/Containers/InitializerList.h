#pragma once
#include <initializer_list>

#include "Atom/Core.h"

namespace Atom
{
    template <typename ElementType>
    using ConstArrayIterator = ElementType*;

    template <typename ElementType>
    class InitializerList
    {
        using StdInitializerList = ::std::initializer_list<ElementType>;

    public:
        using ConstIteratorType = ConstArrayIterator<ElementType>;
        using IteratorType = ConstArrayIterator<ElementType>;

    public:
        constexpr InitializerList(StdInitializerList initList) noexcept:
            _arr(initList.begin()), _count(initList.size()) { }

//// -----------------------------------------------------------------------------------------------
//// Iterator support.
//// -----------------------------------------------------------------------------------------------

    public:
        constexpr ConstIteratorType Begin() const noexcept
        {
            return ConstIteratorType(_arr);
        }

        constexpr ConstIteratorType ConstBegin() const noexcept
        {
            return ConstIteratorType(_arr);
        }

        constexpr ConstIteratorType End() const noexcept
        {
            return ConstIteratorType(_arr + _count);
        }

        constexpr ConstIteratorType ConstEnd() const noexcept
        {
            return ConstIteratorType(_arr + _count);
        }

//// -----------------------------------------------------------------------------------------------
//// STD Iterator support.
//// -----------------------------------------------------------------------------------------------

    public:
        constexpr ConstIteratorType begin() const noexcept
        {
            return Begin();
        }

        constexpr ConstIteratorType cbegin() const noexcept
        {
            return ConstBegin();
        }

        constexpr ConstIteratorType end() const noexcept
        {
            return End();
        }

        constexpr ConstIteratorType cend() const noexcept
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
        const ElementType* _arr;
        const SizeT _count;
    };
}