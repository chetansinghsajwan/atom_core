export module atom_core:core.enums.final;

import :core.enums_impl;
import :core.enums_impl.final;
import :core.enums;
import :core.option;
import :strings.string_view;
import :containers.array_view;

namespace atom::enums
{
    template <typename enum_type>
    constexpr auto from_string(string_view str) -> option<enum_type>
        requires is_enum<enum_type>
    {
        return _enums_impl<enum_type, is_flags<enum_type>>::from_string(str);
    }

    template <typename enum_type, typename comparer_type>
    constexpr auto from_string(string_view str, comparer_type&& comparer) -> option<enum_type>
        requires is_enum<enum_type>
    {
        return _enums_impl<enum_type, is_flags<enum_type>>::from_string(
            str, forward<comparer_type>(comparer));
    }

    template <typename enum_type>
    consteval auto get_type_name() -> string_view
        requires is_enum<enum_type>
    {
        return _enums_impl<enum_type, is_flags<enum_type>>::get_type_name();
    }

    template <typename enum_type>
    consteval auto get_strings() -> array_view<string_view>
        requires is_enum<enum_type>
    {
        return _enums_impl<enum_type, is_flags<enum_type>>::get_strings();
    }

    template <typename enum_type>
    constexpr auto to_string_view(enum_type value) -> string_view
        requires is_enum<enum_type>
    {
        return _enums_impl<enum_type, is_flags<enum_type>>::to_string_view(value);
    }
}
