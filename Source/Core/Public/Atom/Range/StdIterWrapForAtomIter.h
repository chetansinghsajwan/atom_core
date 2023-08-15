#pragma once
#include "Atom/TTI.h"
#include "RangeReq.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    using StdIterCatForAtomIter = TTI::TConditional<not RIter<TIter>, void,
        TTI::TConditional<not RFwdIter<TIter>, std::input_iterator_tag,
            TTI::TConditional<not RBidiIter<TIter>, std::forward_iterator_tag,
                TTI::TConditional<not RJumpIter<TIter>, std::bidirectional_iterator_tag,
                    TTI::TConditional<not RArrIter<TIter>, std::random_access_iterator_tag,
                        std::contiguous_iterator_tag>>>>>;

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
        constexpr StdIterWrapForAtomIter(TIter& iter)
            : iter{ iter } {}

        constexpr StdIterWrapForAtomIter(TIter&& iter)
            : iter{ mov(iter) } {}

    public:
        constexpr auto operator*() const -> const value_type&
            requires(RIter<TIter>)
        {
            return iter.value();
        }

        constexpr auto operator*() -> value_type&
            requires(RMutIter<TIter>)
        {
            return iter.mutValue();
        }

        template <class TIterEnd>
        constexpr auto operator==(const StdIterWrapForAtomIter<TIterEnd>& that) const -> bool
            requires(RIterWithEnd<TIter, TIterEnd>)
        {
            return iter.equals(that.iter);
        }

        template <class TIterEnd>
        constexpr auto operator!=(const StdIterWrapForAtomIter<TIterEnd>& that) const -> bool
            requires(RIterWithEnd<TIter, TIterEnd>)
        {
            return not iter.equals(that.iter);
        }

        constexpr auto operator++() -> Self&
            requires(RIter<TIter>)
        {
            iter.next();
            return *this;
        }

        constexpr auto operator++(i32) -> Self
            requires(RIter<TIter>)
        {
            Self tmp{ iter };
            tmp.iter.next();
            return tmp;
        }

        constexpr auto operator--() -> Self&
            requires(RBidiIter<TIter>)
        {
            iter.prev();
            return *this;
        }

        constexpr auto operator--(i32) const -> Self
            requires(RBidiIter<TIter>)
        {
            Self tmp{ iter };
            tmp.iter.prev();
            return tmp;
        }

        constexpr auto operator+(difference_type steps) -> Self
            requires(RJumpIter<TIter>)
        {
            // TODO: Review this. Should we accept steps as difference_type.
            debug_expects(steps > 0);

            Self tmp{ iter };
            tmp.iter.next(steps);
            return tmp;
        }

        constexpr auto operator-(difference_type steps) -> Self
            requires(RJumpIter<TIter>)
        {
            debug_expects(steps > 0);

            Self tmp{ iter };
            tmp.iter.prev(steps);
            return tmp;
        }

        constexpr auto operator-(const Self& that) -> difference_type
            requires(RJumpIter<TIter>)
        {
            return iter.compare(that.iter);
        }

    public:
        TIter iter;
    };
}
