#pragma once
#include <type_traits>

namespace atom
{
    class typeutils
    {
    public:
        class _empty
        {};

        template <bool condition, typename true_t, typename false_t>
        using conditional_t = std::conditional_t<condition, true_t, false_t>;

        template <bool condition, typename true_t, typename false_t = _empty>
        using conditional_field_t = conditional_t<condition, true_t, _empty>;

        template <bool requirements, typename value_t = void>
        using enable_if_t = std::enable_if_t<requirements, value_t>;

        template <typename value_t>
        using identity_t = std::type_identity_t<value_t>;
    };
}
