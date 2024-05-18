export module atom.core:containers.output_requirements;

import :core;
import :ranges;

// clang-format off

namespace atom
{
    template <typename output_t, typename value_t>
    concept is_output = requires(output_t out, value_t value, mock::range<value_t> range)
    {
        { out += value };
        { out += move(value) };
        { out += range };
        { out += move(range) };
    };
}

// clang-format on
