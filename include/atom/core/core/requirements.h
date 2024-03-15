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
    concept rsame_as = std::same_as<value_t0, value_t1>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `value_t0` is `void`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept ris_void = rsame_as<value_t, void>;

    /// --------------------------------------------------------------------------------------------
    /// ensures unqualified type of `value_t0` is same as unqualified type of `value_t1`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t0, typename value_t1>
    concept rsame_as_unqualified =
        std::same_as<std::remove_cvref_t<value_t0>, std::remove_cvref_t<value_t1>>;

    /// --------------------------------------------------------------------------------------------
    /// enusres `type` is const-qualified.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept rconst = std::is_const_v<value_t>;

    /// --------------------------------------------------------------------------------------------
    /// enusres `type` is volatile-qualified.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept rvolatile = std::is_volatile_v<value_t>;

    /// --------------------------------------------------------------------------------------------
    /// enusres `type` is ref.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept rref = std::is_reference_v<value_t>;

    /// --------------------------------------------------------------------------------------------
    /// enusres `type` is l-value ref.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept rlvalue_ref = std::is_lvalue_reference_v<value_t>;

    /// --------------------------------------------------------------------------------------------
    /// enusres `type` is r-value ref.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept rrvalue_ref = std::is_rvalue_reference_v<value_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `from_t` is `convertible` to `to_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename from_t, typename to_t>
    concept rconvertible_to = requires(from_t from) { static_cast<to_t>(from); };

    /// --------------------------------------------------------------------------------------------
    /// ensures `derived_t` is derived from `base_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename derived_t, typename base_t>
    concept rderived_from =
        std::derived_from<std::remove_cvref_t<derived_t>, std::remove_cvref_t<base_t>>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `derived_t` is same as or derived from `base_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename derived_t, typename base_t>
    concept rsame_or_derived_from =
        rsame_as<derived_t, base_t> || rderived_from<derived_t, base_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `constructible` using `args...`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t, typename... arg_ts>
    concept rconstructible = requires(arg_ts&&... args)
    {
        value_t(forward<arg_ts>(args)...);
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `trivially_constructible` using `args...`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t, typename... arg_ts>
    concept rtrivially_constructible = std::is_trivially_constructible_v<value_t, arg_ts...>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `default_constructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept rdefault_constructible = rconstructible<value_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `trivially_default_constructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept rtrivially_default_constructible = rtrivially_constructible<value_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures each type in `types...` is `default_constructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename... types>
    concept rdefault_constructible_all = (rdefault_constructible<types> and ...);

    /// --------------------------------------------------------------------------------------------
    /// ensures each type in `types...` is `trivially_default_constructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename... types>
    concept rtrivially_default_constructible_all = (rtrivially_default_constructible<types> and ...);

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `copy_constructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept rcopy_constructible = rconstructible<value_t, const value_t&>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `trivially_copy_constructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept rtrivially_copy_constructible = rtrivially_constructible<value_t, const value_t&>;

    /// --------------------------------------------------------------------------------------------
    /// ensures each type in `types...` is `copy_constructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename... types>
    concept rcopy_constructible_all = (rcopy_constructible<types> and ...);

    /// --------------------------------------------------------------------------------------------
    /// ensures each type in `types...` is `trivially_copy_constructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename... types>
    concept rtrivially_copy_constructible_all = (rtrivially_copy_constructible<types> and ...);

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `move_constructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept rmove_constructible = rconstructible<value_t, value_t&&>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `trivially_move_constructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept rtrivially_move_constructible = rtrivially_constructible<value_t, value_t&&>;

    /// --------------------------------------------------------------------------------------------
    /// ensures each type in `types...` is `move_constructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename... types>
    concept rmove_constructible_all = (rmove_constructible<types> and ...);

    /// --------------------------------------------------------------------------------------------
    /// ensures each type in `types...` is `trivially_move_constructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename... types>
    concept rtrivially_move_constructible_all = (rtrivially_move_constructible<types> and ...);

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `default_initializable`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept rdefault_initializable = requires
    {
        requires rdefault_constructible<value_t>;

        (void)new value_t;
        value_t();
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures each type in `types...` is `default_initializable`.
    /// --------------------------------------------------------------------------------------------
    template <typename... types>
    concept rdefault_initializable_all = (rdefault_initializable<types> and ...);

    /// --------------------------------------------------------------------------------------------
    /// ensures `value_t0` is `assignable` using `value_t1`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t0, typename value_t1>
    concept rassignable = requires(value_t0 v0, value_t1 v1)
    {
        { v0 = forward<value_t1>(v1) } -> rsame_as<value_t0&>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `trivially_assignable` using `from`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t0, typename value_t1>
    concept rtrivially_assignable = std::is_trivially_assignable_v<value_t0, value_t1>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `copy_assignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept rcopy_assignable = rassignable<value_t, const value_t&>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `trivially_copy_assignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept rtrivially_copy_assignable = rtrivially_assignable<value_t&, const value_t&>;

    /// --------------------------------------------------------------------------------------------
    /// ensures each type in `types...` is `copy_assignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename... types>
    concept rcopy_assignable_all = (rcopy_assignable<types> and ...);

    /// --------------------------------------------------------------------------------------------
    /// ensures each type in `types...` is `trivially_copy_assignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename... types>
    concept rtrivially_copy_assignable_all = (rtrivially_copy_assignable<types> and ...);

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `move_assignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept rmove_assignable = rassignable<value_t, value_t&&>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `trivially_move_assignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept rtrivially_move_assignable = rtrivially_assignable<value_t&, value_t&&>;

    /// --------------------------------------------------------------------------------------------
    /// ensures each type in `types...` is `move_assignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename... types>
    concept rmove_assignable_all = (rmove_assignable<types> and ...);

    /// --------------------------------------------------------------------------------------------
    /// ensures each type in `types...` is `trivially_move_assignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename... types>
    concept rtrivially_move_assignable_all = (rtrivially_move_assignable<types> and ...);

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is``copy_constructible` and `copy_assignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept rcopyable = rcopy_constructible<value_t> && rcopy_assignable<value_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is``copy_constructible` and `trivially_copy_assignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept rtrivially_copyable =
        rtrivially_copy_constructible<value_t> && rtrivially_copy_assignable<value_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures each type in `types...` is `copyable`.
    /// --------------------------------------------------------------------------------------------
    template <typename... types>
    concept rcopyable_all = (rcopyable<types> and ...);

    /// --------------------------------------------------------------------------------------------
    /// ensures each type in `types...` is `trivially_copyable`.
    /// --------------------------------------------------------------------------------------------
    template <typename... types>
    concept rtrivially_copyable_all = (rtrivially_copyable<types> and ...);

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `move_constructible` and `move_assignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept rmoveable = rmove_constructible<value_t> && rmove_assignable<value_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `move_constructible` and `trivially_move_assignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept rtrivially_moveable =
        rtrivially_move_constructible<value_t> && rtrivially_move_assignable<value_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures each type in `types...` is `moveable`.
    /// --------------------------------------------------------------------------------------------
    template <typename... types>
    concept rmoveable_all = (rmoveable<types> and ...);

    /// --------------------------------------------------------------------------------------------
    /// ensures each type in `types...` is `trivially_moveable`.
    /// --------------------------------------------------------------------------------------------
    template <typename... types>
    concept rtrivially_moveable_all = (rtrivially_moveable<types> and ...);

    /// --------------------------------------------------------------------------------------------
    /// ensures `value_t0` is `swappable` with `value_t1`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t0, typename value_t1>
    concept rswappable_with = rassignable<value_t0, value_t1> && rassignable<value_t1, value_t0>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `value_t0` is `trivially_swappable` with `t2`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t0, typename value_t1>
    concept rtrivially_swappable_with =
        rtrivially_assignable<value_t0, value_t1> && rtrivially_assignable<value_t1, value_t0>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `swappable` with itself.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept rswappable = rswappable_with<value_t, value_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `trivially_swappable` with itself.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept rtrivially_swappable = rtrivially_swappable_with<value_t, value_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `destructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept rdestructible = std::destructible<value_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `trivally_destructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept rtrivially_destructible = std::is_trivially_destructible_v<value_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures each type in `types...` is `destructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename... types>
    concept rdestructible_all = (rdestructible<types> and ...);

    /// --------------------------------------------------------------------------------------------
    /// ensures each type in `types...` is `trivally_destructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename... types>
    concept rtrivially_destructible_all = (rtrivially_destructible<types> and ...);

    /// --------------------------------------------------------------------------------------------
    /// ensures `value_t0` and `value_t1` are `equality_comparable`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t0, typename value_t1>
    concept _requality_comparable_with = requires(const value_t0 v0, const value_t1 v1)
    {
        { v0.is_eq(v1) } -> rsame_as<bool>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `value_t0` and `value_t1` are `equality_comparable`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t0, typename value_t1>
    concept requality_comparable_with = _requality_comparable_with<value_t0, value_t1> or
        requires(const value_t0 v0, const value_t1 v1)
    {
        { v0 == v1 } -> rsame_as<bool>;
        { v0 != v1 } -> rsame_as<bool>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `equality_comparable`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept requality_comparable = requality_comparable_with<value_t, value_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `value_t0` and `value_t1` are `comparable`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t0, typename value_t1>
    concept _rcomparable_with = requires(const value_t0 v0, const value_t1 v1)
    {
        requires _requality_comparable_with<value_t0, value_t1>;

        { v0.is_lt(v1) } -> rsame_as<bool>;
        { v0.is_gt(v1) } -> rsame_as<bool>;
        { v0.is_le(v1) } -> rsame_as<bool>;
        { v0.is_ge(v1) } -> rsame_as<bool>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `value_t0` and `value_t1` are `comparable`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t0, typename value_t1>
    concept rcomparable_with = _rcomparable_with<value_t0, value_t1> or
        requires(const value_t0 v0, const value_t1 v1)
    {
        requires requality_comparable_with<value_t0, value_t1>;

        { v0 < v1 } -> rsame_as<bool>;
        { v0 > v1 } -> rsame_as<bool>;
        { v0 <= v1 } -> rsame_as<bool>;
        { v0 >= v1 } -> rsame_as<bool>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `comparable`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept rcomparable = rcomparable_with<value_t, value_t>;

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept rpolymorphic = std::is_polymorphic_v<value_t>;

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept rpure = not std::is_const_v<value_t> and not std::is_volatile_v<value_t>
        and not std::is_reference_v<value_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is {semi_regular}.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    concept rsemi_regular = rcopyable<value_t> && rdefault_initializable<value_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is {regular}.
    /// --------------------------------------------------------------------------------------------
    template <class value_t>
    concept regular = rsemi_regular<value_t> && requality_comparable<value_t>;
}

// clang-format on

namespace atom
{
    template <typename value_t0, typename value_t1>
    constexpr auto operator==(const value_t0& v0, const value_t1& v1) -> bool
        requires _requality_comparable_with<value_t0, value_t1>
    {
        return v0.is_eq(v1);
    }

    template <typename value_t0, typename value_t1>
    constexpr auto operator!=(const value_t0& v0, const value_t1& v1) -> bool
        requires _requality_comparable_with<value_t0, value_t1>
    {
        return not v0.is_eq(v1);
    }

    template <typename value_t0, typename value_t1>
    constexpr auto operator<(const value_t0& v0, const value_t1& v1) -> bool
        requires _rcomparable_with<value_t0, value_t1>
    {
        return v0.is_lt(v1);
    }

    template <typename value_t0, typename value_t1>
    constexpr auto operator>(const value_t0& v0, const value_t1& v1) -> bool
        requires _rcomparable_with<value_t0, value_t1>
    {
        return v0.is_gt(v1);
    }

    template <typename value_t0, typename value_t1>
    constexpr auto operator<=(const value_t0& v0, const value_t1& v1) -> bool
        requires _rcomparable_with<value_t0, value_t1>
    {
        return v0.is_le(v1);
    }

    template <typename value_t0, typename value_t1>
    constexpr auto operator>=(const value_t0& v0, const value_t1& v1) -> bool

        requires _rcomparable_with<value_t0, value_t1>
    {
        return v0.is_ge(v1);
    }
}
