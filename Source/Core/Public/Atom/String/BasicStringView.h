#pragma once
#include <string_view>

#include "Atom/Containers/ArrayView.h"

#include "Atom/String/BasicChar.h"

namespace Atom
{
    template <typename TEncoding>
    class BasicStringView: public ArrayView<BasicChar<TEncoding>>
    {
        using TBase = ArrayView<BasicChar<TEncoding>>;
        using STD_TStringView = ::std::basic_string_view<BasicChar<TEncoding>>;

    public:
        using TChar = BasicChar<TEncoding>;
        using TIterator = typename TBase::TIterator;

    public:
        constexpr BasicStringView() noexcept:
            TBase() { }

        constexpr BasicStringView(const TChar* str, usize len) noexcept:
            TBase(str, len) { }

        template <usize N>
        constexpr BasicStringView(const TChar(&str)[N]) noexcept:
            TBase(str, N) { }

        template <RInputIterator<TChar> TInput>
        constexpr BasicStringView(TInput in) noexcept:
            TBase(MOVE(in)) { }

        constexpr BasicStringView(const BasicString<TEncoding>& in) noexcept:
            TBase(in.Data(), in.Count()) { }

        constexpr BasicStringView(const STD_TStringView& in) noexcept:
            TBase(in.data(), in.size()) { }
    };
}