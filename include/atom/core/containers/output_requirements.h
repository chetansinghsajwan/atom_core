#pragma once
#include "atom/core/core.h"
#include "atom/core/range.h"

// clang-format off

namespace atom
{
    template <typename output_type, typename value_type>
    concept routput =
        requires(output_type out, value_type value, mock::range<value_type> range)
    {
        { out += value };
        { out += move(value) };
        { out += range };
        { out += move(range) };
    };
}

// clang-format on
