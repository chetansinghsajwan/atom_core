#pragma once
#include "atom/core/_std.h"
#include "atom/core/core.h"
#include "atom/core/typeinfo.h"
#include "atom/core/contracts.h"
#include "atom/core/range/iter_requirements.h"
#include "atom/core/range/range_requirements.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iter_t>
    using std_iter_cat_for_atom_iter = typeinfo::conditional_t<not riter<iter_t>, void,
        typeinfo::conditional_t<not rfwd_iter<iter_t>, std::input_iterator_tag,
            typeinfo::conditional_t<not rbidi_iter<iter_t>, std::forward_iterator_tag,
                typeinfo::conditional_t<not rjump_iter<iter_t>, std::bidirectional_iterator_tag,
                    typeinfo::conditional_t<not rarray_iter<iter_t>,
                        std::random_access_iterator_tag, std::contiguous_iterator_tag>>>>>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iter_t>
    class std_iter_wrap_for_atom_iter
    {
        using this_t = std_iter_wrap_for_atom_iter;

    public:
        using value_t = typename iter_t::elem_t;
        using size_t = usize;
        using difference_t = isize;
        using iterator_category = std_iter_cat_for_atom_iter<iter_t>;
        using pointer = value_t*;
        using reference = value_t&;

    public:
        constexpr std_iter_wrap_for_atom_iter(const iter_t& iter)
            requires rfwd_iter<iter_t>
            : iter(iter)
        {}

        constexpr std_iter_wrap_for_atom_iter(iter_t&& iter)
            : iter(move(iter))
        {}

    public:
        constexpr auto operator*() const -> const value_t&
            requires(riter<iter_t>)
        {
            return iter.value();
        }

        constexpr auto operator*() -> value_t&
            requires(rmut_iter<iter_t>)
        {
            return iter.mut_value();
        }

        template <class iter_end_t>
        constexpr auto operator==(const std_iter_wrap_for_atom_iter<iter_end_t>& that) const
            -> bool
            requires(riter_with_end<iter_t, iter_end_t>)
        {
            return iter.is_eq(that.iter);
        }

        template <class iter_end_t>
        constexpr auto operator!=(const std_iter_wrap_for_atom_iter<iter_end_t>& that) const
            -> bool
            requires(riter_with_end<iter_t, iter_end_t>)
        {
            return not iter.is_eq(that.iter);
        }

        constexpr auto operator++() -> this_t&
            requires(riter<iter_t>)
        {
            iter.next();
            return *this;
        }

        constexpr auto operator++(int) -> this_t
            requires(riter<iter_t>)
        {
            this_t tmp(iter);
            tmp.iter.next();
            return tmp;
        }

        constexpr auto operator--() -> this_t&
            requires(rbidi_iter<iter_t>)
        {
            iter.prev();
            return *this;
        }

        constexpr auto operator--(int) const -> this_t
            requires(rbidi_iter<iter_t>)
        {
            this_t tmp(iter);
            tmp.iter.prev();
            return tmp;
        }

        constexpr auto operator+(difference_t steps) -> this_t
            requires(rjump_iter<iter_t>)
        {
            // todo: review this. should we accept steps as difference_t.
            ATOM_DEBUG_EXPECTS(steps > 0);

            this_t tmp(iter);
            tmp.iter.next(steps);
            return tmp;
        }

        constexpr auto operator+=(difference_t steps) -> this_t
            requires(rjump_iter<iter_t>)
        {
            ATOM_DEBUG_EXPECTS(steps > 0);

            iter.next(steps);
            return *this;
        }

        constexpr auto operator-(difference_t steps) -> this_t
            requires(rjump_iter<iter_t>)
        {
            ATOM_DEBUG_EXPECTS(steps > 0);

            this_t tmp(iter);
            tmp.iter.prev(steps);
            return tmp;
        }

        constexpr auto operator-=(difference_t steps) -> this_t
            requires(rjump_iter<iter_t>)
        {
            ATOM_DEBUG_EXPECTS(steps > 0);

            iter.prev(steps);
            return *this;
        }

        constexpr auto operator-(const this_t& that) const -> difference_t
            requires(rjump_iter<iter_t>)
        {
            return iter.compare(that.iter);
        }

    public:
        iter_t iter;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iter_t>
        requires(not riter<iter_t>)
    class std_iter_wrap_for_atom_iter<iter_t>
    {
        using this_t = std_iter_wrap_for_atom_iter;

    public:
        constexpr std_iter_wrap_for_atom_iter(const iter_t& iter)
            requires rfwd_iter<iter_t>
            : iter(iter)
        {}

        constexpr std_iter_wrap_for_atom_iter(iter_t&& iter)
            : iter(move(iter))
        {}

    public:
        iter_t iter;
    };
}
