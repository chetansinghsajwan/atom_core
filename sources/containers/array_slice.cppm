export module atom_core:containers.array_slice;

import :core;
import :ranges;
import :types;

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    export template <typename in_elem_type>
    class array_slice
    {
        static_assert(type_info<in_elem_type>::is_pure());

    private:
        using this_type = array_slice;

    public:
        using value_type = in_elem_type;
        using const_iterator_type = array_iterator<value_type>;
        using const_iterator_end_type = const_iterator_type;
        using iterator_type = mut_array_iterator<value_type>;
        using iterator_end_type = iterator_type;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr array_slice()
            : _data{ nullptr }
            , _count{ 0 }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # trivial copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr array_slice(const this_type& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # trivial copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr array_slice& operator=(const this_type& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # trivial move constructor
        /// ----------------------------------------------------------------------------------------
        constexpr array_slice(this_type&& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # trivial move operator
        /// ----------------------------------------------------------------------------------------
        constexpr array_slice& operator=(this_type&& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # range constructor
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        constexpr array_slice(range_type& range)
            requires ranges::array_range_concept<range_type, value_type>
            : _data{ range.get_data() }
            , _count{ range.get_count() }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # range operator
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        constexpr array_slice& operator=(range_type& range)
            requires ranges::array_range_concept<range_type, value_type>
        {
            _data = range.get_data();
            _count = range.get_count();
        }

        /// ----------------------------------------------------------------------------------------
        /// # array constructor
        /// ----------------------------------------------------------------------------------------
        template <usize count>
        constexpr array_slice(const value_type (&arr)[count])
            : _data{ arr }
            , _count{ count }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # array operator
        /// ----------------------------------------------------------------------------------------
        template <usize count>
        constexpr array_slice& operator=(const value_type (&arr)[count])
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
        constexpr auto operator[](usize i) -> value_type&
        {
            return _data[i];
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator[](usize i) const -> const value_type&
        {
            return _data[i];
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_data() const -> const value_type*
        {
            return _data;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_data() -> value_type*
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
        constexpr auto get_iterator() const -> const_iterator_type
        {
            return const_iterator_type(_data);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_iterator_end() const -> const_iterator_end_type
        {
            return const_iterator_end_type(_data + _count);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_iterator() -> iterator_type
        {
            return iterator_type(_data);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_iterator_end() -> iterator_end_type
        {
            return iterator_end_type(_data + _count);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_empty() const -> bool
        {
            return _count == 0;
        }

    private:
        value_type* _data;
        usize _count;
    };
}
