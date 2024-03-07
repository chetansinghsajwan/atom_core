#pragma once
#include "atom/core/_std.h"
#include "atom/core/core.h"
#include "atom/core/tti.h"
#include "atom/core/contracts.h"
#include "atom/core/range/iter_requirements.h"
#include "atom/core/range/range_requirements.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iter_type>
    using std_iter_cat_for_atom_iter = tti::conditional_type<not riter<iter_type>, void,
        tti::conditional_type<not rfwd_iter<iter_type>, std::input_iterator_tag,
            tti::conditional_type<not rbidi_iter<iter_type>, std::forward_iterator_tag,
                tti::conditional_type<not rjump_iter<iter_type>, std::bidirectional_iterator_tag,
                    tti::conditional_type<not rarray_iter<iter_type>,
                        std::random_access_iterator_tag, std::contiguous_iterator_tag>>>>>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iter_type>
    class std_iter_wrap_for_atom_iter
    {
        using this_type = std_iter_wrap_for_atom_iter;

    public:
        using value_type = typename iter_type::elem_type;
        using size_type = usize;
        using difference_type = isize;
        using iterator_category = std_iter_cat_for_atom_iter<iter_type>;
        using pointer = value_type*;
        using reference = value_type&;

    public:
        constexpr std_iter_wrap_for_atom_iter(const iter_type& iter)
            requires rfwd_iter<iter_type>
            : iter(iter)
        {}

        constexpr std_iter_wrap_for_atom_iter(iter_type&& iter)
            : iter(move(iter))
        {}

    public:
        constexpr auto operator*() const -> const value_type&
            requires(riter<iter_type>)
        {
            return iter.value();
        }

        constexpr auto operator*() -> value_type&
            requires(rmut_iter<iter_type>)
        {
            return iter.mut_value();
        }

        template <class iter_end_type>
        constexpr auto operator==(const std_iter_wrap_for_atom_iter<iter_end_type>& that) const
            -> bool
            requires(riter_with_end<iter_type, iter_end_type>)
        {
            return iter.is_eq(that.iter);
        }

        template <class iter_end_type>
        constexpr auto operator!=(const std_iter_wrap_for_atom_iter<iter_end_type>& that) const
            -> bool
            requires(riter_with_end<iter_type, iter_end_type>)
        {
            return not iter.is_eq(that.iter);
        }

        constexpr auto operator++() -> this_type&
            requires(riter<iter_type>)
        {
            iter.next();
            return *this;
        }

        constexpr auto operator++(int) -> this_type
            requires(riter<iter_type>)
        {
            this_type tmp(iter);
            tmp.iter.next();
            return tmp;
        }

        constexpr auto operator--() -> this_type&
            requires(rbidi_iter<iter_type>)
        {
            iter.prev();
            return *this;
        }

        constexpr auto operator--(int) const -> this_type
            requires(rbidi_iter<iter_type>)
        {
            this_type tmp(iter);
            tmp.iter.prev();
            return tmp;
        }

        constexpr auto operator+(difference_type steps) -> this_type
            requires(rjump_iter<iter_type>)
        {
            // todo: review this. should we accept steps as difference_type.
            ATOM_DEBUG_EXPECTS(steps > 0);

            this_type tmp(iter);
            tmp.iter.next(steps);
            return tmp;
        }

        constexpr auto operator+=(difference_type steps) -> this_type
            requires(rjump_iter<iter_type>)
        {
            ATOM_DEBUG_EXPECTS(steps > 0);

            iter.next(steps);
            return *this;
        }

        constexpr auto operator-(difference_type steps) -> this_type
            requires(rjump_iter<iter_type>)
        {
            ATOM_DEBUG_EXPECTS(steps > 0);

            this_type tmp(iter);
            tmp.iter.prev(steps);
            return tmp;
        }

        constexpr auto operator-=(difference_type steps) -> this_type
            requires(rjump_iter<iter_type>)
        {
            ATOM_DEBUG_EXPECTS(steps > 0);

            iter.prev(steps);
            return *this;
        }

        constexpr auto operator-(const this_type& that) const -> difference_type
            requires(rjump_iter<iter_type>)
        {
            return iter.compare(that.iter);
        }

    public:
        iter_type iter;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iter_type>
        requires(not riter<iter_type>)
    class std_iter_wrap_for_atom_iter<iter_type>
    {
        using this_type = std_iter_wrap_for_atom_iter;

    public:
        constexpr std_iter_wrap_for_atom_iter(const iter_type& iter)
            requires rfwd_iter<iter_type>
            : iter(iter)
        {}

        constexpr std_iter_wrap_for_atom_iter(iter_type&& iter)
            : iter(move(iter))
        {}

    public:
        iter_type iter;
    };
}
