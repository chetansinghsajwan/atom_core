#pragma once
#include "atom/core/lang_extensions.h"

// #include <concepts>
// #include <type_traits>

// clang-format off
namespace atom
{
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////
    //// basic concepts
    ////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    /// --------------------------------------------------------------------------------------------
    /// ensures `t0` is same as `t1`.
    /// --------------------------------------------------------------------------------------------
    template <typename t0, typename t1>
    concept rsame_as = std::same_as<t0, t1>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `t0` is `void`.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    concept ris_void = rsame_as<type, void>;

    /// --------------------------------------------------------------------------------------------
    /// ensures unqualified type of `t0` is same as unqualified type of `t1`.
    /// --------------------------------------------------------------------------------------------
    template <typename t0, typename t1>
    concept rsame_as_unqualified = std::same_as<std::remove_cvref_t<t0>, std::remove_cvref_t<t1>>;

    /// --------------------------------------------------------------------------------------------
    /// enusres `type` is const-qualified.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    concept rconst = std::is_const_v<type>;

    /// --------------------------------------------------------------------------------------------
    /// enusres `type` is volatile-qualified.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    concept rvolatile = std::is_volatile_v<type>;

    /// --------------------------------------------------------------------------------------------
    /// enusres `type` is ref.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    concept rref = std::is_reference_v<type>;

    /// --------------------------------------------------------------------------------------------
    /// enusres `type` is l-value ref.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    concept rlvalue_ref = std::is_lvalue_reference_v<type>;

    /// --------------------------------------------------------------------------------------------
    /// enusres `type` is r-value ref.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    concept rrvalue_ref = std::is_rvalue_reference_v<type>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `tfrom` is `convertible` to `tto`.
    /// --------------------------------------------------------------------------------------------
    template <typename tfrom, typename tto>
    concept rconvertible_to = requires(tfrom from) { static_cast<tto>(from); };

    /// --------------------------------------------------------------------------------------------
    /// ensures `tderived` is derived from `tbase`.
    /// --------------------------------------------------------------------------------------------
    template <typename tderived, typename tbase>
    concept rderived_from =
        std::derived_from<std::remove_cvref_t<tderived>, std::remove_cvref_t<tbase>>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `tderived` not is derived from `tbase`.
    /// --------------------------------------------------------------------------------------------
    template <typename tderived, typename tbase>
    concept rnot_derived_from = (!rderived_from<tderived, tbase>);

    /// --------------------------------------------------------------------------------------------
    /// ensures `tderived` is same as or derived from `tbase`.
    /// --------------------------------------------------------------------------------------------
    template <typename tderived, typename tbase>
    concept rsame_or_derived_from = rsame_as<tderived, tbase> || rderived_from<tderived, tbase>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `tderived` is not same as or derived from `tbase`.
    /// --------------------------------------------------------------------------------------------
    template <typename tderived, typename tbase>
    concept rnot_same_or_derived_from = !rsame_or_derived_from<tderived, tbase>;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////
    //// object concepts
    ////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `constructible` using `args...`.
    /// --------------------------------------------------------------------------------------------
    template <typename type, typename... args_type>
    concept rconstructible = requires(args_type&&... args) { type(forward<args_type>(args)...); };

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `trivially_constructible` using `args...`.
    /// --------------------------------------------------------------------------------------------
    template <typename type, typename... args_type>
    concept rtrivially_constructible = std::is_trivially_constructible_v<type, args_type...>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `default_constructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    concept rdefault_constructible = rconstructible<type>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `trivially_default_constructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    concept rtrivially_default_constructible = rtrivially_constructible<type>;

    /// --------------------------------------------------------------------------------------------
    /// ensures each type in `ts...` is `default_constructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename... ts>
    concept rdefault_constructible_all = (rdefault_constructible<ts> and ...);

    /// --------------------------------------------------------------------------------------------
    /// ensures each type in `ts...` is `trivially_default_constructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename... ts>
    concept rtrivially_default_constructible_all = (rtrivially_default_constructible<ts> and ...);

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `copy_constructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    concept rcopy_constructible = rconstructible<type, const type&>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `trivially_copy_constructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    concept rtrivially_copy_constructible = rtrivially_constructible<type, const type&>;

    /// --------------------------------------------------------------------------------------------
    /// ensures each type in `ts...` is `copy_constructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename... ts>
    concept rcopy_constructible_all = (rcopy_constructible<ts> and ...);

    /// --------------------------------------------------------------------------------------------
    /// ensures each type in `ts...` is `trivially_copy_constructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename... ts>
    concept rtrivially_copy_constructible_all = (rtrivially_copy_constructible<ts> and ...);

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `move_constructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    concept rmove_constructible = rconstructible<type, type&&>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `trivially_move_constructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    concept rtrivially_move_constructible = rtrivially_constructible<type, type&&>;

