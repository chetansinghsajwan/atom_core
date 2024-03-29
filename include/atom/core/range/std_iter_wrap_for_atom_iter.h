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
    using std_iter_cat_for_atom_iter = typeutils::conditional_t<not is_iter<iter_t>, void,
        typeutils::conditional_t<not is_fwd_iter<iter_t>, std::input_iterator_tag,
            typeutils::conditional_t<not is_bidi_iter<iter_t>, std::forward_iterator_tag,
                typeutils::conditional_t<not is_jump_iter<iter_t>, std::bidirectional_iterator_tag,
                    typeutils::conditional_t<not is_array_iter<iter_t>,
                        std::random_access_iterator_tag, std::contiguous_iterator_tag>>>>>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iter_t>
    class std_iter_wrap_for_atom_iter
    {
        using this_t = std_iter_wrap_for_atom_iter;

    public:
        using value_type = typename iter_t::value_t;
        using size_type = usize;
        using difference_type = isize;
        using iterator_category = std_iter_cat_for_atom_iter<iter_t>;
        using pointer = value_type*;
        using reference = value_type&;

    public:
        constexpr std_iter_wrap_for_atom_iter(const iter_t& iter)
            requires is_fwd_iter<iter_t>
            : iter(iter)
        {}

        constexpr std_iter_wrap_for_atom_iter(iter_t&& iter)
            : iter(move(iter))
        {}

    public:
        constexpr auto operator*() const -> const value_type&
            requires(is_iter<iter_t>)
        {
            return iter.value();
        }

        constexpr auto operator*() -> value_type&
            requires(is_mut_iter<iter_t>)
        {
            return iter.mut_value();
        }

        template <class iter_end_t>
        constexpr auto operator==(const std_iter_wrap_for_atom_iter<iter_end_t>& that) const -> bool
            requires(is_iter_with_end<iter_t, iter_end_t>)
        {
            return iter.is_eq(that.iter);
        }

        template <class iter_end_t>
        constexpr auto operator!=(const std_iter_wrap_for_atom_iter<iter_end_t>& that) const -> bool
            requires(is_iter_with_end<iter_t, iter_end_t>)
        {
            return not iter.is_eq(that.iter);
        }

        constexpr auto operator++() -> this_t&
            requires(is_iter<iter_t>)
        {
            iter.next();
            return *this;
        }

        constexpr auto operator++(int) -> this_t
            requires(is_iter<iter_t>)
        {
            this_t tmp(iter);
            tmp.iter.next();
            return tmp;
        }

        constexpr auto operator--() -> this_t&
            requires(is_bidi_iter<iter_t>)
        {
            iter.prev();
            return *this;
        }

        constexpr auto operator--(int) const -> this_t
            requires(is_bidi_iter<iter_t>)
        {
            this_t tmp(iter);
            tmp.iter.prev();
            return tmp;
        }

        constexpr auto operator+(difference_type steps) -> this_t
            requires(is_jump_iter<iter_t>)
        {
            // todo: review this. should we accept steps as difference_type.
            ATOM_DEBUG_EXPECTS(steps >= 0);

            this_t tmp(iter);
            tmp.iter.next(steps);
            return tmp;
        }

        constexpr auto operator+=(difference_type steps) -> this_t
            requires(is_jump_iter<iter_t>)
        {
            ATOM_DEBUG_EXPECTS(steps >= 0);

            iter.next(steps);
            return *this;
        }

        constexpr auto operator-(difference_type steps) -> this_t
            requires(is_jump_iter<iter_t>)
        {
            ATOM_DEBUG_EXPECTS(steps >= 0);

            this_t tmp(iter);
            tmp.iter.prev(steps);
            return tmp;
        }

        constexpr auto operator-=(difference_type steps) -> this_t
            requires(is_jump_iter<iter_t>)
        {
            ATOM_DEBUG_EXPECTS(steps >= 0);

            iter.prev(steps);
            return *this;
        }

        constexpr auto operator-(const this_t& that) const -> difference_type
            requires(is_jump_iter<iter_t>)
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
        requires(not is_iter<iter_t>)
    class std_iter_wrap_for_atom_iter<iter_t>
    {
        using this_t = std_iter_wrap_for_atom_iter;

    public:
        constexpr std_iter_wrap_for_atom_iter(const iter_t& iter)
            requires is_fwd_iter<iter_t>
            : iter(iter)
        {}

        constexpr std_iter_wrap_for_atom_iter(iter_t&& iter)
            : iter(move(iter))
        {}

    public:
        iter_t iter;
    };
}
