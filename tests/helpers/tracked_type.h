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

        static constexpr auto to_string_view(operation op) -> std::string_view
        {
            switch (op)
            {
                case operation::none:                        return "none";
                case operation::default_constructor:         return "default_constructor";
                case operation::copy_constructor:            return "copy_constructor";
                case operation::copy_constructor_as_that:    return "copy_constructor_as_that";
                case operation::copy_operator:               return "copy_operator";
                case operation::copy_operator_as_that:       return "copy_operator_as_that";
                case operation::move_constructor:            return "move_constructor";
                case operation::move_constructor_as_that:    return "move_constructor_as_that";
                case operation::move_operator:               return "move_operator";
                case operation::move_operator_as_that:       return "move_operator_as_that";
                case operation::value_copy_constructor:      return "value_copy_constructor";
                case operation::value_move_constructor:      return "value_move_constructor";
                case operation::value_copy_operator:         return "value_copy_operator";
                case operation::value_move_operator:         return "value_move_operator";
                case operation::destructor:                  return "destructor";
                case operation::equal_operator:              return "equal_operator";
                case operation::less_than_operator:          return "less_than_operator";
                case operation::greater_than_operator:       return "greater_than_operator";
                case operation::less_than_or_equal_operator: return "less_than_or_equal_operator";
                case operation::greater_than_or_equal_operator:
                    return "greater_than_or_equal_operator";
                default: return "unknown";
            }
        }

    public:
        constexpr tracked_type()
        {
            last_op = operation::default_constructor;
        }

        constexpr tracked_type(const tracked_type& that)
        {
            last_op = operation::copy_constructor;
            that.last_op = operation::copy_constructor_as_that;
        }

        constexpr auto operator=(const tracked_type& that) -> tracked_type&
        {
            last_op = operation::copy_operator;
            that.last_op = operation::copy_operator_as_that;
            return *this;
        }

        constexpr tracked_type(tracked_type&& that)
        {
            last_op = operation::move_constructor;
            that.last_op = operation::move_constructor_as_that;
        }

        constexpr auto operator=(tracked_type&& that) -> tracked_type&
        {
            last_op = operation::move_operator;
            that.last_op = operation::move_operator_as_that;
            return *this;
        }

        constexpr tracked_type(operation op)
            : last_op(op)
        {}

        constexpr ~tracked_type()
        {
            last_op = operation::destructor;
        }

    public:
        constexpr auto is_eq(const tracked_type& that) const -> bool
        {
            last_op = operation::equal_operator;
            that.last_op = operation::equal_operator;
            return true;
        }

        constexpr auto is_lt(const tracked_type& that) const -> bool
        {
            last_op = operation::less_than_operator;
            that.last_op = operation::less_than_operator;
            return true;
        }

        constexpr auto is_gt(const tracked_type& that) const -> bool
        {
            last_op = operation::greater_than_operator;
            that.last_op = operation::greater_than_operator;
            return true;
        }

        constexpr auto is_le(const tracked_type& that) const -> bool
        {
            last_op = operation::less_than_or_equal_operator;
            that.last_op = operation::less_than_or_equal_operator;
            return true;
        }

        constexpr auto is_ge(const tracked_type& that) const -> bool
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
    template <typename value_t>
    class tracked_t_of: public tracked_type
    {
        using base_t = tracked_type;

    public:
        using base_t::base_t;
        using base_t::operator=;

        constexpr tracked_t_of(const value_t& value)
            : base_t(operation::value_copy_constructor)
            , value(value)
        {}

        constexpr tracked_t_of(value_t&& value)
            : base_t(operation::value_move_constructor)
            , value(move(value))
        {}

        constexpr auto operator=(const value_t& value) -> tracked_type&
        {
            last_op = operation::value_copy_operator;
            this->value = value;
            return *this;
        }

        constexpr auto operator=(value_t&& value) -> tracked_type&
        {
            last_op = operation::value_move_operator;
            this->value = move(value);
            return *this;
        }

    public:
        value_t value;
    };
}

namespace Catch
{
    template <>
    class StringMaker<atom::tests::tracked_type::operation>
    {
    public:
        static constexpr auto convert(atom::tests::tracked_type::operation op) -> std::string
        {
            return std::string(atom::tests::tracked_type::to_string_view(op));
        }
    };
}
