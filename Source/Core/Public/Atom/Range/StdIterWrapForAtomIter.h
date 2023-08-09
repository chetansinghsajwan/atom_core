#pragma once
#include "RangeReq.h"
#include "Atom/TTI.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    using StdIterCatForAtomIter = TTI::TEnableIf
    <
        RIter<TIter>,
        TTI::TNotConditional
        <
            RFwdIter<TIter>, std::input_iterator_tag,
            TTI::TNotConditional
            <
                RBidiIter<TIter>, std::forward_iterator_tag,
                TTI::TNotConditional
                <
                    RJumpIter<TIter>, std::bidirectional_iterator_tag,
                    TTI::TNotConditional
                    <
                        RArrIter<TIter>, std::random_access_iterator_tag,
                        std::contiguous_iterator_tag
                    >
                >
            >
        >
    >;

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    class StdIterWrapForAtomIter
    {
        using Self = StdIterWrapForAtomIter<TIter>;

    public:
        using value_type = typename TIter::TElem;
        using size_type = usize;
        using difference_type = isize;
        using iterator_category = StdIterCatForAtomIter<TIter>;
        using pointer = value_type*;
        using reference = value_type&;

    public:
        cexpr ctor StdIterWrapForAtomIter(const TIter& iter):
            iter{ iter } { }

        cexpr ctor StdIterWrapForAtomIter(TIter& iter):
            iter{ iter } { }

        cexpr ctor StdIterWrapForAtomIter(TIter&& iter):
            iter{ mov(iter) } { }

    public:
        template <class = void>
        requires RIter<TIter>
        cexpr fn op*() const -> const value_type&
        {
            return *iter;
        }

        template <class = void>
        requires RMutIter<TIter>
        cexpr fn op*() -> value_type&
        {
            return *iter;
        }

        template <class TIterEnd>
        requires RIterEnd<TIter, TIterEnd>
        cexpr fn op== (const StdIterWrapForAtomIter<TIterEnd>& that) const -> bool
        {
            return iter == that.iter;
        }

        template <class TIterEnd>
        requires RIterEnd<TIter, TIterEnd>
        cexpr fn op!= (const StdIterWrapForAtomIter<TIterEnd>& that) const -> bool
        {
            return iter != that.iter;
        }

        template <class = void>
        requires RIter<TIter>
        cexpr fn op++() -> Self&
        {
            ++iter;
            return self;
        }

        template <class = void>
        requires RIter<TIter>
        cexpr fn op++(i32) -> Self
        {
            return Self{ iter++ };
        }

        template <class = void>
        requires RBidiIter<TIter>
        cexpr fn op--() -> Self&
        {
            --iter;
            return self;
        }

        template <class = void>
        requires RBidiIter<TIter>
        cexpr fn op--(i32) const -> Self
        {
            return Self{ iter-- };
        }

        template <class = void>
        requires RJumpIter<TIter>
        cexpr fn op+(difference_type steps) -> Self
        {
            return Self{ iter + steps };
        }

        template <class = void>
        requires RJumpIter<TIter>
        cexpr fn op-(difference_type steps) -> Self
        {
            return Self{ iter - steps };
        }

        template <class = void>
        requires RJumpIter<TIter>
        cexpr fn op-(const Self& that) -> difference_type
        {
            return iter - that.iter;
        }

    public:
        TIter iter;
    };
}