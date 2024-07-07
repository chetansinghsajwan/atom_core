export module atom_core:containers.static_array;

import :core;
import :types;
import :ranges;

namespace atom
{
    export template <typename in_value_type, usize in_count>
    class static_array
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
            requires(other_count <= in_count)
            : _arr{ arr }
        {}

        template <typename... arg_types>
        constexpr static_array(arg_types&&... args)
            requires(type_list<arg_types...>::template are_convertible_to<value_type>())
                    and (type_list<arg_types...>::get_count() <= in_count)
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
            return in_count;
        }

        constexpr auto get_iterator() const -> const_iterator_type
        {
            return const_iterator_type{ _arr };
        }

        constexpr auto get_iterator_end() const -> const_iterator_end_type
        {
            return const_iterator_end_type{ _arr + in_count };
        }

        constexpr auto get_iterator() -> iterator_type
        {
            return iterator_type{ _arr };
        }

        constexpr auto get_iterator_end() -> iterator_end_type
        {
            return iterator_end_type{ _arr + in_count };
        }

    private:
        value_type _arr[in_count];
    };
}
