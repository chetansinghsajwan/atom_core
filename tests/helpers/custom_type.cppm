export module atom.core.tests:custom_type;

import atom.core;

namespace atom::tests
{
    export enum class custom_type_flags: u64
    {
        none = 0,
        all = nums::get_max_u64(),

        default_constructor = 1 << 0,
        copy_constructor = 1 << 1,
        copy_operator = 1 << 2,
        move_constructor = 1 << 3,
        move_operator = 1 << 4,
        destructor = 1 << 5,

        trivial_default_constructor = 1 << 6,
        trivial_copy_constructor = 1 << 7,
        trivial_copy_operator = 1 << 8,
        trivial_move_constructor = 1 << 9,
        trivial_move_operator = 1 << 10,
        trivial_destructor = 1 << 11,

        trivial = 1 << 12,

        equal_to_operator = 1 << 13,
        less_than_to_operator = 1 << 14,
        greater_than_to_operator = 1 << 15,
        less_than_or_equal_to_operator = 1 << 16,
        greater_than_or_equal_to_operator = 1 << 17,
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    export template <custom_type_flags flags>
    class custom_type
    {
        using this_type = custom_type;
        using this_type_lvalue_ref = this_type&;
        using this_type_rvalue_ref = this_type&;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # trivial default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr custom_type()
            requires(enums::has_all_flags(flags, custom_type_flags::trivial_default_constructor))
        = default;

        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr custom_type()
            requires(
                enums::has_all_flags(flags, custom_type_flags::default_constructor)
                and not enums::has_all_flags(flags, custom_type_flags::trivial_default_constructor))
        {}

        /// ----------------------------------------------------------------------------------------
        /// # trivial copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr custom_type(const this_type& that)
            requires(enums::has_all_flags(flags, custom_type_flags::trivial_copy_constructor))
        = default;

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr custom_type(const this_type& that)
            requires(
                enums::has_all_flags(flags, custom_type_flags::copy_constructor)
                and not enums::has_all_flags(flags, custom_type_flags::trivial_copy_constructor))
        {}

        /// ----------------------------------------------------------------------------------------
        /// # trivial copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(const this_type& that) -> this_type_lvalue_ref
            requires(enums::has_all_flags(flags, custom_type_flags::trivial_copy_operator))
        = default;

        /// ----------------------------------------------------------------------------------------
        /// # copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(const this_type& that) -> this_type_lvalue_ref
            requires(enums::has_all_flags(flags, custom_type_flags::copy_operator))
                    and (not enums::has_all_flags(flags, custom_type_flags::trivial_copy_operator))
        {
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # trivial move constructor
        /// ----------------------------------------------------------------------------------------
        constexpr custom_type(this_type&& that)
            requires(enums::has_all_flags(flags, custom_type_flags::trivial_move_constructor))
        = default;

        /// ----------------------------------------------------------------------------------------
        /// # move constructor
        /// ----------------------------------------------------------------------------------------
        constexpr custom_type(this_type&& that)
            requires(
                enums::has_all_flags(flags, custom_type_flags::move_constructor)
                and not enums::has_all_flags(flags, custom_type_flags::trivial_move_constructor))
        {}

        /// ----------------------------------------------------------------------------------------
        /// # trivial move operator
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(this_type&& that) -> this_type_lvalue_ref
            requires(enums::has_all_flags(flags, custom_type_flags::trivial_move_operator))
        = default;

        /// ----------------------------------------------------------------------------------------
        /// # move operator
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(this_type&& that) -> this_type_lvalue_ref
            requires(enums::has_all_flags(flags, custom_type_flags::move_operator)
                     and not enums::has_all_flags(flags, custom_type_flags::trivial_move_operator))
        {
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # trivial destructor
        /// ----------------------------------------------------------------------------------------
        constexpr custom_type(this_type&& that)
            requires(enums::has_all_flags(flags, custom_type_flags::trivial_destructor))
        = default;

        /// ----------------------------------------------------------------------------------------
        /// # destructor
        /// ----------------------------------------------------------------------------------------
        constexpr custom_type(this_type&& that)
            requires(enums::has_all_flags(flags, custom_type_flags::destructor)
                     and not enums::has_all_flags(flags, custom_type_flags::trivial_destructor))
        {}

    public:
        /// ----------------------------------------------------------------------------------------
        /// # equal to opeartor
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator==(const this_type& that) const -> bool
            requires(enums::has_all_flags(flags, custom_type_flags::equal_to_operator))
        {
            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// # less than opeartor
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator<(const this_type& that) const -> bool
            requires(enums::has_all_flags(flags, custom_type_flags::less_than_to_operator))
        {
            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// # greater than opeartor
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator>(const this_type& that) const -> bool
            requires(enums::has_all_flags(flags, custom_type_flags::greater_than_to_operator))
        {
            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// # less than or equal to opeartor
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator<=(const this_type& that) const -> bool
            requires(enums::has_all_flags(flags, custom_type_flags::less_than_or_equal_to_operator))
        {
            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// # greater than or equal to opeartor
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator>=(const this_type& that) const -> bool
            requires(
                enums::has_all_flags(flags, custom_type_flags::greater_than_or_equal_to_operator))
        {
            return true;
        }
    };

    class non_default_constructible_type
    {
    public:
        non_default_constructible_type() = delete;
    };

    using copy_constructible_type = custom_type<custom_type_flags::copy_constructor>;

    using copy_assignable_type = custom_type<custom_type_flags::copy_operator>;

    using copyable_type =
        custom_type<custom_type_flags::copy_constructor | custom_type_flags::copy_operator>;

    using move_constructible_type = custom_type<custom_type_flags::move_constructor>;

    using move_assignable_type = custom_type<custom_type_flags::move_operator>;

    using moveable_type =
        custom_type<custom_type_flags::copy_constructor | custom_type_flags::copy_operator
                    | custom_type_flags::move_constructor | custom_type_flags::move_operator>;

    using destructible_type = custom_type<custom_type_flags::destructor>;

    using trivially_copy_constructible_type =
        custom_type<custom_type_flags::trivial_copy_constructor>;

    using trivially_copy_assignable_type = custom_type<custom_type_flags::trivial_copy_operator>;

    using trivially_copyable_type = custom_type<custom_type_flags::trivial_copy_constructor
                                                | custom_type_flags::trivial_copy_operator>;

    using trivially_move_constructible_type =
        custom_type<custom_type_flags::trivial_move_constructor>;

    using trivially_move_assignable_type = custom_type<custom_type_flags::trivial_move_operator>;

    using trivially_moveable_type = custom_type<
        custom_type_flags::trivial_copy_constructor | custom_type_flags::trivial_copy_operator
        | custom_type_flags::trivial_move_constructor | custom_type_flags::trivial_move_operator>;

    using trivially_destructible_type = custom_type<custom_type_flags::trivial_destructor>;

    using equality_comparable_type = custom_type<custom_type_flags::equal_to_operator>;

    using comparable_type =
        custom_type<custom_type_flags::equal_to_operator | custom_type_flags::less_than_to_operator
                    | custom_type_flags::greater_than_to_operator
                    | custom_type_flags::less_than_or_equal_to_operator
                    | custom_type_flags::greater_than_or_equal_to_operator>;
}
