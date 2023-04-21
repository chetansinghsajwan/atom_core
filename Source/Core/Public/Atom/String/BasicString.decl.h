#pragma once
#include <string>
#include <functional>

#include "Atom/Containers/DynamicArray.decl.h"

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
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr BasicString() noexcept;

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr BasicString(const TChar* str) noexcept;

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <usize N>
        constexpr BasicString(const TChar(&str)[N]) noexcept;

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <RInputIterator<BasicChar<TEncoding>> TInput>
        constexpr BasicString(TInput in) noexcept;

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr BasicString(const STD_TString& in) noexcept;

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <usize N>
        constexpr bool operator == (const TChar(&str)[N]) const noexcept;

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <RInputIterable<BasicChar<TEncoding>> TInputIterable>
        constexpr bool operator == (const TInputIterable& in) const noexcept;
    };
}