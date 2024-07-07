export module atom_core:containers.array_view;

import std;
import :core;
import :ranges;
import :types;

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    export template <typename in_value_type>
    class array_view
    {
        static_assert(type_info<in_value_type>::is_pure());

    private:
        using this_type = array_view;

    public:
        using value_type = in_value_type;
        using const_iterator_type = array_iterator<value_type>;
        using const_iterator_end_type = const_iterator_type;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr array_view()
            : _data{ nullptr }
            , _count{ 0 }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # trivial copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr array_view(const this_type& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # trivial copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr array_view& operator=(const this_type& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # trivial move constructor
        /// ----------------------------------------------------------------------------------------
        constexpr array_view(this_type&& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # trivial move operator
        /// ----------------------------------------------------------------------------------------
        constexpr array_view& operator=(this_type&& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # range constructor
        /// ----------------------------------------------------------------------------------------
        constexpr array_view(std::initializer_list<value_type> list)
            : _data{ list.begin() }
            , _count{ list.size() }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # range constructor
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        constexpr array_view(const range_type& range)
            requires ranges::const_array_range_concept<range_type, value_type>
            : _data{ range.get_data() }
            , _count{ range.get_count() }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # range operator
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        constexpr array_view& operator=(const range_type& range)
            requires ranges::const_array_range_concept<range_type, value_type>
        {
            _data = range.get_data();
            _count = range.get_count();
        }

        /// ----------------------------------------------------------------------------------------
        /// # array constructor
        /// ----------------------------------------------------------------------------------------
        template <usize count>
        constexpr array_view(const value_type (&arr)[count])
            : _data{ arr }
            , _count{ count }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # array operator
        /// ----------------------------------------------------------------------------------------
        template <usize count>
        constexpr array_view& operator=(const value_type (&arr)[count])
        {
            _data = arr;
            _count = count;

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
        constexpr auto is_empty() const -> bool
        {
            return _count == 0;
        }

    private:
        const value_type* _data;
        usize _count;
    };
}
