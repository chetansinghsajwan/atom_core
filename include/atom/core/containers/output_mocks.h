#pragma once
#include "atom/core/typeinfo.h"
#include "atom/core/range.h"

namespace atom::mock
{
    template <typename value_t>
    class output
    {
        ATOM_STATIC_ASSERTS(typeinfo::is_pure<value_t>);

    public:
        auto operator+=(const value_t& el);

        auto operator+=(value_t&& el);

        template <typename range_t>
        auto operator+=(const range_t& range)
            requires(rrange_of<range_t, value_t>);
    };
}
