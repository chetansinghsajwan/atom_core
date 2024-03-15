#pragma once
#include "atom/core/core.h"
#include "atom/core/range/array_iter.h"
#include "atom/core/range.h"

namespace atom
{
    template <typename in_elem_t, usize in_count>
    class basic_static_array
    {
    public:
        using elem_t = in_elem_t;
        using iter_t = array_iter<elem_t>;
        using iter_end_t = iter_t;
        using mut_iter_t = mut_array_iter<elem_t>;
        using mut_iter_end_t = mut_iter_t;

    public:
        constexpr basic_static_array() = default;

        template <usize other_count>
        constexpr basic_static_array(const elem_t (&arr)[other_count])
            requires(other_count <= in_count)
            : _arr(arr)
        {}

        template <typename... arg_ts>
        constexpr basic_static_array(arg_ts&&... args)
            requires(rconvertible_to<arg_ts, elem_t> and ...)
                    and (sizeof...(arg_ts) <= in_count)
            : _arr(0)
        {}

    public:
        constexpr auto get_data() const -> const elem_t*
        {
            return _arr;
        }

        constexpr auto get_mut_data() -> elem_t*
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
        elem_t _arr[in_count];
    };

    template <typename elem_t, usize count>
    class static_array: public mut_array_range_extensions<basic_static_array<elem_t, count>>
    {
        using base_t = mut_array_range_extensions<basic_static_array<elem_t, count>>;

    public:
        using base_t::base_t;
        using base_t::operator=;
    };
}
