export module atom.core:core.core;

import std;

export namespace atom
{
    struct _with_count_type
    {};

    struct _with_capacity_type
    {};

    struct _with_size_type
    {};

    struct create_from_raw_tag
    {};

    struct create_with_join_tag
    {};

    struct create_from_std_vector_tag
    {};

    constexpr auto _with_count = _with_count_type{};
    constexpr auto _with_capacity = _with_capacity_type{};
    constexpr auto _with_size = _with_size_type{};
    constexpr auto create_from_raw = create_from_raw_tag{};
    constexpr auto create_with_join = create_with_join_tag{};
    constexpr auto create_from_std_vector = create_from_std_vector_tag{};

    template <typename value_type>
    constexpr auto move(value_type&& val) -> typename std::remove_reference_t<value_type>&&
    {
        return static_cast<typename std::remove_reference_t<value_type>&&>(val);
    }

    template <typename value_type>
    constexpr auto forward(typename std::remove_reference_t<value_type>& val) -> value_type&&
    {
        return static_cast<value_type&&>(val);
    }

    template <typename value_type>
    constexpr auto forward(typename std::remove_reference_t<value_type>&& val) -> value_type&&
    {
        static_assert(
            !std::is_lvalue_reference_v<value_type>, "can not forward an rvalue as an lvalue.");

        return static_cast<value_type&&>(val);
    }

    using nullptr_t = std::nullptr_t;

    template <typename... arg_types>
    class type_holder
    {};

    template <typename value_type>
    using initializer_list = std::initializer_list<value_type>;
}
