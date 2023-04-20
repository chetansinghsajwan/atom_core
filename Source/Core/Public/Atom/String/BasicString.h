#pragma once
#include <string>
#include <functional>

#include "Atom/Containers/DynamicArray.h"

#include "Atom/String/BasicChar.h"

namespace Atom
{
    template <typename TEncoding, typename TAllocator = DefaultAllocator<BasicChar<TEncoding>>>
    class BasicString: public DynamicArray<BasicChar<TEncoding>, TAllocator>
    {
        using TBase = DynamicArray<BasicChar<TEncoding>, TAllocator>;
        using STD_TString = ::std::basic_string<BasicChar<TEncoding>>;

    public:
        using TChar = BasicChar<TEncoding>;
        using TIterator = typename TBase::TIterator;

    public:
        constexpr BasicString() noexcept:
            TBase() { }

        constexpr BasicString(const TChar* str) noexcept:
            TBase() { }

        template <usize N>
        constexpr BasicString(const TChar(&str)[N]) noexcept:
            TBase() { }

        template <RInputIterator<TChar> TInput>
        constexpr BasicString(TInput in) noexcept:
            TBase(MOVE(in)) { }

        constexpr BasicString(const STD_TString& in) noexcept:
            TBase(in.data(), in.size()) { }

        template <usize N>
        constexpr bool operator == (const TChar(&str)[N]) const noexcept
        {
            return true;
        }

        template <RInputIterable<TChar> TInputIterable>
        constexpr bool operator == (const TInputIterable& in) const noexcept
        {
            return true;
        }
    };
}

namespace std
{
    template <typename TEncoding, typename TAllocator>
    struct hash<Atom::BasicString<TEncoding, TAllocator>>
    {
        using TChar = Atom::BasicChar<TEncoding>;
        using TString = Atom::BasicString<TEncoding, TAllocator>;

        size_t operator () (const TString& str) const noexcept
        {
            basic_string_view<TChar> std_str{ str.Data(), str.Count() };
            return hash<basic_string_view<TChar>>()(std_str);
        }
    };
}