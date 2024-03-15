#pragma once
#include "atom/core/core.h"

namespace atom
{
    template <typename value_t>
    class range_literal
    {
    public:
        template <usize count>
        constexpr range_literal(const value_t (&arr)[count])
            : _arr(arr)
            , _count(count)
        {}

        constexpr range_literal(initializer_list<value_t> list)
            : _arr(list.begin())
            , _count(list.size())
        {}

    public:
        constexpr auto get_data() const -> const value_t*
        {
            return _arr;
        }

        constexpr auto get_count() const -> usize
        {
            return _count;
        }

    private:
        const value_t* _arr;
        usize _count;
    };
}
