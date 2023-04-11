#pragma once
#include <string_view>

#include "Atom/Containers.h"

#include "Atom/String/BasicChar.h"

namespace Atom
{
    template <typename TEncoding>
    using BasicStringView = ::std::basic_string_view<BasicChar<TEncoding>>;

    template <typename TEncoding>
    struct BasicStringViewWrapper
    {
        using TChar = BasicChar<TEncoding>;
        using TStringView = BasicStringView<TEncoding>;

        using TConstIterator = typename TStringView::const_iterator;
        using TIterator = typename TStringView::iterator;

    public:
        constexpr BasicStringViewWrapper(TStringView in) noexcept:
            str{ in } { }

    //// -------------------------------------------------------------------------------------------
    //// RConstIterable [BEGIN]
    //// -------------------------------------------------------------------------------------------

    public:
        constexpr TConstIterator ConstBegin() const noexcept
        {
            return str.cbegin();
        }

        constexpr TConstIterator ConstEnd() const noexcept
        {
            return str.cend();
        }

        constexpr TConstIterator Begin() const noexcept
        {
            return str.cbegin();
        }

        constexpr TConstIterator End() const noexcept
        {
            return str.cend();
        }

        constexpr TConstIterator cbegin() const noexcept
        {
            return str.cbegin();
        }

        constexpr TConstIterator cend() const noexcept
        {
            return str.cend();
        }

        constexpr TConstIterator begin() const noexcept
        {
            return str.cbegin();
        }

        constexpr TConstIterator end() const noexcept
        {
            return str.cend();
        }

    //// -------------------------------------------------------------------------------------------
    //// RConstIterable [END]
    //// -------------------------------------------------------------------------------------------

        TStringView str;
    };
}