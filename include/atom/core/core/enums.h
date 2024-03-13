#pragma once
#include "_enums_impl.h"

namespace atom::enums
{
    /// --------------------------------------------------------------------------------------------
    /// returns the underlying data type used for storage of enum, if the type is not enum,
    /// compiler error occurs.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_type>
    using get_underlying_type = _enums_impl<enum_type>::underlying_t;

    /// --------------------------------------------------------------------------------------------
    /// returns `true` if `enum_type` is an enum.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_type>
    constexpr bool is_enum = _enums_impl<enum_type>::is_enum();

    /// --------------------------------------------------------------------------------------------
    /// returns `true` if enum is a scoped enum.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_type>
    constexpr bool is_scoped = _enums_impl<enum_type>::is_scoped();

    /// --------------------------------------------------------------------------------------------
    /// returns true if enum type is flags.
    ///
    /// @note this always returns false, specialize this function for to return `true` for flag
    /// enums.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_type>
    constexpr bool is_flags = false;

    /// --------------------------------------------------------------------------------------------
    /// returns first enum value of type `enum_type` matching string `str`.
    ///
    /// @example value from string.
    /// ```
    /// enum class example
    /// {
    ///     value1,
    ///     value2,
    ///     value3,
    /// };
    ///
    /// example value = enums::from_string<example>("value1");
    /// ```
    ///
    /// @example value from flags.
    /// ```
    /// enum class flags
    /// {
    ///     value1 = 1 << 0,
    ///     value2 = 1 << 1,
    ///     value3 = 1 << 2,
    /// };
    ///
    /// flags value = enums::from_string<flags>("value1|value2");
    /// ```
    /// --------------------------------------------------------------------------------------------
    template <typename enum_type>
    constexpr auto from_string(string_view str) -> option<enum_type>
        requires is_enum<enum_type>
    {
        return _enums_impl<enum_type>::from_string(str);
    }

    /// --------------------------------------------------------------------------------------------
    /// returns first enum value of type `enum_type` matching string `str`. same as above, except
    /// `comparer` is used to match the string.
    ///
    /// @param comparer used to match string `str` with enum values.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_type, typename comparer_type>
    constexpr auto from_string(string_view str, comparer_type&& comparer) -> option<enum_type>
        requires is_enum<enum_type>
    {
        return _enums_impl<enum_type>::from_string(str, ATOM_FORWARD(comparer));
    }

    /// --------------------------------------------------------------------------------------------
    /// returns enum value of type `enum_type` created from underlying type.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_type>
    constexpr auto from_underlying_unchecked(get_underlying_type<enum_type> value) -> enum_type
        requires is_enum<enum_type>
    {
        return _enums_impl<enum_type>::from_underlying_unchecked(value);
    }

    /// --------------------------------------------------------------------------------------------
    /// returns enum value of type `enum_type` created from underlying type.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_type>
    constexpr auto from_underlying_try(get_underlying_type<enum_type> value) -> option<enum_type>
        requires is_enum<enum_type>
    {
        return _enums_impl<enum_type>::from_underlying_try(value);
    }

    /// --------------------------------------------------------------------------------------------
    /// returns enum value at index `index`.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_type>
    constexpr auto from_index_unchecked(usize index) -> enum_type
        requires is_enum<enum_type>
    {
        return _enums_impl<enum_type>::from_index_unchecked(index);
    }

    /// --------------------------------------------------------------------------------------------
    /// returns enum value at index `index`.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_type>
    constexpr auto from_index_try(usize index) -> option<enum_type>
        requires is_enum<enum_type>
    {
        return _enums_impl<enum_type>::from_index_try(index);
    }

    /// --------------------------------------------------------------------------------------------
    /// returns the name of the `enum_type`.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_type>
    consteval auto get_type_name() -> string_view
        requires is_enum<enum_type>
    {
        return _enums_impl<enum_type>::get_type_name();
    }

    /// --------------------------------------------------------------------------------------------
    /// returns the count of enum values present in `enum_type`.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_type>
    consteval auto get_count() -> usize
        requires is_enum<enum_type>
    {
        return _enums_impl<enum_type>::get_count();
    }

    /// --------------------------------------------------------------------------------------------
    /// returns an `array_view` of values.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_type>
    consteval auto get_values() -> array_view<enum_type>
        requires is_enum<enum_type>
    {
        return _enums_impl<enum_type>::get_values();
    }

    /// --------------------------------------------------------------------------------------------
    /// returns an `array_view` of string representation of enum values.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_type>
    consteval auto get_strings() -> array_view<string_view>
        requires is_enum<enum_type>
    {
        return _enums_impl<enum_type>::get_strings();
    }

    /// --------------------------------------------------------------------------------------------
    /// returns an `array_view` of pairs of enum values and their string representations.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_type>
    consteval auto get_entries() -> array_view<tuple<enum_type, string_view>>
        requires is_enum<enum_type>
    {
        return _enums_impl<enum_type>::get_entries();
    }

    /// --------------------------------------------------------------------------------------------
    /// returns `true` if value `value` exists.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_type>
    constexpr auto is_value_valid(enum_type value) -> bool
        requires is_enum<enum_type>
    {
        return _enums_impl<enum_type>::is_value_valid(value);
    }

    /// --------------------------------------------------------------------------------------------
    /// returns `true` if a value at index `index` exists.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_type>
    constexpr auto is_index_valid(usize index) -> bool
        requires is_enum<enum_type>
    {
        return _enums_impl<enum_type>::is_index_valid(index);
    }

