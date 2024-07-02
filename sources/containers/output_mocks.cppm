export module atom_core:containers.output_mocks;

import :types;
import :ranges;

namespace atom::mock
{
    export template <typename value_type>
    class output
    {
        static_assert(type_info<value_type>::is_pure);

    public:
        auto operator+=(const value_type& el);

        auto operator+=(value_type&& el);

        template <typename range_type>
        auto operator+=(const range_type& range)
            requires ranges::is_range_of<range_type, value_type>;
    };
}
