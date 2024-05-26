export module atom.core:containers.static_array;

import :core;
import :types;
import :ranges;

namespace atom
{
    export template <typename in_elem_t, usize in_count>
    class static_array: public range_extensions
    {
    public:
        using value_t = in_elem_t;
        using iterator_t = array_iterator<value_t>;
        using iterator_end_t = iterator_t;
        using mut_iterator_t = mut_array_iterator<value_t>;
        using mut_iterator_end_t = mut_iterator_t;

    public:
        constexpr static_array() = default;

        template <usize other_count>
        constexpr static_array(const value_t (&arr)[other_count])
            requires(other_count <= in_count)
            : _arr(arr)
        {}

        template <typename... arg_ts>
        constexpr static_array(arg_ts&&... args)
            requires(typelist<arg_ts...>::info_t::template are_convertible_to<value_t>)
                    and (typelist<arg_ts...>::count <= in_count)
            : _arr(0)
        {}

    public:
        constexpr auto get_data() const -> const value_t*
        {
            return _arr;
        }

        constexpr auto get_mut_data() -> value_t*
        {
            return _arr;
        }

        constexpr auto get_count() const -> usize
        {
            return in_count;
        }

        constexpr auto get_iterator() const -> iterator_t
        {
            return iterator_t(_arr);
        }

        constexpr auto get_iterator_end() const -> iterator_end_t
        {
            return iterator_end_t(_arr + in_count);
        }

        constexpr auto get_mut_iterator() -> mut_iterator_t
        {
            return mut_iterator_t(_arr);
        }

        constexpr auto get_mut_iterator_end() -> mut_iterator_end_t
        {
            return mut_iterator_end_t(_arr + in_count);
        }

    private:
        value_t _arr[in_count];
    };
}
