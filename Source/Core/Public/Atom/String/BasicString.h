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

        using TConstIterator = typename TString::const_iterator;
        using TIterator = typename TString::iterator;

    public:
        constexpr BasicStringWrapper(TString in) noexcept:
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

        TString str;
    };
}