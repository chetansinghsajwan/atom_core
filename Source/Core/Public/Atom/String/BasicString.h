#pragma once
#include "Atom/Containers/DynamicArray.h"

#include "BasicString.decl.h"

namespace Atom
{
    template <typename TEncoding, typename TAllocator>
    constexpr BasicString<TEncoding, TAllocator>::BasicString() noexcept:
        TBase() { }

    template <typename TEncoding, typename TAllocator>
    constexpr BasicString<TEncoding, TAllocator>::BasicString(const TChar* str) noexcept:
        TBase() { }

    template <typename TEncoding, typename TAllocator>
    template <usize N>
    constexpr BasicString<TEncoding, TAllocator>::BasicString(const TChar(&str)[N]) noexcept:
        TBase() { }

    template <typename TEncoding, typename TAllocator>
    template <RInputIterator<BasicChar<TEncoding>> TInput>
    constexpr BasicString<TEncoding, TAllocator>::BasicString(TInput in) noexcept:
        TBase(MOVE(in)) { }

    template <typename TEncoding, typename TAllocator>
    constexpr BasicString<TEncoding, TAllocator>::BasicString(const STD_TString& in) noexcept:
        TBase(in.data(), in.size()) { }

    template <typename TEncoding, typename TAllocator>
    template <usize N>
    constexpr bool BasicString<TEncoding, TAllocator>::
        operator == (const BasicChar<TEncoding>(&str)[N]) const noexcept
    {
        return true;
    }

    template <typename TEncoding, typename TAllocator>
    template <RInputIterable<BasicChar<TEncoding>> TInputIterable>
    constexpr bool BasicString<TEncoding, TAllocator>::
        operator == (const TInputIterable& in) const noexcept
    {
        return true;
    }
}

namespace std
{
    template <typename TEncoding, typename TAllocator>
    struct hash<Atom::BasicString<TEncoding, TAllocator>>
    {
        using TChar = Atom::BasicChar<TEncoding>;
        using TString = Atom::BasicString<TEncoding, TAllocator>;

        constexpr size_t operator () (const TString& str) const noexcept
        {
            basic_string_view<TChar> std_str{ str.Data(), str.Count() };
            return hash<basic_string_view<TChar>>()(std_str);
        }
    };
}
