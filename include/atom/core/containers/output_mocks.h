#pragma once
#include "atom/core/tti.h"
#include "atom/core/range.h"

namespace atom::mock
{
    template <typename value_type>
    class output
    {
        static_assert(tti::is_pure<value_type>);

    public:
        auto operator+=(const value_type& el);

        auto operator+=(value_type&& el);

        template <typename range_type>
        auto operator+=(const range_type& range)
            requires(rrange_of<range_type, value_type>);
    };
}
