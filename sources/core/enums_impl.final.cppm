export module atom.core:core.enums_impl.final;

import std;
import magic_enum;
import :core.core;
import :core.nums;
import :core.tuple;
import :core.option;
import :strings.string_view;
import :containers.array_view;

namespace atom
{
    template <typename enum_type, bool is_flags>
    constexpr auto _enums_impl<enum_type, is_flags>::from_string(
        string_view str) -> option<enum_type>
    {
        magic_optional_type<enum_type> result = [&]
        {
            if constexpr (is_flags())
                return magic_enum::enum_flags_cast<enum_type>(str.to_std());
            else
                magic_enum::enum_cast<enum_type>(str.to_std());
        }();

        if (not result.has_value())
            return { create_from_null };

        return result.value();
    }

    template <typename enum_type, bool is_flags>
    template <typename comparer_type>
    constexpr auto _enums_impl<enum_type, is_flags>::from_string(
        string_view str, comparer_type&& comparer) -> option<enum_type>
    {
        magic_optional_type<enum_type> result = [&]
        {
            if constexpr (is_flags())
                return magic_enum::enum_flags_cast<enum_type>(
                    str.to_std(), forward<comparer_type>(comparer));
            else
                magic_enum::enum_cast<enum_type>(str.to_std(), forward<comparer_type>(comparer));
        }();

        if (not result.has_value())
            return { create_from_null };

        return result.value();
    }

    template <typename enum_type, bool is_flags>
    consteval auto _enums_impl<enum_type, is_flags>::get_type_name() -> string_view
    {
        return string_view::from_std(magic_enum::enum_type_name<enum_type>());
    }

    template <typename enum_type, bool is_flags>
    consteval auto _enums_impl<enum_type, is_flags>::get_values() -> array_view<enum_type>
    {
        auto values = magic_enum::enum_values<enum_type>();
        return array_view<enum_type>(values.data(), values.size());
    }

    template <typename enum_type, bool is_flags>
    consteval auto _enums_impl<enum_type, is_flags>::get_strings() -> array_view<string_view>
    {
        magic_array_type<magic_string_view_type, get_count()> magic_strings =
            magic_enum::enum_names<enum_type>();

        magic_array_type<string_view, get_count()> strings;
        for (usize i = 0; i < get_count(); i++)
        {
            strings[i] = string_view::from_std(magic_strings[i]);
        }

        return array_view<string_view>(strings.data(), strings.size());
    }

    template <typename enum_type, bool is_flags>
    consteval auto _enums_impl<enum_type, is_flags>::get_entries()
        -> array_view<tuple<enum_type, string_view>>
    {
        magic_array_type<magic_pair_type<enum_type, magic_string_view_type>, get_count()>
            magic_strings = magic_enum::enum_names<enum_type>();

        magic_array_type<tuple<enum_type, string_view>, get_count()> strings;
        for (usize i = 0; i < get_count(); i++)
        {
            strings[i] = tuple<enum_type, string_view>(
                magic_strings[i].first, string_view::from_std(magic_strings[i].second));
        }

        return array_view<tuple<enum_type, string_view>>(strings.data(), strings.size());
    }

    template <typename enum_type, bool is_flags>
    constexpr auto _enums_impl<enum_type, is_flags>::to_string_view(enum_type value) -> string_view
    {
        return string_view::from_std(
            [value]
            {
                if constexpr (is_flags())
                {
                    return magic_enum::enum_flags_name<enum_type>(value);
                }
                else
                {
                    return magic_enum::enum_name<enum_type>(value);
                }
            }());
    }

}
