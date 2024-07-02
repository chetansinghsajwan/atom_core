module;
#include "catch2/catch_tostring.hpp"

export module atom_core.tests:tracked_type;

import atom_core;

namespace atom::tests
{
    /// --------------------------------------------------------------------------------------------
    /// type used to track object state.
    /// --------------------------------------------------------------------------------------------
    export class tracked_type
    {
        using this_type = tracked_type;

    public:
        /// ----------------------------------------------------------------------------------------
        /// enum used to represent state.
        /// ----------------------------------------------------------------------------------------
        enum class operation
        {
            none,

            default_constructor,
            copy_constructor,
            copy_constructor_as_that,
            copy_operator,
            copy_operator_as_that,
            move_constructor,
            move_constructor_as_that,
            move_operator,
            move_operator_as_that,
            value_copy_constructor,
            value_move_constructor,
            value_copy_operator,
            value_move_operator,
            destructor,

            equal_operator,
            less_than_operator,
            greater_than_operator,
            less_than_or_equal_operator,
            greater_than_or_equal_operator
        };

    public:
        constexpr tracked_type()
        {
            last_op = operation::default_constructor;
        }

        constexpr tracked_type(const this_type& that)
        {
            last_op = operation::copy_constructor;
            that.last_op = operation::copy_constructor_as_that;
        }

        constexpr auto operator=(const this_type& that) -> this_type&
        {
            last_op = operation::copy_operator;
            that.last_op = operation::copy_operator_as_that;
            return *this;
        }

        constexpr tracked_type(this_type&& that)
        {
            last_op = operation::move_constructor;
            that.last_op = operation::move_constructor_as_that;
        }

        constexpr auto operator=(this_type&& that) -> this_type&
        {
            last_op = operation::move_operator;
            that.last_op = operation::move_operator_as_that;
            return *this;
        }

        constexpr tracked_type(operation op)
            : last_op{ op }
        {}

        constexpr ~tracked_type()
        {
            last_op = operation::destructor;
        }

    public:
        constexpr auto operator==(const this_type& that) const -> bool
        {
            last_op = operation::equal_operator;
            that.last_op = operation::equal_operator;
            return true;
        }

        constexpr auto operator<(const this_type& that) const -> bool
        {
            last_op = operation::less_than_operator;
            that.last_op = operation::less_than_operator;
            return true;
        }

        constexpr auto operator>(const this_type& that) const -> bool
        {
            last_op = operation::greater_than_operator;
            that.last_op = operation::greater_than_operator;
            return true;
        }

        constexpr auto operator<=(const this_type& that) const -> bool
        {
            last_op = operation::less_than_or_equal_operator;
            that.last_op = operation::less_than_or_equal_operator;
            return true;
        }

        constexpr auto operator>=(const this_type& that) const -> bool
        {
            last_op = operation::greater_than_or_equal_operator;
            that.last_op = operation::greater_than_or_equal_operator;
            return true;
        }

    public:
        mutable operation last_op;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    export template <typename value_type>
    class tracked_type_of: public tracked_type
    {
        using this_type = tracked_type_of;
        using base_type = tracked_type;

    public:
        using base_type::base_type;
        using base_type::operator=;

        constexpr tracked_type_of(const value_type& value)
            : base_type(operation::value_copy_constructor)
            , value{ value }
        {}

        constexpr tracked_type_of(value_type&& value)
            : base_type(operation::value_move_constructor)
            , value{ move(value) }
        {}

        constexpr auto operator=(const value_type& value) -> this_type&
        {
            last_op = operation::value_copy_operator;
            this->value = value;
            return *this;
        }

        constexpr auto operator=(value_type&& value) -> this_type&
        {
            last_op = operation::value_move_operator;
            this->value = move(value);
            return *this;
        }

    public:
        value_type value;
    };
}

namespace Catch
{
    export template <>
    class StringMaker<atom::tests::tracked_type::operation>
    {
    public:
        static constexpr auto convert(atom::tests::tracked_type::operation op) -> std::string
        {
            return std::string{ atom::enums::to_string_view(op).to_std() };
        }
    };
}
