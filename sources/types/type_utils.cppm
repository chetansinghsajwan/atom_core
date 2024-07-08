export module atom_core:types.type_utils;

import std;
import :contracts;

namespace atom
{
    export class type_utils
    {
    public:
        class empty_type
        {};

        template <bool condition, typename true_type, typename false_type = empty_type>
        using conditional_type = std::conditional_t<condition, true_type, false_type>;

        template <bool requirements, typename value_type = void>
        using enable_if_type = std::enable_if_t<requirements, value_type>;

        template <typename value_type>
        using identity_type = std::type_identity_t<value_type>;

        template <typename value_type, typename... arg_types>
        static constexpr auto construct_as(void* value, arg_types&&... args) -> void
        {
            contract_debug_expects(value != nullptr);

            construct(static_cast<value_type*>(value), std::forward<arg_types>(args)...);
        }

        template <typename value_type, typename... arg_types>
        static constexpr auto construct(value_type* value, arg_types&&... args) -> void
        {
            contract_debug_expects(value != nullptr);

            std::construct_at(value, std::forward<arg_types>(args)...);
        }

        template <typename value_type, typename arg_type>
        static constexpr auto assign_as(void* value, arg_type&& arg) -> void
        {
            contract_debug_expects(value != nullptr);

            assign(static_cast<value_type*>(value), std::forward<arg_type>(arg));
        }

        template <typename value_type, typename arg_type>
        static constexpr auto assign(value_type* value, arg_type&& arg) -> void
        {
            contract_debug_expects(value != nullptr);

            *value = std::forward<arg_type>(arg);
        }

        template <typename value_type>
        static constexpr auto swap(value_type& value0, value_type& value1) -> void
        {
            value_type tmp = move(value0);
            value0 = move(value1);
            value1 = move(tmp);
        }

        template <typename value_type>
        static constexpr auto destruct_as(void* value) -> void
        {
            contract_debug_expects(value != nullptr);

            destruct(static_cast<value_type*>(value));
        }

        template <typename value_type>
        static constexpr auto destruct(value_type* value) -> void
        {
            contract_debug_expects(value != nullptr);

            std::destroy_at(value);
        }
    };
}
