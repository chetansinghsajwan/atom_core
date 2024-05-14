#pragma once
#include "atom/core/string/string_view.h"
#include "atom/core/core/tuple.h"
#include "atom/core/core/option.h"
#include "atom/core/math.h"
// #include "magic_enum_all.hpp"
// #include <array>
// #include <utility>

namespace atom
{
    template <typename enum_t>
    constexpr auto _enum_impl_is_flags() -> bool;

    template <typename enum_t, bool in_is_flags>
    class _enums_impl
    {
        using magic_string_t = magic_enum::string;
        using magic_string_view_t = magic_enum::string_view;

        template <typename value_t>
        using magic_optional_t = magic_enum::optional<value_t>;

        template <typename first_value_t, typename second_value_t>
        using magic_pair_t = std::pair<first_value_t, second_value_t>;

        template <typename value_t, usize count>
        using magic_array_t = std::array<value_t, count>;

    public:
        using underlying_t = magic_enum::underlying_type_t<enum_t>;

        static consteval auto is_flags() -> bool
        {
            return in_is_flags;
        }

        static consteval auto is_enum() -> bool
        {
            return typeinfo<enum_t>::is_enum;
        }

        static consteval auto is_scoped() -> bool
        {
            return magic_enum::is_scoped_enum_v<enum_t>;
        }

        static constexpr auto from_string(string_view str) -> option<enum_t>
        {
            magic_optional_t<enum_t> result = [&] {
                if constexpr (is_flags())
                    return magic_enum::enum_flags_cast<enum_t>(str.to_std());
                else
                    magic_enum::enum_cast<enum_t>(str.to_std());
            }();

            if (not result.has_value())
                return nullopt();

            return string_view::from_std(result.value());
        }

        template <typename comparer_t>
        static constexpr auto from_string(string_view str, comparer_t&& comparer) -> option<enum_t>
        {
            magic_optional_t<enum_t> result = [&] {
                if constexpr (is_flags())
                    return magic_enum::enum_flags_cast<enum_t>(
                        str.to_std(), forward<comparer_t>(comparer));
                else
                    magic_enum::enum_cast<enum_t>(str.to_std(), forward<comparer_t>(comparer));
            }();

            if (not result.has_value())
                return nullopt();

            return string_view::from_std(result.value());
        }

        static constexpr auto from_underlying_unchecked(underlying_t value) -> enum_t
        {
            return enum_t(value);
        }

        static constexpr auto from_underlying_try(underlying_t value) -> option<enum_t>
        {
            magic_optional_t<enum_t> value_opt = [&] {
                if constexpr (is_flags())
                    return magic_enum::enum_flags_cast<enum_t>(value);
                else
                    magic_enum::enum_cast<enum_t>(value);
            }();

            if (not value_opt.has_value())
                return nullopt();

            return value_opt.value();
        }

        static constexpr auto from_index_unchecked(usize index) -> enum_t
        {
            return magic_enum::enum_value<enum_t>(index);
        }

        static constexpr auto from_index_try(usize index) -> option<enum_t>
        {
            if (index >= get_count())
                return nullopt();

            return magic_enum::enum_value<enum_t>(index);
        }

        static consteval auto get_type_name() -> string_view
        {
            return string_view::from_std(magic_enum::enum_type_name<enum_t>());
        }

        static consteval auto get_count() -> usize
        {
            return magic_enum::enum_count<enum_t>();
        }

        static consteval auto get_values() -> array_view<enum_t>
        {
            auto values = magic_enum::enum_values<enum_t>();
            return array_view<enum_t>(values.data(), values.size());
        }

        static consteval auto get_strings() -> array_view<string_view>
        {
            magic_array_t<magic_string_view_t, get_count()> magic_strings =
                magic_enum::enum_names<enum_t>();

            magic_array_t<string_view, get_count()> strings;
            for (usize i = 0; i < get_count(); i++)
            {
                strings[i] = string_view::from_std(magic_strings[i]);
            }

            return array_view<string_view>(strings.data(), strings.size());
        }

        static consteval auto get_entries() -> array_view<tuple<enum_t, string_view>>
        {
            magic_array_t<magic_pair_t<enum_t, magic_string_view_t>, get_count()> magic_strings =
                magic_enum::enum_names<enum_t>();

            magic_array_t<tuple<enum_t, string_view>, get_count()> strings;
            for (usize i = 0; i < get_count(); i++)
            {
                strings[i] = tuple<enum_t, string_view>(
                    magic_strings[i].first, string_view::from_std(magic_strings[i].second));
            }

            return array_view<tuple<enum_t, string_view>>(strings.data(), strings.size());
        }

        static constexpr auto is_value_valid(enum_t value) -> bool
        {
            if constexpr (is_flags())
                return magic_enum::enum_flags_contains(value);
            else
                return magic_enum::enum_contains(value);
        }

        static constexpr auto is_index_valid(usize index) -> bool
        {
            return index < get_count();
        }

        static constexpr auto is_underlying_valid(underlying_t value) -> bool
        {
            if constexpr (is_flags())
                return magic_enum::enum_flags_contains(value);
            else
                return magic_enum::enum_contains(value);
        }

        static constexpr auto to_index(enum_t value) -> usize
        {
            return magic_enum::enum_index(value).value_or(math::max<usize>());
        }

        static constexpr auto to_underlying(enum_t value) -> underlying_t
        {
            return underlying_t(value);
        }

        static constexpr auto to_string_view(enum_t value) -> string_view
        {
            return string_view::from_std([value] {
                if constexpr (is_flags())
                {
                    return magic_enum::enum_flags_name<enum_t>(value);
                }
                else
                {
                    return magic_enum::enum_name<enum_t>(value);
                }
            }());
        }

        /// @todo implement this.
        static consteval auto get_min() -> enum_t {}

        /// @todo implement this.
        static consteval auto get_max() -> enum_t {}

        /// @todo implement this. this is just a workaround.
        static consteval auto get_all_flags() -> enum_t
        {
            return enum_t(math::max<underlying_t>());
        }

        static constexpr auto add_flags(enum_t lhs, enum_t rhs) -> enum_t
        {
            return enum_t(underlying_t(lhs) | underlying_t(rhs));
        }

        static constexpr auto remove_flags(enum_t lhs, enum_t rhs) -> enum_t
        {
            return enum_t(underlying_t(lhs) & ~underlying_t(rhs));
        }

        static constexpr auto get_common_flags(enum_t lhs, enum_t rhs) -> enum_t
        {
            return enum_t(underlying_t(lhs) & underlying_t(rhs));
        }

        static constexpr auto get_uncommon_flags(enum_t lhs, enum_t rhs) -> enum_t
        {
            underlying_t union_flags = underlying_t(lhs) | underlying_t(rhs);
            underlying_t intersection_flags = underlying_t(lhs) & underlying_t(rhs);
            return enum_t(union_flags & intersection_flags);
        }

        static constexpr auto get_reverse_flags(enum_t flags) -> enum_t
        {
            return enum_t(underlying_t(get_all_flags()) & ~underlying_t(flags));
        }

        static constexpr auto has_all_flags(enum_t lhs, enum_t rhs) -> bool
        {
            return (underlying_t(lhs) & underlying_t(rhs)) == underlying_t(lhs);
        }

        static constexpr auto has_any_flags(enum_t lhs, enum_t rhs) -> bool
        {
            return (underlying_t(lhs) & underlying_t(rhs)) != 0;
        }
    };
}

template <typename enum_t>
    requires(atom::_enums_impl<enum_t, true>::is_enum())
struct magic_enum::customize::enum_range<enum_t>
{
    static constexpr bool is_flags = true;
};