    /// --------------------------------------------------------------------------------------------
    ///  returns `true` if a value whose underlying representation is `value` exists.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_type>
    constexpr auto is_underlying_valid(get_underlying_type<enum_type> value) -> bool
        requires is_enum<enum_type>
    {
        return _enums_impl<enum_type>::is_underlying_valid(value);
    }

    /// --------------------------------------------------------------------------------------------
    /// returns the index of the enum value if valid, else max.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_type>
    constexpr auto to_index(enum_type value) -> usize
        requires is_enum<enum_type>
    {
        return _enums_impl<enum_type>::to_index(value);
    }

    /// --------------------------------------------------------------------------------------------
    /// returns the underlying representation of enum value.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_type>
    constexpr auto to_underlying(enum_type value) -> get_underlying_type<enum_type>
        requires is_enum<enum_type>
    {
        return _enums_impl<enum_type>::to_underlying(value);
    }

    /// --------------------------------------------------------------------------------------------
    /// returns the null terminated string representation of enum value if valid, else empty.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_type>
    constexpr auto to_string_view(enum_type value) -> string_view
        requires is_enum<enum_type>
    {
        return _enums_impl<enum_type>::to_string_view(value);
    }

    /// --------------------------------------------------------------------------------------------
    /// returns the enum value which has the minimum underlying type value. in case of flags,
    /// returns zero.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_type>
    consteval auto get_min() -> enum_type
        requires is_enum<enum_type>
    {
        return _enums_impl<enum_type>::get_min();
    }

    /// --------------------------------------------------------------------------------------------
    /// returns the enum value which has the maximum underlying type. in case of flags, returns
    /// enum value having all flags on.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_type>
    consteval auto get_max() -> enum_type
        requires is_enum<enum_type>
    {
        return _enums_impl<enum_type>::get_max();
    }

    /// --------------------------------------------------------------------------------------------
    /// adds the rhs flags into `lhs` flags and returns the result.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_type>
    constexpr auto add_flags(enum_type lhs, enum_type rhs) -> enum_type
        requires is_enum<enum_type> and is_flags<enum_type>
    {
        return _enums_impl<enum_type>::add_flags(lhs, rhs);
    }

    /// --------------------------------------------------------------------------------------------
    /// removes the rhs flags from `lhs` flags and returns the result.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_type>
    constexpr auto remove_flags(enum_type lhs, enum_type rhs) -> enum_type
        requires is_enum<enum_type> and is_flags<enum_type>
    {
        return _enums_impl<enum_type>::remove_flags(lhs, rhs);
    }

    /// --------------------------------------------------------------------------------------------
    /// returns only the flags present in both `lhs` and `rhs`.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_type>
    constexpr auto get_common_flags(enum_type lhs, enum_type rhs) -> enum_type
        requires is_enum<enum_type> and is_flags<enum_type>
    {
        return _enums_impl<enum_type>::get_common_flags(lhs, rhs);
    }

    /// --------------------------------------------------------------------------------------------
    /// adds `lhs` and `rhs` and removes the flags that are common in both.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_type>
    constexpr auto get_uncommon_flags(enum_type lhs, enum_type rhs) -> enum_type
        requires is_enum<enum_type> and is_flags<enum_type>
    {
        return _enums_impl<enum_type>::get_uncommon_flags(lhs, rhs);
    }

    /// --------------------------------------------------------------------------------------------
    /// returns all the flags not present in both `flags`.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_type>
    constexpr auto get_reverse_flags(enum_type flags) -> enum_type
        requires is_enum<enum_type> and is_flags<enum_type>
    {
        return _enums_impl<enum_type>::get_reverse_flags(flags);
    }

    /// --------------------------------------------------------------------------------------------
    /// returns `true` if `lhs` has all the `rhs` flags.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_type>
    constexpr auto has_all_flags(enum_type lhs, enum_type rhs) -> bool
        requires is_enum<enum_type> and is_flags<enum_type>
    {
        return _enums_impl<enum_type>::has_all_flags(lhs, rhs);
    }

    /// --------------------------------------------------------------------------------------------
    /// returns `true` if `lhs` has any of the `rhs` flags.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_type>
    constexpr auto has_any_flags(enum_type lhs, enum_type rhs) -> bool
        requires is_enum<enum_type> and is_flags<enum_type>
    {
        return _enums_impl<enum_type>::has_any_flags(lhs, rhs);
    }
}

namespace atom
{
    template <typename enum_type>
    constexpr auto operator|(enum_type lhs, enum_type rhs) -> enum_type
        requires enums::is_enum<enum_type> and enums::is_flags<enum_type>
    {
        return _enums_impl<enum_type>::add_flags(lhs, rhs);
    }

    template <typename enum_type>
    constexpr auto operator&(enum_type lhs, enum_type rhs) -> enum_type
        requires enums::is_enum<enum_type> and enums::is_flags<enum_type>
    {
        return _enums_impl<enum_type>::get_common_flags(lhs, rhs);
    }

    template <typename enum_type>
    constexpr auto operator^(enum_type lhs, enum_type rhs) -> enum_type
        requires enums::is_enum<enum_type> and enums::is_flags<enum_type>
    {
        return _enums_impl<enum_type>::get_uncommon_flags(lhs, rhs);
    }
}
