#pragma once
#include "catch2/catch_tostring.hpp"

namespace atom::tests
{
    /// --------------------------------------------------------------------------------------------
    /// type used to track object state.
    /// --------------------------------------------------------------------------------------------
    class tracked_type
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// enum used to represent state.
        /// ----------------------------------------------------------------------------------------
        enum class eoperation
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
            destructor,

            equal_operator,
            less_than_operator,
            greater_than_operator,
            less_than_or_equal_operator,
            greater_than_or_equal_operator
        };

        static constexpr auto to_string_view(eoperation op) -> std::string_view
        {
            switch (op)
            {
                case eoperation::none:                       return "none";
                case eoperation::default_constructor:         return "default_constructor";
                case eoperation::copy_constructor:            return "copy_constructor";
                case eoperation::copy_constructor_as_that:      return "copy_constructor_as_that";
                case eoperation::copy_operator:               return "copy_operator";
                case eoperation::copy_operator_as_that:         return "copy_operator_as_that";
                case eoperation::move_constructor:            return "move_constructor";
                case eoperation::move_constructor_as_that:      return "move_constructor_as_that";
                case eoperation::move_operator:               return "move_operator";
                case eoperation::move_operator_as_that:         return "move_operator_as_that";
                case eoperation::destructor:                 return "destructor";
                case eoperation::equal_operator:              return "equal_operator";
                case eoperation::less_than_operator:           return "less_than_operator";
                case eoperation::greater_than_operator:        return "greater_than_operator";
                case eoperation::less_than_or_equal_operator:    return "less_than_or_equal_operator";
                case eoperation::greater_than_or_equal_operator: return "greater_than_or_equal_operator";
                default:                                     return "unknown";
            }
        }

    public:
        constexpr tracked_type()
        {
            last_op = eoperation::default_constructor;
        }

        constexpr tracked_type(const tracked_type& that)
        {
            last_op = eoperation::copy_constructor;
            that.last_op = eoperation::copy_constructor_as_that;
        }

        constexpr auto operator=(const tracked_type& that) -> tracked_type&
        {
            last_op = eoperation::copy_operator;
            that.last_op = eoperation::copy_operator_as_that;
            return *this;
        }

        constexpr tracked_type(tracked_type&& that)
        {
            last_op = eoperation::move_constructor;
            that.last_op = eoperation::move_constructor_as_that;
        }

        constexpr auto operator=(tracked_type&& that) -> tracked_type&
        {
            last_op = eoperation::move_operator;
            that.last_op = eoperation::move_operator_as_that;
            return *this;
        }

        constexpr ~tracked_type()
        {
            last_op = eoperation::destructor;
        }

    public:
        constexpr auto eq(const tracked_type& that) const -> bool
        {
            last_op = eoperation::equal_operator;
            that.last_op = eoperation::equal_operator;
            return true;
        }

        constexpr auto lt(const tracked_type& that) const -> bool
        {
            last_op = eoperation::less_than_operator;
            that.last_op = eoperation::less_than_operator;
            return true;
        }

        constexpr auto gt(const tracked_type& that) const -> bool
        {
            last_op = eoperation::greater_than_operator;
            that.last_op = eoperation::greater_than_operator;
            return true;
        }

        constexpr auto le(const tracked_type& that) const -> bool
        {
            last_op = eoperation::less_than_or_equal_operator;
            that.last_op = eoperation::less_than_or_equal_operator;
            return true;
        }

        constexpr auto ge(const tracked_type& that) const -> bool
        {
            last_op = eoperation::greater_than_or_equal_operator;
            that.last_op = eoperation::greater_than_or_equal_operator;
            return true;
        }

    public:
        mutable eoperation last_op;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    class tracked_type_of: public tracked_type
    {
        using base_type = tracked_type;

    public:
        using base_type::base_type;
        using base_type::operator=;

    public:
        type value;
    };
}

namespace Catch
{
    template <>
    class StringMaker<atom::tests::tracked_type::eoperation>
    {
    public:
        static constexpr auto convert(atom::tests::tracked_type::eoperation op) -> std::string
        {
            return std::string(atom::tests::tracked_type::to_string_view(op));
        }
    };
}
