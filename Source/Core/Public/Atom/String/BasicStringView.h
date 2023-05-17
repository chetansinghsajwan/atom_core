#pragma once
#include "BasicStringView.decl.h"
#include "Atom/Containers/ArrayView.h"
#include "BasicString.h"

namespace Atom
{
    template <typename TEncoding>
    constexpr BasicStringView<TEncoding>::BasicStringView() noexcept:
        TBase() { }

    template <typename TEncoding>
    constexpr BasicStringView<TEncoding>::BasicStringView(const TChar* str, usize len) noexcept:
        TBase(str, len) { }

    template <typename TEncoding>
    template <usize N>
    constexpr BasicStringView<TEncoding>::BasicStringView(const TChar(&str)[N]) noexcept:
        TBase(str, N) { }

    template <typename TEncoding>
    template <RFwdIter<BasicChar<TEncoding>> TInput>
    constexpr BasicStringView<TEncoding>::BasicStringView(TInput in) noexcept:
        TBase(MOVE(in)) { }

    template <typename TEncoding>
    template <typename TAllocator>
    constexpr BasicStringView<TEncoding>::BasicStringView(const BasicString<TEncoding, TAllocator>& in) noexcept:
        TBase(in.Data(), in.Count()) { }

    template <typename TEncoding>
    constexpr BasicStringView<TEncoding>::BasicStringView(const STD_TStringView& in) noexcept:
        TBase(in.data(), in.size()) { }
}