#pragma once
// #include "array_iter.h"
#include "range_req.h"
#include "std_iter_wrap_for_atom_iter.h"

// #include <algorithm>

namespace atom
{
    template <typename range_type>
    class _range_extensions_impl
    {
    protected:
        using _timpl = range_type;

    public:
        using elem_type = typename _timpl::elem_type;
        using iter_type = typename _timpl::iter_type;
        using iter_end_type = typename _timpl::iter_end_type;

    public:
        constexpr _range_extensions_impl(range_type& range)
            : _range_(range)
        {}

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto iter() const -> iter_type
        {
            return _range().iter();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto iter_end() const -> iter_end_type
        {
            return _range().iter_end();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename telem1>
        constexpr auto find_elem(const telem1& el) const -> iter_type
        {
            std_iter_wrap_for_atom_iter std_iter{ iter() };
            std_iter_wrap_for_atom_iter std_iter_end{ iter_end() };
            return std::find(std_iter, std_iter_end, el).iter;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename trange1>
        constexpr auto find_range(const trange1& range) const -> iter_type
        {
            std_iter_wrap_for_atom_iter std_iter{ iter() };
            std_iter_wrap_for_atom_iter std_iter_end{ iter_end() };
            std_iter_wrap_for_atom_iter std_iter1{ range.iter() };
            std_iter_wrap_for_atom_iter std_iter_end1{ range.iter_end() };
            return std::find(std_iter, std_iter_end, std_iter1, std_iter_end1);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename trange1>
        auto compare(const trange1& range) const -> i8
        {
            std_iter_wrap_for_atom_iter std_iter{ iter() };
            std_iter_wrap_for_atom_iter std_iter_end{ iter_end() };
            std_iter_wrap_for_atom_iter std_iter1{ range.iter() };
            std_iter_wrap_for_atom_iter std_iter_end1{ range.iter_end() };

            return std::equal(std_iter, std_iter_end, std_iter1, std_iter_end1);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_count() const -> usize
        {
            if constexpr (rjump_iter_pair<iter_type, iter_end_type>)
            {
                return iter_end() - iter();
            }

            usize count = 0;
            for (auto it = iter(); it != iter_end(); it++)
                count++;

            return count;
        }

    protected:
        constexpr auto _range() const -> const range_type&
        {
            return _range_;
        }

        constexpr auto _range() -> range_type&
        {
            return _range_;
        }

    private:
        range_type& _range_;
    };

    template <typename range_type, typename _trange_extensions_impl = void>
    class range_extensions: public range_type
    {
        using this_type = range_extensions;
        using base_type = range_type;

    protected:
        using _timpl = _trange_extensions_impl;

    public:
        using elem_type = typename _timpl::elem_type;
        using iter_type = typename _timpl::iter_type;
        using iter_end_type = typename _timpl::iter_end_type;

    public:
        using base_type::base_type;
        using base_type::operator=;

    public:
        ////////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// iteration
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto iter() const -> iter_type
        {
            return _impl().iter();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto iter_end() const -> iter_end_type
        {
            return _impl().iter_end();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto begin() const
        {
            return std_iter_wrap_for_atom_iter{ _impl().iter() };
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto end() const
        {
            return std_iter_wrap_for_atom_iter{ _impl().iter_end() };
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// search
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename telem1>
        constexpr auto find(const telem1& el) const -> iter_type
            requires(requality_comparable_with<elem_type, telem1>)
        {
            return _impl().find_elem(el);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename trange1>
        constexpr auto find_range(const trange1& range) const -> iter_type
            requires(rfwd_range<trange1>)
                    and (requality_comparable_with<elem_type, typename trange1::elem_type>)
        {
            return _impl().find_range(range);
        }

        template <typename trange1>
        auto count_any(const trange1& range) const -> usize
        {
            usize count = 0;
            for (auto it = iter(); it.compare(iter_end()) != 0; it++)
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
        template <typename telem1>
        constexpr auto contains(const telem1& el) const -> bool
            requires(requality_comparable_with<elem_type, telem1>)
        {
            return _impl().find_elem(el).compare(_impl().iter_end()) != 0;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename trange1>
        constexpr auto contains(const trange1& range) const -> bool
            requires(rfwd_range<trange1>)
                    and (requality_comparable_with<elem_type, typename trange1::elem_type>)
        {
            return _impl().find_range(range).compare(_impl().iter_end()) != 0;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// comparision
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename trange1>
        constexpr auto compare(const trange1& range) const -> i8
            requires(rrange<trange1>) and (requality_comparable_with<elem_type, typename trange1::elem_type>)
        {
            return _impl().compare(range);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename trange1>
        constexpr auto eq(const trange1& range) const -> bool
            requires(rrange<trange1>) and (requality_comparable_with<elem_type, typename trange1::elem_type>)
        {
            return _impl().compare(range) == 0;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename trange1>
        constexpr auto ne(const trange1& range) const -> bool
            requires(rrange<trange1>) and (requality_comparable_with<elem_type, typename trange1::elem_type>)
        {
            return not eq(range);
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
            return rfwd_iter_pair<iter_type, iter_end_type>;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_count() const -> usize
        {
            return _impl().get_count();
        }

    protected:
        constexpr auto _impl() const -> const _timpl
        {
            return _timpl(const_cast<this_type&>(*this));
        }

        constexpr auto _impl() -> _timpl
        {
            return _timpl(*this);
        }
    };

    template <typename range_type>
    class range_extensions<range_type, void>
        : public range_extensions<range_type, _range_extensions_impl<range_type>>
    {
        using base_type = range_extensions<range_type, _range_extensions_impl<range_type>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };
}