    /// --------------------------------------------------------------------------------------------
    /// ensures each type in `ts...` is `move_constructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename... ts>
    concept rmove_constructible_all = (rmove_constructible<ts> and ...);

    /// --------------------------------------------------------------------------------------------
    /// ensures each type in `ts...` is `trivially_move_constructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename... ts>
    concept rtrivially_move_constructible_all = (rtrivially_move_constructible<ts> and ...);

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `default_initializable`.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    concept rdefault_initializable = requires {
        requires rdefault_constructible<type>;

        (void)new type;
        type{};
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures each type in `ts...` is `default_initializable`.
    /// --------------------------------------------------------------------------------------------
    template <typename... ts>
    concept rdefault_initializable_all = (rdefault_initializable<ts> and ...);

    /// --------------------------------------------------------------------------------------------
    /// ensures `t0` is `assignable` using `t1`.
    /// --------------------------------------------------------------------------------------------
    template <typename t0, typename t1>
    concept rassignable = requires(t0 v0, t1 v1)
    {
        { v0 = forward<t1>(v1) } -> rsame_as<t0&>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `trivially_assignable` using `from`.
    /// --------------------------------------------------------------------------------------------
    template <typename t0, typename t1>
    concept rtrivially_assignable = std::is_trivially_assignable_v<t0, t1>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `copy_assignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    concept rcopy_assignable = rassignable<type, const type&>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `trivially_copy_assignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    concept rtrivially_copy_assignable = rtrivially_assignable<type&, const type&>;

    /// --------------------------------------------------------------------------------------------
    /// ensures each type in `ts...` is `copy_assignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename... ts>
    concept rcopy_assignable_all = (rcopy_assignable<ts> and ...);

    /// --------------------------------------------------------------------------------------------
    /// ensures each type in `ts...` is `trivially_copy_assignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename... ts>
    concept rtrivially_copy_assignable_all = (rtrivially_copy_assignable<ts> and ...);

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `move_assignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    concept rmove_assignable = rassignable<type, type&&>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `trivially_move_assignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    concept rtrivially_move_assignable = rtrivially_assignable<type&, type&&>;

    /// --------------------------------------------------------------------------------------------
    /// ensures each type in `ts...` is `move_assignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename... ts>
    concept rmove_assignable_all = (rmove_assignable<ts> and ...);

    /// --------------------------------------------------------------------------------------------
    /// ensures each type in `ts...` is `trivially_move_assignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename... ts>
    concept rtrivially_move_assignable_all = (rtrivially_move_assignable<ts> and ...);

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is``copy_constructible` and `copy_assignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    concept rcopyable = rcopy_constructible<type> && rcopy_assignable<type>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is``copy_constructible` and `trivially_copy_assignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    concept rtrivially_copyable = rtrivially_copy_constructible<type> && rtrivially_copy_assignable<type>;

    /// --------------------------------------------------------------------------------------------
    /// ensures each type in `ts...` is `copyable`.
    /// --------------------------------------------------------------------------------------------
    template <typename... ts>
    concept rcopyable_all = (rcopyable<ts> and ...);

    /// --------------------------------------------------------------------------------------------
    /// ensures each type in `ts...` is `trivially_copyable`.
    /// --------------------------------------------------------------------------------------------
    template <typename... ts>
    concept rtrivially_copyable_all = (rtrivially_copyable<ts> and ...);

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `move_constructible` and `move_assignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    concept rmoveable = rmove_constructible<type> && rmove_assignable<type>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `move_constructible` and `trivially_move_assignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    concept rtrivially_moveable = rtrivially_move_constructible<type> && rtrivially_move_assignable<type>;

    /// --------------------------------------------------------------------------------------------
    /// ensures each type in `ts...` is `moveable`.
    /// --------------------------------------------------------------------------------------------
    template <typename... ts>
    concept rmoveable_all = (rmoveable<ts> and ...);

    /// --------------------------------------------------------------------------------------------
    /// ensures each type in `ts...` is `trivially_moveable`.
    /// --------------------------------------------------------------------------------------------
    template <typename... ts>
    concept rtrivially_moveable_all = (rtrivially_moveable<ts> and ...);

    /// --------------------------------------------------------------------------------------------
    /// ensures `t0` is `swappable` with `t1`.
    /// --------------------------------------------------------------------------------------------
    template <typename t0, typename t1>
    concept rswappable_with = rassignable<t0, t1> && rassignable<t1, t0>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `t0` is `trivially_swappable` with `t2`.
    /// --------------------------------------------------------------------------------------------
    template <typename t0, typename t1>
    concept rtrivially_swappable_with = rtrivially_assignable<t0, t1> && rtrivially_assignable<t1, t0>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `swappable` with itself.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    concept rswappable = rswappable_with<type, type>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `trivially_swappable` with itself.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    concept rtrivially_swappable = rtrivially_swappable_with<type, type>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `destructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    concept rdestructible = std::destructible<type>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `trivally_destructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    concept rtrivially_destructible = std::is_trivially_destructible_v<type>;

    /// --------------------------------------------------------------------------------------------
    /// ensures each type in `ts...` is `destructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename... ts>
    concept rdestructible_all = (rdestructible<ts> and ...);

    /// --------------------------------------------------------------------------------------------
    /// ensures each type in `ts...` is `trivally_destructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename... ts>
    concept rtrivially_destructible_all = (rtrivially_destructible<ts> and ...);

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////
    //// comparision concepts.
    ////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    /// --------------------------------------------------------------------------------------------
    /// ensures `t0` and `t1` are `equality_comparable`.
    /// --------------------------------------------------------------------------------------------
    template <typename t0, typename t1>
    concept _requality_comparable_with = requires(const t0 v0, const t1 v1)
    {
        { v0 == v1 } -> rsame_as<bool>;
        { v0 != v1 } -> rsame_as<bool>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `t0` and `t1` are `equality_comparable`.
    /// --------------------------------------------------------------------------------------------
    template <typename t0, typename t1>
    concept requality_comparable_with = requires(const t0 v0, const t1 v1)
    {
        { v0.eq(v1) } -> rsame_as<bool>;
        { v0.ne(v1) } -> rsame_as<bool>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `equality_comparable`.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    concept requality_comparable = requality_comparable_with<type, type>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `t0` and `t1` are `comparable`.
    /// --------------------------------------------------------------------------------------------
    template <typename t0, typename t1>
    concept _rcomparable_with = requires(const t0 v0, const t1 v1)
    {
        requires _requality_comparable_with<t0, t1>;

        { v0 < v1 } -> rsame_as<bool>;
        { v0 > v1 } -> rsame_as<bool>;
        { v0 <= v1 } -> rsame_as<bool>;
        { v0 >= v1 } -> rsame_as<bool>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `t0` and `t1` are `comparable`.
    /// --------------------------------------------------------------------------------------------
    template <typename t0, typename t1>
    concept rcomparable_with = requires(const t0 v0, const t1 v1)
    {
        requires requality_comparable_with<t0, t1>;

        { v0.lt(v1) } -> rsame_as<bool>;
        { v0.gt(v1) } -> rsame_as<bool>;
        { v0.le(v1) } -> rsame_as<bool>;
        { v0.ge(v1) } -> rsame_as<bool>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `comparable`.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    concept rcomparable = rcomparable_with<type, type>;

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    concept rpolymorphic = std::is_polymorphic_v<type>;

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    concept rpure = not std::is_const_v<type> and not std::is_volatile_v<type> and not std::is_reference_v<type>;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////
    //// type concepts
    ////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is {semi_regular}.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    concept rsemi_regular = rcopyable<type> && rdefault_initializable<type>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is {regular}.
    /// --------------------------------------------------------------------------------------------
    template <class type>
    concept regular = rsemi_regular<type> && requality_comparable<type>;
}

namespace atom
{
    template <typename t0, typename t1>
    constexpr auto operator==(const t0& v0, const t1& v1) -> bool
        requires requality_comparable_with<t0, t1>
    {
        return v0.eq(v1);
    }

    template <typename t0, typename t1>
    constexpr auto operator!=(const t0& v0, const t1& v1) -> bool
        requires requality_comparable_with<t0, t1>
    {
        return v0.ne(v1);
    }

    template <typename t0, typename t1>
    constexpr auto operator<(const t0& v0, const t1& v1) -> bool
        requires rcomparable_with<t0, t1>
    {
        return v0.lt(v1);
    }

    template <typename t0, typename t1>
    constexpr auto operator>(const t0& v0, const t1& v1) -> bool
        requires rcomparable_with<t0, t1>
    {
        return v0.gt(v1);
    }

    template <typename t0, typename t1>
    constexpr auto operator<=(const t0& v0, const t1& v1) -> bool
        requires rcomparable_with<t0, t1>
    {
        return v0.le(v1);
    }

    template <typename t0, typename t1>
    constexpr auto operator>=(const t0& v0, const t1& v1) -> bool
    
        requires rcomparable_with<t0, t1>
    {
        return v0.ge(v1);
    }
}