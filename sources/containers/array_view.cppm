export module atom_core:containers.array_view;

import std;
import :core;
import :ranges;
import :types;

namespace atom
{
    export class array_view_tag
    {};

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    export template <typename in_value_type>
    class array_view: public array_view_tag
    {
        static_assert(type_info<in_value_type>::is_pure());

    private:
        using this_type = array_view;

    public:
        using value_type = in_value_type;
        using const_iterator_type = const value_type*;
        using const_iterator_end_type = const_iterator_type;

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr array_view()
            : _data{ nullptr }
            , _count{ 0 }
        {}

        constexpr array_view(const this_type& that) = default;
        constexpr array_view& operator=(const this_type& that) = default;

        constexpr array_view(this_type&& that) = default;
        constexpr array_view& operator=(this_type&& that) = default;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        constexpr array_view(const range_type& range)
            requires(ranges::const_array_range_concept<range_type, value_type>)
            : _data{ ranges::get_data(range) }
            , _count{ ranges::get_count(range) }
        {}

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        constexpr array_view& operator=(const range_type& range)
            requires(ranges::const_array_range_concept<range_type, value_type>)
        {
            _data = ranges::get_data(range);
            _count = ranges::get_count(range);

            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # trivial destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~array_view() = default;

    public:
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
        constexpr auto get_first() const -> const value_type&
        {
            return *_data;
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
        constexpr auto get_size() const -> usize
        {
            return _count * sizeof(char) / sizeof(byte);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_iterator() const -> const_iterator_type
        {
            return const_iterator_type{ _data };
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_iterator_end() const -> const_iterator_end_type
        {
            return const_iterator_end_type{ _data + _count };
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_empty() const -> bool
        {
            return _count == 0;
        }

    private:
        const value_type* _data;
        usize _count;
    };

    export template <typename range_type>
        requires(type_info<range_type>::template is_derived_from<array_view_tag>())
    class ranges::range_definition<range_type>
    {
    public:
        using value_type = typename range_type::value_type;
        using const_iterator_type = typename range_type::const_iterator_type;
        using const_iterator_end_type = typename range_type::const_iterator_end_type;

    public:
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
