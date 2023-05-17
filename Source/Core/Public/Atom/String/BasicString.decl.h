#pragma once
#include <string>
#include "Atom/String/BasicChar.h"
#include "Atom/Containers/DynamicArray.decl.h"

namespace Atom
{
    template <typename TEncoding, typename TAllocator = DefaultAllocator<BasicChar<TEncoding>>>
    class BasicString: public DynamicArray<BasicChar<TEncoding>, TAllocator>
    {
        using TBase = DynamicArray<BasicChar<TEncoding>, TAllocator>;
        using _TString = ::std::basic_string<BasicChar<TEncoding>>;

    public:
        using TChar = BasicChar<TEncoding>;
        using TIter = typename TBase::TIter;
        using TEnd = typename TBase::TEnd;

    public:
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr BasicString() noexcept;

        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr BasicString(const TChar* str) noexcept;

        /// 
        /// ----------------------------------------------------------------------------------------
        template <usize count>
        constexpr BasicString(const TChar(&str)[count]) noexcept;

        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        requires RFwdRange<TRange, const BasicChar<TEncoding>>
        constexpr BasicString(const TRange& range) noexcept;

        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr BasicString(const _TString& in) noexcept;

        /// 
        /// ----------------------------------------------------------------------------------------
        template <usize count>
        constexpr bool operator == (const TChar(&str)[count]) const noexcept;

        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        requires RFwdRange<TRange, const BasicChar<TEncoding>>
        constexpr bool operator == (const TRange& in) const noexcept;

        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr ~BasicString();
    };
}