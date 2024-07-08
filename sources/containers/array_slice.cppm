export module atom_core:containers.array_slice;

import :core;
import :ranges;
import :types;
import :contracts;

namespace atom
{
    export class array_slice_tag
    {};

    /// --------------------------------------------------------------------------------------------
    /// \todo add doc.
    ///
    /// \pre `type_info<in_value_type>::is_pure()`.
    /// --------------------------------------------------------------------------------------------
    export template <typename in_value_type>
    class array_slice: public array_slice_tag
    {
        static_assert(type_info<in_value_type>::is_pure());

    private:
        using this_type = array_slice;

    public:
        using value_type = in_value_type;
        using const_iterator_type = const value_type*;
        using const_iterator_end_type = const_iterator_type;
        using iterator_type = value_type*;
        using iterator_end_type = iterator_type;

    public:
        /// ----------------------------------------------------------------------------------------
        /// initializes with nothing.
        /// ----------------------------------------------------------------------------------------
        constexpr array_slice()
            : _data{ nullptr }
            , _count{ 0 }
        {}

        constexpr array_slice(const this_type& that) = default;
        constexpr array_slice& operator=(const this_type& that) = default;

        constexpr array_slice(this_type&& that) = default;
        constexpr array_slice& operator=(this_type&& that) = default;

        constexpr ~array_slice() = default;

        /// ----------------------------------------------------------------------------------------
        /// initializes with array range `range`.
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        constexpr array_slice(range_type& range)
            requires(ranges::array_range_concept<range_type, value_type>)
            : _data{ ranges::get_data(range) }
            , _count{ ranges::get_count(range) }
        {}

        /// ----------------------------------------------------------------------------------------
        /// assigns new array range `range`.
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        constexpr array_slice& operator=(range_type& range)
            requires(ranges::array_range_concept<range_type, value_type>)
        {
            _data = ranges::get_data(range);
            _count = ranges::get_count(range);
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// \returns the reference to value at index `i`.
        ///
        /// \pre if debug `is_index_valid(i)`: index is out of bounds.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator[](usize i) -> value_type&
        {
            contract_debug_expects(is_index_valid(i), "index is out of bounds.");

            return _data[i];
        }

        /// \copydoc operator[]
        constexpr auto operator[](usize i) const -> const value_type&
        {
            contract_debug_expects(is_index_valid(i), "index is out of bounds.");

            return _data[i];
        }

        /// ----------------------------------------------------------------------------------------
        /// \returns underlying array pointer.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_data() -> value_type*
        {
            return _data;
        }

        /// \copydoc get_data()
        constexpr auto get_data() const -> const value_type*
        {
            return _data;
        }

        /// ----------------------------------------------------------------------------------------
        /// \returns the count of values `this` contains.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_count() const -> usize
        {
            return _count;
        }

        /// ----------------------------------------------------------------------------------------
        /// \returns the const_iterator_type to the first value. if array is null, this returns iterator
        /// pointing to nullptr.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_iterator() const -> const_iterator_type
        {
            return const_iterator_type{ _data };
        }

        /// ----------------------------------------------------------------------------------------
        /// \returns the const_iterator_end_type to one past the last value. if array is null,
        /// this returns iterator pointing to nullptr.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_iterator_end() const -> const_iterator_end_type
        {
            return const_iterator_end_type{ _data + _count };
        }

        /// ----------------------------------------------------------------------------------------
        /// \returns the iterator_type to the first value. if array is null, this returns
        /// iterator pointing to nullptr.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_iterator() -> iterator_type
        {
            return iterator_type{ _data };
        }

        /// ----------------------------------------------------------------------------------------
        /// \returns the iterator_end_type to one past the last value. if array is null,
        /// this returns iterator pointing to nullptr.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_iterator_end() -> iterator_end_type
        {
            return iterator_end_type{ _data + _count };
        }

        /// ----------------------------------------------------------------------------------------
        /// \returns `true` if `get_count() == 0`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_empty() const -> bool
        {
            return _count == 0;
        }

        /// ----------------------------------------------------------------------------------------
        /// \returns `true` if index `i` is in range `[0, get_count())`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_index_valid() const -> bool
        {
            return true;
        }

    private:
        value_type* _data;
        usize _count;
    };

    export template <typename range_type>
        requires(type_info<range_type>::template is_derived_from<array_slice_tag>())
    class ranges::range_definition<range_type>
    {
    public:
        using value_type = typename range_type::value_type;
        using const_iterator_type = typename range_type::const_iterator_type;
        using const_iterator_end_type = typename range_type::const_iterator_end_type;
        using iterator_type = typename range_type::iterator_type;
        using iterator_end_type = typename range_type::iterator_end_type;

    public:
        static constexpr auto get_iterator(range_type& range) -> iterator_type
        {
            return range.get_iterator();
        }

        static constexpr auto get_iterator_end(range_type& range) -> iterator_end_type
        {
            return range.get_iterator_end();
        }

        static constexpr auto get_const_iterator(const range_type& range) -> const_iterator_type
        {
            return range.get_iterator();
        }

        static constexpr auto get_const_iterator_end(
            const range_type& range) -> const_iterator_end_type
        {
            return range.get_iterator_end();
        }
    };
}
