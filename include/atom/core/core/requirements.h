#pragma once
#include "atom/core/_std.h"
#include "atom/core/core/core.h"

// clang-format off
namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// ensures `value_t0` is same as `value_t1`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t0, typename value_t1>
    concept is_same_as = std::same_as<value_t0, value_t1>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `value_t0` is `void`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept is_void = is_same_as<value_t, void>;

    /// --------------------------------------------------------------------------------------------
    /// ensures unqualified type of `value_t0` is same as unqualified type of `value_t1`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t0, typename value_t1>
    concept is_same_as_unqualified =
        std::same_as<std::remove_cvref_t<value_t0>, std::remove_cvref_t<value_t1>>;

    /// --------------------------------------------------------------------------------------------
    /// enusres `type` is const-qualified.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept is_const = std::is_const_v<value_t>;

    /// --------------------------------------------------------------------------------------------
    /// enusres `type` is volatile-qualified.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept is_volatile = std::is_volatile_v<value_t>;

    /// --------------------------------------------------------------------------------------------
    /// enusres `type` is ref.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept is_ref = std::is_reference_v<value_t>;

    /// --------------------------------------------------------------------------------------------
    /// enusres `type` is l-value ref.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept is_lvalue_ref = std::is_lvalue_reference_v<value_t>;

    /// --------------------------------------------------------------------------------------------
    /// enusres `type` is r-value ref.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept is_rvalue_ref = std::is_rvalue_reference_v<value_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `from_t` is `convertible` to `to_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename from_t, typename to_t>
    concept is_convertible_to = requires(from_t from) { static_cast<to_t>(from); };

    /// --------------------------------------------------------------------------------------------
    /// ensures `derived_t` is derived from `base_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename derived_t, typename base_t>
    concept is_derived_from =
        std::derived_from<std::remove_cvref_t<derived_t>, std::remove_cvref_t<base_t>>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `derived_t` is same as or derived from `base_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename derived_t, typename base_t>
    concept is_same_or_derived_from =
        is_same_as<derived_t, base_t> || is_derived_from<derived_t, base_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `constructible` using `args...`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t, typename... arg_ts>
    concept is_constructible = requires(arg_ts&&... args)
    {
        value_t(forward<arg_ts>(args)...);
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `trivially_constructible` using `args...`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t, typename... arg_ts>
    concept is_trivially_constructible = std::is_trivially_constructible_v<value_t, arg_ts...>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `default_constructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept is_default_constructible = is_constructible<value_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `trivially_default_constructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept is_trivially_default_constructible = is_trivially_constructible<value_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `copy_constructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept is_copy_constructible = is_constructible<value_t, const value_t&>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `trivially_copy_constructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept is_trivially_copy_constructible = is_trivially_constructible<value_t, const value_t&>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `move_constructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept is_move_constructible = is_constructible<value_t, value_t&&>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `trivially_move_constructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept is_trivially_move_constructible = is_trivially_constructible<value_t, value_t&&>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `default_initializable`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept is_default_initializable = requires
    {
        requires is_default_constructible<value_t>;

        (void)new value_t;
        value_t();
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `value_t0` is `assignable` using `value_t1`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t0, typename value_t1>
    concept is_assignable = requires(value_t0 v0, value_t1 v1)
    {
        { v0 = forward<value_t1>(v1) } -> is_same_as<value_t0&>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `trivially_assignable` using `from`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t0, typename value_t1>
    concept is_trivially_assignable = std::is_trivially_assignable_v<value_t0, value_t1>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `copy_assignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept is_copy_assignable = is_assignable<value_t, const value_t&>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `trivially_copy_assignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept is_trivially_copy_assignable = is_trivially_assignable<value_t&, const value_t&>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `move_assignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept is_move_assignable = is_assignable<value_t, value_t&&>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `trivially_move_assignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept is_trivially_move_assignable = is_trivially_assignable<value_t&, value_t&&>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is``copy_constructible` and `copy_assignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept is_copyable = is_copy_constructible<value_t> && is_copy_assignable<value_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is``copy_constructible` and `trivially_copy_assignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept is_trivially_copyable =
        is_trivially_copy_constructible<value_t> && is_trivially_copy_assignable<value_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `move_constructible` and `move_assignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept is_moveable = is_move_constructible<value_t> && is_move_assignable<value_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `move_constructible` and `trivially_move_assignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept is_trivially_moveable =
        is_trivially_move_constructible<value_t> && is_trivially_move_assignable<value_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `value_t0` is `swappable` with `value_t1`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t0, typename value_t1>
    concept is_swappable_with = is_assignable<value_t0, value_t1> && is_assignable<value_t1, value_t0>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `value_t0` is `trivially_swappable` with `t2`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t0, typename value_t1>
    concept is_trivially_swappable_with =
        is_trivially_assignable<value_t0, value_t1> && is_trivially_assignable<value_t1, value_t0>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `swappable` with itself.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept is_swappable = is_swappable_with<value_t, value_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `trivially_swappable` with itself.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept is_trivially_swappable = is_trivially_swappable_with<value_t, value_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `destructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept is_destructible = std::destructible<value_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `trivally_destructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept is_trivially_destructible = std::is_trivially_destructible_v<value_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `value_t0` and `value_t1` are `equality_comparable`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t0, typename value_t1>
    concept _is_equality_comparable_with = requires(const value_t0 v0, const value_t1 v1)
    {
        { v0.is_eq(v1) } -> is_same_as<bool>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `value_t0` and `value_t1` are `equality_comparable`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t0, typename value_t1>
    concept is_equality_comparable_with = _is_equality_comparable_with<value_t0, value_t1> or
        requires(const value_t0 v0, const value_t1 v1)
    {
        { v0 == v1 } -> is_same_as<bool>;
        { v0 != v1 } -> is_same_as<bool>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `equality_comparable`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept is_equality_comparable = is_equality_comparable_with<value_t, value_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `value_t0` and `value_t1` are `comparable`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t0, typename value_t1>
    concept _is_comparable_with = requires(const value_t0 v0, const value_t1 v1)
    {
        requires _is_equality_comparable_with<value_t0, value_t1>;

        { v0.is_lt(v1) } -> is_same_as<bool>;
        { v0.is_gt(v1) } -> is_same_as<bool>;
        { v0.is_le(v1) } -> is_same_as<bool>;
        { v0.is_ge(v1) } -> is_same_as<bool>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `value_t0` and `value_t1` are `comparable`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t0, typename value_t1>
    concept is_comparable_with = _is_comparable_with<value_t0, value_t1> or
        requires(const value_t0 v0, const value_t1 v1)
    {
        requires is_equality_comparable_with<value_t0, value_t1>;

        { v0 < v1 } -> is_same_as<bool>;
        { v0 > v1 } -> is_same_as<bool>;
        { v0 <= v1 } -> is_same_as<bool>;
        { v0 >= v1 } -> is_same_as<bool>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `comparable`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept is_comparable = is_comparable_with<value_t, value_t>;

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept is_pure = not std::is_const_v<value_t> and not std::is_volatile_v<value_t>
        and not std::is_reference_v<value_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is {semi_regular}.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept is_semi_regular = is_copyable<value_t> && is_default_initializable<value_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is {regular}.
    /// --------------------------------------------------------------------------------------------
    template <class value_t>
    concept is_regular = is_semi_regular<value_t> && is_equality_comparable<value_t>;
}

// clang-format on

namespace atom
{
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
