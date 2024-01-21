#pragma once
#include "atom/range.h"

// clang-format off

namespace atom
{
    template <typename output_type, typename value_type>
    concept routput = requires(
        output_type out, value_type value, range_req_mock<value_type> range)
    {
        { out += value };
        { out += mov(value) };
        { out += range };
        { out += mov(range) };
    };
}

// clang-format on
