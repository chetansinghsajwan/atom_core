#pragma once
#include "atom/core/types/typeinfo.h"
#include "atom/core/types/typelist.h"

namespace atom
{
    template <typename... values_t>
    class typeinfo_list: public typeinfo_list<type_list<values_t...>>
    {};

    template <typename... values_t>
    class typeinfo_list<type_list<values_t...>>
    {
    public:
        using value_types_list = type_list<values_t...>;

    public:
        static constexpr bool are_pure =
            value_types_list::are_all([](auto info) { return info.is_pure; });

        static constexpr bool are_default_constructible =
            value_types_list::are_all([](auto info) { return info.is_default_constructible; });

        static constexpr bool are_copy_constructible =
            value_types_list::are_all([](auto info) { return info.is_copy_constructible; });

        static constexpr bool are_copy_assignable =
            value_types_list::are_all([](auto info) { return info.is_copy_assignable; });

        static constexpr bool are_copyable =
            value_types_list::are_all([](auto info) { return info.is_copyable; });

        static constexpr bool are_move_constructible =
            value_types_list::are_all([](auto info) { return info.is_move_constructible; });

        static constexpr bool are_move_assignable =
            value_types_list::are_all([](auto info) { return info.is_move_assignable; });

        static constexpr bool are_moveable =
            value_types_list::are_all([](auto info) { return info.is_moveable; });

        static constexpr bool are_destructible =
            value_types_list::are_all([](auto info) { return info.is_destructible; });

        static constexpr bool are_trivially_default_constructible = value_types_list::are_all(
            [](auto info) { return info.is_trivially_default_constructible; });

        static constexpr bool are_trivially_copy_constructible = value_types_list::are_all(
            [](auto info) { return info.is_trivially_copy_constructible; });

        static constexpr bool are_trivially_copy_assignable =
            value_types_list::are_all([](auto info) { return info.is_trivially_copy_assignable; });

        static constexpr bool are_not_trivially_copy_assignable = not are_trivially_copy_assignable;

        static constexpr bool are_trivially_copyable =
            value_types_list::are_all([](auto info) { return info.is_trivially_copyable; });

        static constexpr bool are_trivially_move_constructible = value_types_list::are_all(
            [](auto info) { return info.is_trivially_move_constructible; });

        static constexpr bool are_not_trivially_move_constructible =
            not are_trivially_move_constructible;

        static constexpr bool are_trivially_move_assignable =
            value_types_list::are_all([](auto info) { return info.is_trivially_move_assignable; });

        static constexpr bool are_not_trivially_move_assignable = not are_trivially_move_assignable;

        static constexpr bool are_trivially_moveable =
            value_types_list::are_all([](auto info) { return info.is_trivially_moveable; });

        static constexpr bool are_trivially_destructible =
            value_types_list::are_all([](auto info) { return info.is_trivially_destructible; });
    };
}
