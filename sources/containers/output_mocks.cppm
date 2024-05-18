export module atom.core:containers.output_mocks;

import :types;
import :ranges;

namespace atom::mock
{
    export template <typename value_t>
    class output
    {
        static_assert(typeinfo<value_t>::is_pure);

    public:
        auto operator+=(const value_t& el);

        auto operator+=(value_t&& el);

        template <typename range_t>
        auto operator+=(const range_t& range)
            requires is_range_of<range_t, value_t>;
    };
}
