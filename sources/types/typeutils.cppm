export module atom.core:types.typeutils;

import std;

namespace atom
{
    export class typeutils
    {
    public:
        class empty
        {};

        template <bool condition, typename true_type, typename false_type>
        using conditional_type = std::conditional_t<condition, true_type, false_type>;

        template <bool condition, typename true_type, typename false_type = empty>
        using conditional_field_type = conditional_type<condition, true_type, empty>;

        template <bool requirements, typename value_type = void>
        using enable_if_type = std::enable_if_t<requirements, value_type>;

        template <typename value_type>
        using identity_type = std::type_identity_t<value_type>;
    };
}
