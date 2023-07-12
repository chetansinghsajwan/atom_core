#pragma once
#include "RangeReq.h"
#include "Atom/TTI.h"

#define old_noex_if(...)

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TIter>
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
    template <tname TIter>
    class StdIterWrapForAtomIter
    {
        using This = StdIterWrapForAtomIter<TIter>;

    public:
        using value_type = tname TIter::TElem;
        using size_type = usize;
        using difference_type = isize;
        using iterator_category = StdIterCatForAtomIter<TIter>;
        using pointer = value_type*;
        using reference = value_type&;

    public:
        cexpr ctor StdIterWrapForAtomIter(const TIter& iter) noex:
            _iter{ iter } { }

        cexpr ctor StdIterWrapForAtomIter(TIter& iter) noex:
            _iter{ iter } { }

        cexpr ctor StdIterWrapForAtomIter(TIter&& iter) noex:
            _iter{ MOVE(iter) } { }

    public:
        template <class = void>
        requires RIter<TIter>
        cexpr fn operator *() const noex_if(*_iter) -> const value_type&
        {
            return *_iter;
        }

        template <class = void>
        requires RMutIter<TIter>
        cexpr fn operator *() noex_if(*_iter) -> value_type&
        {
            return *_iter;
        }

        template <class TIterEnd>
        requires RIterEnd<TIter, TIterEnd>
        cexpr fn operator == (const StdIterWrapForAtomIter<TIterEnd>& that)
            const noex_if(_iter == that._iter) -> bool
        {
            return _iter == that._iter;
        }

        template <class TIterEnd>
        requires RIterEnd<TIter, TIterEnd>
        cexpr fn operator != (const StdIterWrapForAtomIter<TIterEnd>& that)
            const noex_if(_iter != that._iter) -> bool
        {
            return _iter != that._iter;
        }

        template <class = void>
        requires RIter<TIter>
        cexpr fn operator ++() noex_if(++_iter) -> This&
        {
            ++_iter;
            return *this;
        }

        template <class = void>
        requires RIter<TIter>
        cexpr fn operator ++(int) noex_if(_iter++) -> This
        {
            return This{ _iter++ };
        }

        template <class = void>
        requires RBidiIter<TIter>
        cexpr fn operator --() noex_if(--_iter) -> This&
        {
            --_iter;
            return *this;
        }

        template <class = void>
        requires RBidiIter<TIter>
        cexpr fn operator --(int) const noex_if(_iter--) -> This
        {
            return This{ _iter-- };
        }

        template <class = void>
        requires RJumpIter<TIter>
        cexpr fn operator +(difference_type steps) noex_if(_iter + steps) -> This
        {
            return This{ _iter + steps };
        }

        template <class = void>
        requires RJumpIter<TIter>
        cexpr fn operator -(difference_type steps) noex_if(_iter - steps) -> This
        {
            return This{ _iter - steps };
        }

        template <class = void>
        requires RJumpIter<TIter>
        cexpr fn operator -(const This& that) noex_if(_iter - that._iter) -> difference_type
        {
            return _iter - that._iter;
        }

    private:
        TIter _iter;
    };
}