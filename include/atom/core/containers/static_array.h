#pragma once
// #include "atom/core/core.h"
#include "atom/core/range/array_iter.h"
#include "atom/core/range.h"

namespace atom
{
    template <typename in_elem_t, usize in_count>
    class static_array: public range_extensions
    {
    public:
        using value_t = in_elem_t;
        using iter_t = array_iter<value_t>;
        using iter_end_t = iter_t;
        using mut_iter_t = mut_array_iter<value_t>;
        using mut_iter_end_t = mut_iter_t;

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

        constexpr auto get_iter() const -> iter_t
        {
            return iter_t(_arr);
        }

        constexpr auto get_iter_end() const -> iter_end_t
        {
            return iter_end_t(_arr + in_count);
        }

        constexpr auto get_mut_iter() -> mut_iter_t
        {
            return mut_iter_t(_arr);
        }

        constexpr auto get_mut_iter_end() -> mut_iter_end_t
        {
            return mut_iter_end_t(_arr + in_count);
        }

    private:
        value_t _arr[in_count];
    };
}
