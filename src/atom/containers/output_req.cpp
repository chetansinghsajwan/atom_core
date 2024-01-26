export module atom.core:output_requirements;
import :core;
import :range;

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
