#pragma once
#include "RangeReq.h"
#include "Atom/TTI.h"

#define ATOM_NOEX_IF(...)

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
        constexpr StdIterWrapForAtomIter(const TIter& iter) ATOM_NOEX_IF(_iter{ iter }):
            _iter{ iter } { }

        constexpr StdIterWrapForAtomIter(TIter& iter) ATOM_NOEX_IF(_iter{ iter }):
            _iter{ iter } { }

        constexpr StdIterWrapForAtomIter(TIter&& iter) ATOM_NOEX_IF(_iter{ MOVE(iter) }):
            _iter{ MOVE(iter) } { }

    public:
        template <class = void>
        requires RIter<TIter>
        constexpr const value_type& operator *() const ATOM_NOEX_IF(*_iter)
        {
            return *_iter;
        }

        template <class = void>
        requires RMutIter<TIter>
        constexpr value_type& operator *() ATOM_NOEX_IF(*_iter)
        {
            return *_iter;
        }

        template <class TIterEnd>
        requires RIterEnd<TIter, TIterEnd>
        constexpr bool operator == (const StdIterWrapForAtomIter<TIterEnd>& that) const
            ATOM_NOEX_IF(_iter == that._iter)
        {
            return _iter == that._iter;
        }

        template <class = void>
        requires RIter<TIter>
        constexpr This& operator ++() ATOM_NOEX_IF(++_iter)
        {
            ++_iter;
            return *this;
        }

        template <class = void>
        requires RIter<TIter>
        constexpr This operator ++(int) ATOM_NOEX_IF(_iter++)
        {
            return This{ _iter++ };
        }

        template <class = void>
        requires RBidiIter<TIter>
        constexpr This& operator --() ATOM_NOEX_IF(--_iter)
        {
            --_iter;
            return *this;
        }

        template <class = void>
        requires RBidiIter<TIter>
        constexpr This operator --(int) const ATOM_NOEX_IF(_iter--)
        {
            return This{ _iter-- };
        }

        template <class = void>
        requires RJumpIter<TIter>
        constexpr This operator +(difference_type steps) ATOM_NOEX_IF(_iter + steps)
        {
            return This{ _iter + steps };
        }

        template <class = void>
        requires RJumpIter<TIter>
        constexpr This operator -(difference_type steps) ATOM_NOEX_IF(_iter - steps)
        {
            return This{ _iter - steps };
        }

        template <class = void>
        requires RJumpIter<TIter>
        constexpr difference_type operator -(const This& that) ATOM_NOEX_IF(_iter - that._iter)
        {
            return _iter - that._iter;
        }

    private:
        TIter _iter;
    };
}