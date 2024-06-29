export module atom.core:core.union_storage;

import :types;
import :core.core;

namespace atom
{
    export template <typename... value_types>
    union union_storage;

    template <>
    union union_storage<>
    {};

    template <typename this_value_type, typename... other_value_types>
    union union_storage<this_value_type, other_value_types...>
    {
        using this_type = union_storage;
        using next_type = union_storage<other_value_types...>;
        using this_field_value_type =
            type_utils::conditional_type<not type_info<this_value_type>::is_void(),
                this_value_type>;

    public:
        constexpr union_storage() = default;
        constexpr union_storage(const this_type& that) = default;
        constexpr union_storage& operator=(const this_type& that) = default;
        constexpr union_storage(this_type&& that) = default;
        constexpr union_storage& operator=(this_type&& that) = default;

        template <typename value_type, typename... arg_types>
        constexpr union_storage(create_by_emplace_tag<value_type> tag, arg_types&&... args)
            : _next{ tag, forward<arg_types>(args)... }
        {}

        template <typename... arg_types>
        constexpr union_storage(create_by_emplace_tag<this_value_type> tag, arg_types&&... args)
            : _value{ forward<arg_types>(args)... }
        {}

        constexpr ~union_storage() = default;

        constexpr ~union_storage()
            requires(not type_info<this_value_type>::is_trivially_destructible()
                     or not type_info<next_type>::is_trivially_destructible())
        {}

    public:
        template <typename value_type>
        constexpr auto get_data_as() const -> const value_type*
        {
            if constexpr (type_info<value_type>::template is_same_as<this_value_type>())
                return &_value;
            else
                return _next.template get_data_as<value_type>();
        }

        template <typename value_type>
        constexpr auto get_data_as() -> value_type*
        {
            if constexpr (type_info<value_type>::template is_same_as<this_value_type>())
                return &_value;
            else
                return _next.template get_data_as<value_type>();
        }

    private:
        this_field_value_type _value;
        next_type _next;
    };
}
