#pragma once
#include "atom/core/_std.h"
#include "atom/core/range/array_iter.h"
#include "atom/core/core.h"
#include "atom/core/core/requirements.h"
#include "atom/core/range/iter_requirements.h"
#include "atom/core/range/range_requirements.h"
#include "atom/core/range/std_iter_wrap_for_atom_iter.h"

namespace atom
{
    template <typename range_t>
    class _range_extensions_impl
    {
    protected:
        using _impl_t = range_t;

    public:
        using value_t = typename _impl_t::value_t;
        using iter_t = typename _impl_t::iter_t;
        using iter_end_t = typename _impl_t::iter_end_t;

    public:
        constexpr _range_extensions_impl(range_t& range)
            : _range_(range)
        {}

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_iter() const -> iter_t
        {
            return _range().get_iter();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_iter_end() const -> iter_end_t
        {
            return _range().get_iter_end();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename elem1_t>
        constexpr auto find_elem(const elem1_t& el) const -> iter_t
        {
            std_iter_wrap_for_atom_iter std_iter(get_iter());
            std_iter_wrap_for_atom_iter std_iter_end(get_iter_end());
            return std::find(std_iter, std_iter_end, el).iter;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename other_range_t>
        constexpr auto find_range(const other_range_t& range) const -> iter_t
        {
            std_iter_wrap_for_atom_iter std_iter(get_iter());
            std_iter_wrap_for_atom_iter std_iter_end(get_iter_end());
            std_iter_wrap_for_atom_iter std_iter1(range.get_iter());
            std_iter_wrap_for_atom_iter std_iter_end1(range.get_iter_end());
            return std::search(std_iter, std_iter_end, std_iter1, std_iter_end1).iter;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename other_range_t>
        auto compare(const other_range_t& range) const -> i8
        {
            std_iter_wrap_for_atom_iter std_iter(get_iter());
            std_iter_wrap_for_atom_iter std_iter_end(get_iter_end());
            std_iter_wrap_for_atom_iter std_iter1(range.get_iter());
            std_iter_wrap_for_atom_iter std_iter_end1(range.get_iter_end());

            return std::equal(std_iter, std_iter_end, std_iter1, std_iter_end1);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_count() const -> usize
        {
            if constexpr (is_jump_iter_pair<iter_t, iter_end_t>)
            {
                return get_iter_end() - get_iter();
            }

            usize count = 0;
            for (auto it = get_iter(); it != get_iter_end(); it++)
                count++;

            return count;
        }

    protected:
        constexpr auto _range() const -> const range_t&
        {
            return _range_;
        }

        constexpr auto _range() -> range_t&
        {
            return _range_;
        }

    private:
        range_t& _range_;
    };

    template <typename range_t, typename _range_extensions_impl_t = void>
    class range_extensions: public range_t
    {
        using this_t = range_extensions;
        using base_t = range_t;

    protected:
        using _impl_t = _range_extensions_impl_t;

    public:
        using value_t = typename _impl_t::value_t;
        using iter_t = typename _impl_t::iter_t;
        using iter_end_t = typename _impl_t::iter_end_t;

    public:
        using base_t::base_t;
        using base_t::operator=;

    public:
        ////////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// iteration
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_iter() const -> iter_t
        {
            return _impl().get_iter();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_iter_end() const -> iter_end_t
        {
            return _impl().get_iter_end();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto begin() const
        {
            return std_iter_wrap_for_atom_iter(_impl().get_iter());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto end() const
        {
            return std_iter_wrap_for_atom_iter(_impl().get_iter_end());
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// search
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename elem1_t>
        constexpr auto find(const elem1_t& el) const -> iter_t
            requires(is_equality_comparable_with<value_t, elem1_t>)
        {
            return _impl().find_elem(el);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename other_range_t>
        constexpr auto find_range(const other_range_t& range) const -> iter_t
            requires(is_fwd_range<other_range_t>)
                    and (is_equality_comparable_with<value_t, typename other_range_t::value_t>)
        {
            return _impl().find_range(range);
        }

        template <typename other_range_t>
        auto count_any(const other_range_t& range) const -> usize
        {
            usize count = 0;
            for (auto it = get_iter(); it.compare(get_iter_end()) != 0; it++)
                for (const auto& el : range)
                {
                    if (*it == el)
                        count++;
                }

            return count;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename elem1_t>
        constexpr auto contains(const elem1_t& el) const -> bool
            requires(is_equality_comparable_with<value_t, elem1_t>)
        {
            return _impl().find_elem(el).compare(_impl().get_iter_end()) != 0;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename other_range_t>
        constexpr auto contains(const other_range_t& range) const -> bool
            requires(is_fwd_range<other_range_t>)
                    and (is_equality_comparable_with<value_t, typename other_range_t::value_t>)
        {
            return _impl().find_range(range).compare(_impl().get_iter_end()) != 0;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// comparision
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename other_range_t>
        constexpr auto compare(const other_range_t& range) const -> i8
            requires(is_range<other_range_t>)
                    and (is_equality_comparable_with<value_t, typename other_range_t::value_t>)
        {
            return _impl().compare(range);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename other_range_t>
        constexpr auto is_eq(const other_range_t& range) const -> bool
            requires(is_range<other_range_t>)
                    and (is_equality_comparable_with<value_t, typename other_range_t::value_t>)
        {
            return _impl().compare(range) == 0;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// helpers
        ////
        //// # to do
        //// - add range helper functions.
        //// -------------------------------------------------------------------------------------------

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto can_get_count() const -> bool
        {
            return is_fwd_iter_pair<iter_t, iter_end_t>;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_count() const -> usize
        {
            return _impl().get_count();
        }

    protected:
        constexpr auto _impl() const -> const _impl_t
        {
            return _impl_t(const_cast<this_t&>(*this));
        }

        constexpr auto _impl() -> _impl_t
        {
            return _impl_t(*this);
        }
    };

    template <typename range_t>
    class range_extensions<range_t, void>
        : public range_extensions<range_t, _range_extensions_impl<range_t>>
    {
        using base_t = range_extensions<range_t, _range_extensions_impl<range_t>>;

    public:
        using base_t::base_t;
        using base_t::operator=;
    };
}
