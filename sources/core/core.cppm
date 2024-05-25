export module atom.core:core.core;

import std;

export namespace atom
{
    struct _with_count_type{};
    struct _with_capacity_type{};

    constexpr auto _with_count = _with_count_type{};
    constexpr auto _with_capacity = _with_capacity_type{};

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

export namespace atom
{
    template <typename value_t0, typename value_t1>
    concept _is_equality_comparable_with = requires(const value_t0 v0, const value_t1 v1) {
        { v0.is_eq(v1) } -> std::same_as<bool>;
    };

    template <typename value_t0, typename value_t1>
    concept _is_comparable_with = requires(const value_t0 v0, const value_t1 v1) {
        requires _is_equality_comparable_with<value_t0, value_t1>;

        { v0.is_lt(v1) } -> std::same_as<bool>;
        { v0.is_gt(v1) } -> std::same_as<bool>;
        { v0.is_le(v1) } -> std::same_as<bool>;
        { v0.is_ge(v1) } -> std::same_as<bool>;
    };

    template <typename value_t0, typename value_t1>
    constexpr auto operator==(const value_t0& v0, const value_t1& v1) -> bool
        requires _is_equality_comparable_with<value_t0, value_t1>
    {
        return v0.is_eq(v1);
    }

    template <typename value_t0, typename value_t1>
    constexpr auto operator!=(const value_t0& v0, const value_t1& v1) -> bool
        requires _is_equality_comparable_with<value_t0, value_t1>
    {
        return not v0.is_eq(v1);
    }

    template <typename value_t0, typename value_t1>
    constexpr auto operator<(const value_t0& v0, const value_t1& v1) -> bool
        requires _is_comparable_with<value_t0, value_t1>
    {
        return v0.is_lt(v1);
    }

    template <typename value_t0, typename value_t1>
    constexpr auto operator>(const value_t0& v0, const value_t1& v1) -> bool
        requires _is_comparable_with<value_t0, value_t1>
    {
        return v0.is_gt(v1);
    }

    template <typename value_t0, typename value_t1>
    constexpr auto operator<=(const value_t0& v0, const value_t1& v1) -> bool
        requires _is_comparable_with<value_t0, value_t1>
    {
        return v0.is_le(v1);
    }

    template <typename value_t0, typename value_t1>
    constexpr auto operator>=(const value_t0& v0, const value_t1& v1) -> bool

        requires _is_comparable_with<value_t0, value_t1>
    {
        return v0.is_ge(v1);
    }
}
