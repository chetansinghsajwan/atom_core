#pragma once
#include "BasicString.decl.h"
#include "Atom/Containers/DynamicArray.h"

namespace Atom
{
    template <typename TEncoding, typename TAllocator>
    constexpr BasicString<TEncoding, TAllocator>::BasicString() noexcept:
        TBase() { }

    template <typename TEncoding, typename TAllocator>
    constexpr BasicString<TEncoding, TAllocator>::BasicString(const TChar* str) noexcept:
        TBase() { }

    template <typename TEncoding, typename TAllocator>
    template <usize Count>
    constexpr BasicString<TEncoding, TAllocator>::BasicString(const TChar(&str)[Count]) noexcept:
        TBase(ArrayIterator<const TChar>(str, Count)) { }

    template <typename TEncoding, typename TAllocator>
    template <typename TInput>
    requires RFwdIter<TInput, const BasicChar<TEncoding>>
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
    template <typename TRange>
    requires RFwdRange<TRange, const BasicChar<TEncoding>>
    constexpr bool BasicString<TEncoding, TAllocator>::
        operator == (const TRange& in) const noexcept
    {
        return true;
    }

    template <typename TEncoding, typename TAllocator>
    constexpr BasicString<TEncoding, TAllocator>::
        ~BasicString() { }
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
