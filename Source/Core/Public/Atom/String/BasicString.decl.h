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
        template <usize count>
        constexpr BasicString(const TChar(&str)[count]) noexcept;

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TInput>
        requires RFwdIter<TInput, const BasicChar<TEncoding>>
        constexpr BasicString(TInput in) noexcept;

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr BasicString(const STD_TString& in) noexcept;

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <usize count>
        constexpr bool operator == (const TChar(&str)[count]) const noexcept;

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        requires RFwdRange<TRange, const BasicChar<TEncoding>>
        constexpr bool operator == (const TRange& in) const noexcept;

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr ~BasicString();
    };
}