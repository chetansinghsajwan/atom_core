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

    struct create_with_join_tag
    {};

    struct create_from_std_vector_tag
    {};

    constexpr auto _with_count = _with_count_type{};
    constexpr auto _with_capacity = _with_capacity_type{};
    constexpr auto _with_size = _with_size_type{};
    constexpr auto create_with_join = create_with_join_tag{};
    constexpr auto create_from_std_vector = create_from_std_vector_tag{};

    template <typename value_t>
    constexpr auto move(value_t&& val) -> typename std::remove_reference_t<value_t>&&
    {
        return static_cast<typename std::remove_reference_t<value_t>&&>(val);
    }

    template <typename value_t>
    constexpr auto forward(typename std::remove_reference_t<value_t>& val) -> value_t&&
    {
        return static_cast<value_t&&>(val);
    }

    template <typename value_t>
    constexpr auto forward(typename std::remove_reference_t<value_t>&& val) -> value_t&&
    {
        static_assert(
            !std::is_lvalue_reference_v<value_t>, "can not forward an rvalue as an lvalue.");

        return static_cast<value_t&&>(val);
    }

    using nullptr_t = std::nullptr_t;

    template <typename... arg_ts>
    class type_holder
    {};

    template <typename value_t>
    using initializer_list = std::initializer_list<value_t>;
}
