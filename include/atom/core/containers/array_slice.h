#pragma once
#include "atom/core/range/array_iter.h"
#include "atom/core/range.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename in_elem_t>
    class array_slice: public range_extensions
    {
        static_assert(typeinfo<in_elem_t>::is_pure);

    private:
        using this_t = array_slice;

    public:
        using value_t = in_elem_t;
        using iter_t = array_iter<value_t>;
        using iter_end_t = iter_t;
        using mut_iter_t = mut_array_iter<value_t>;
        using mut_iter_end_t = mut_iter_t;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr array_slice()
            : _data(nullptr)
            , _count(0)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # trivial copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr array_slice(const this_t& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # trivial copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr array_slice& operator=(const this_t& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # trivial move constructor
        /// ----------------------------------------------------------------------------------------
        constexpr array_slice(this_t&& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # trivial move operator
        /// ----------------------------------------------------------------------------------------
        constexpr array_slice& operator=(this_t&& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # range constructor
        /// ----------------------------------------------------------------------------------------
        template <typename range_t>
        constexpr array_slice(range_t& range)
            requires is_mut_array_range_of<range_t, value_t>
            : _data(range.get_mut_data())
            , _count(range.get_count())
        {}

        /// ----------------------------------------------------------------------------------------
        /// # range operator
        /// ----------------------------------------------------------------------------------------
        template <typename range_t>
        constexpr array_slice& operator=(range_t& range)
            requires is_mut_array_range_of<range_t, value_t>
        {
            _data = range.get_mut_data();
            _count = range.get_count();
        }

        /// ----------------------------------------------------------------------------------------
        /// # array constructor
        /// ----------------------------------------------------------------------------------------
        template <usize count>
        constexpr array_slice(const value_t (&arr)[count])
            : _data(arr)
            , _count(count)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # array operator
        /// ----------------------------------------------------------------------------------------
        template <usize count>
        constexpr array_slice& operator=(const value_t (&arr)[count])
        {
            _data = arr;
            _count = count;
        }

        /// ----------------------------------------------------------------------------------------
        /// # trivial destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~array_slice() = default;

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_data(this const this_t& self) -> const value_t*
        {
            return self._data;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_data(this this_t& self) -> value_t*
        {
            return self._data;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_mut_data(this this_t& self) -> value_t*
        {
            return self._data;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_count(this const this_t& self) -> usize
        {
            return self._count;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_iter(this const this_t& self) -> iter_t
        {
            return iter_t(self._data);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_iter_end(this const this_t& self) -> iter_end_t
        {
            return iter_end_t(self._data + self._count);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_mut_iter(this this_t& self) -> mut_iter_t
        {
            return mut_iter_t(self._data);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_mut_iter_end(this this_t& self) -> mut_iter_end_t
        {
            return mut_iter_end_t(self._data + self._count);
        }

    private:
        value_t* _data;
        usize _count;
    };
}
