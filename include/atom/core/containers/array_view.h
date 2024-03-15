#pragma once
#include "atom/core/core.h"
#include "atom/core/range/array_iter.h"
#include "atom/core/range.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename in_elem_t>
    class basic_array_view
    {
        using this_t = basic_array_view;

    public:
        using elem_t = in_elem_t;
        using iter_t = array_iter<elem_t>;
        using iter_end_t = iter_t;
        using mut_iter_t = mut_array_iter<elem_t>;
        using mut_iter_end_t = mut_iter_t;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr basic_array_view()
            : _data(nullptr)
            , _count(0)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # trivial copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr basic_array_view(const this_t& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # trivial copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr basic_array_view& operator=(const this_t& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # trivial move constructor
        /// ----------------------------------------------------------------------------------------
        constexpr basic_array_view(this_t&& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # trivial move operator
        /// ----------------------------------------------------------------------------------------
        constexpr basic_array_view& operator=(this_t&& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # array constructor
        /// ----------------------------------------------------------------------------------------
        template <usize count>
        constexpr basic_array_view(const elem_t (&arr)[count])
            : _data(arr)
            , _count(count)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # array operator
        /// ----------------------------------------------------------------------------------------
        template <usize count>
        constexpr basic_array_view& operator=(const elem_t (&arr)[count])
        {
            _data = arr;
            _count = count;
        }

        /// ----------------------------------------------------------------------------------------
        /// # range constructor
        /// ----------------------------------------------------------------------------------------
        template <typename range_t>
        constexpr basic_array_view(const range_t& range)
            requires(is_array_range_of<range_t, elem_t>)
            : _data(range.get_data())
            , _count(range.get_count())
        {}

        /// ----------------------------------------------------------------------------------------
        /// # range operator
        /// ----------------------------------------------------------------------------------------
        template <typename range_t>
        constexpr basic_array_view& operator=(const range_t& range)
            requires(is_array_range_of<range_t, elem_t>)
        {
            _data = range.get_data();
            _count = range.get_count();
        }

        /// ----------------------------------------------------------------------------------------
        /// # trivial destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~basic_array_view() = default;

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_data() const -> const elem_t*
        {
            return _data;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_count() const -> usize
        {
            return _count;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_iter() const -> iter_t
        {
            return iter_t(_data);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_iter_end() const -> iter_end_t
        {
            return iter_end_t(_data + _count);
        }

    private:
        const elem_t* _data;
        usize _count;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename elem_t>
    class array_view: public array_range_extensions<basic_array_view<elem_t>>
    {
        using base_t = array_range_extensions<basic_array_view<elem_t>>;

    public:
        using base_t::base_t;
        using base_t::operator=;
    };
}
