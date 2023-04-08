#pragma once
#include <string>

#include "Atom/Containers.h"

#include "Atom/String/BasicChar.h"

namespace Atom
{
    template <typename T>
    using DefaultAllocator = ::std::allocator<T>;

    template <typename TEncoding, typename TAllocator = DefaultAllocator<BasicChar<TEncoding>>>
    using BasicString = ::std::basic_string
    <
        BasicChar<TEncoding>,
        ::std::char_traits<BasicChar<TEncoding>>,
        TAllocator
    >;

    template <typename TEncoding>
    struct BasicStringWrapper
    {
        using TChar = BasicChar<TEncoding>;
        using TString = BasicString<TEncoding>;

        constexpr BasicStringWrapper(TString& in) noexcept:
            str{ in } { }

        constexpr void InsertBack(TChar in) noexcept
        {
            str.push_back(in);
        }

        constexpr void InsertBack(const RConstIterable<TChar> auto& in) noexcept
        {
            str.insert(str.end(), in.Begin(), in.End());
        }

        constexpr void operator += (TChar in)
        {
            str.push_back(in);
        }

        constexpr void operator += (const RConstIterable<TChar> auto& in) noexcept
        {
            str.insert(str.end(), in.Begin(), in.End());
        }

        TString& str;
    };
}