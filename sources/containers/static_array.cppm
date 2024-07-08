export module atom_core:containers.static_array;

import :core;
import :types;
import :ranges;

namespace atom
{
    class static_array_tag
    {};

    export template <typename in_value_type, usize count>
    class static_array: public static_array_tag
    {
    public:
        using value_type = in_value_type;
        using const_iterator_type = array_iterator<value_type>;
        using const_iterator_end_type = const_iterator_type;
        using iterator_type = mut_array_iterator<value_type>;
        using iterator_end_type = iterator_type;

    public:
        constexpr static_array() = default;

        template <usize other_count>
        constexpr static_array(const value_type (&arr)[other_count])
            requires(other_count <= count)
            : _arr{ arr }
        {}

        template <typename... arg_types>
        constexpr static_array(arg_types&&... args)
            requires(type_list<arg_types...>::template are_convertible_to<value_type>())
                    and (type_list<arg_types...>::get_count() <= count)
            : _arr{ 0 }
        {}

    public:
        constexpr auto operator[](usize i) -> value_type&
        {
            return _arr[i];
        }

        constexpr auto operator[](usize i) const -> const value_type&
        {
            return _arr[i];
        }

        constexpr auto get_data() -> value_type*
        {
            return _arr;
        }

        constexpr auto get_data() const -> const value_type*
        {
            return _arr;
        }

        constexpr auto get_count() const -> usize
        {
            return count;
        }

        constexpr auto get_iterator() const -> const_iterator_type
        {
            return const_iterator_type{ _arr };
        }

        constexpr auto get_iterator_end() const -> const_iterator_end_type
        {
            return const_iterator_end_type{ _arr + count };
        }

        constexpr auto get_iterator() -> iterator_type
        {
            return iterator_type{ _arr };
        }

        constexpr auto get_iterator_end() -> iterator_end_type
        {
            return iterator_end_type{ _arr + count };
        }

    private:
        value_type _arr[count];
    };

    template <typename range_type>
        requires(type_info<range_type>::template is_derived_from<static_array_tag>())
    class ranges::range_definition<range_type>
    {
    public:
        using value_type = typename range_type::value_type;
        using iterator_type = typename range_type::iterator_type;
        using iterator_end_type = typename range_type::iterator_end_type;
        using const_iterator_type = typename range_type::const_iterator_type;
        using const_iterator_end_type = typename range_type::const_iterator_end_type;

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
