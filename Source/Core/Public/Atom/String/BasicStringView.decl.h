#pragma once
#include <string_view>

#include "Atom/Containers/ArrayView.decl.h"
#include "Atom/String/BasicChar.h"
#include "Atom/String/BasicString.decl.h"

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
        constexpr BasicStringView() noexcept;

        constexpr BasicStringView(const TChar* str, usize len) noexcept;

        template <usize N>
        constexpr BasicStringView(const TChar(&str)[N]) noexcept;

        template <RFwdIter<BasicChar<TEncoding>> TInput>
        constexpr BasicStringView(TInput in) noexcept;

        template <typename TAllocator>
        constexpr BasicStringView(const BasicString<TEncoding, TAllocator>& in) noexcept;

        constexpr BasicStringView(const STD_TStringView& in) noexcept;
    };
}