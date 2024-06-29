module;
#include "catch2/catch_test_macros.hpp"

module atom.core.tests:option;

import atom.core;
import :tracked_type;
import :custom_type;

using namespace atom;
using namespace atom::tests;

TEST_CASE("atom.core.option")
{
    SECTION("default constructor")
    {
        // default constructor doesn't constructs the object, so it's not affected by the object's
        // default constructor.
        STATIC_REQUIRE(type_info<option<non_default_constructible_mock>>::is_default_constructible());

        option<tracked_type> opt;

        REQUIRE(not opt.is_value());
    }

    SECTION("null constructor")
    {
        option<i32> opt = { create_from_null };

        REQUIRE(not opt.is_value());
    }

    SECTION("null operator")
    {
        option<i32> opt = i32{ 0 };

        opt = { create_from_null };
        REQUIRE(not opt.is_value());
    }

    SECTION("value copy constructor")
    {
        tracked_type obj = tracked_type();
        option<tracked_type> opt = obj;

        REQUIRE(opt.is_value());
        REQUIRE(opt.get().last_op == tracked_type::operation::copy_constructor);
    }

    SECTION("value copy operator")
    {
        tracked_type obj = tracked_type();
        option<tracked_type> opt;
        opt = obj;

        REQUIRE(opt.is_value());
        REQUIRE(opt.get().last_op == tracked_type::operation::copy_constructor);

        opt = obj;

        REQUIRE(opt.is_value());
        REQUIRE(opt.get().last_op == tracked_type::operation::copy_operator);
    }

    SECTION("value move constructor")
    {
        tracked_type obj = tracked_type();
        option<tracked_type> opt = move(obj);

        REQUIRE(opt.is_value());
        REQUIRE(opt.get().last_op == tracked_type::operation::move_constructor);
    }

    SECTION("value move operator")
    {
        tracked_type obj = tracked_type();
        option<tracked_type> opt;
        opt = move(obj);

        REQUIRE(opt.is_value());
        REQUIRE(opt.get().last_op == tracked_type::operation::move_constructor);

        opt = move(obj);

        REQUIRE(opt.is_value());
        REQUIRE(opt.get().last_op == tracked_type::operation::move_operator);
    }

    SECTION("copy constructor")
    {
        STATIC_REQUIRE(
            type_info<option<trivially_copy_constructible_mock>>::is_trivially_copy_constructible());
        STATIC_REQUIRE(type_info<option<copy_constructible_mock>>::is_copy_constructible());

        const option<tracked_type> opt0 = tracked_type();
        option<tracked_type> opt1 = opt0;

        REQUIRE(opt0.is_value());
        REQUIRE(opt0.get().last_op == tracked_type::operation::copy_constructor_as_that);

        REQUIRE(opt1.is_value());
        REQUIRE(opt1.get().last_op == tracked_type::operation::copy_constructor);
    }

    SECTION("copy operator")
    {
        STATIC_REQUIRE(type_info<option<trivially_copyable_mock>>::is_trivially_copy_assignable());
        STATIC_REQUIRE(type_info<option<copyable_mock>>::is_copy_assignable());

        const option<tracked_type> opt0 = tracked_type();
        option<tracked_type> opt1 = tracked_type();
        opt1 = opt0;

        REQUIRE(opt0.is_value());
        REQUIRE(opt0.get().last_op == tracked_type::operation::copy_operator_as_that);

        REQUIRE(opt1.is_value());
        REQUIRE(opt1.get().last_op == tracked_type::operation::copy_operator);

        const option<tracked_type> opt2;
        opt1 = opt2;

        REQUIRE(not opt1.is_value());
    }

    SECTION("move constructor")
    {
        STATIC_REQUIRE(
            type_info<option<trivially_move_constructible_mock>>::is_trivially_move_constructible());
        STATIC_REQUIRE(type_info<option<move_constructible_mock>>::is_move_constructible());

        option<tracked_type> opt0 = tracked_type();
        option<tracked_type> opt1 = move(opt0);

        REQUIRE(opt0.is_value());
        REQUIRE(opt0.get().last_op == tracked_type::operation::move_constructor_as_that);

        REQUIRE(opt1.is_value());
        REQUIRE(opt1.get().last_op == tracked_type::operation::move_constructor);
    }

    SECTION("move operator")
    {
        STATIC_REQUIRE(type_info<option<trivially_moveable_mock>>::is_trivially_move_assignable());
        STATIC_REQUIRE(type_info<option<moveable_mock>>::is_move_assignable());

        option<tracked_type> opt0 = tracked_type();
        option<tracked_type> opt1 = tracked_type();
        opt1 = move(opt0);

        REQUIRE(opt0.is_value());
        REQUIRE(opt0.get().last_op == tracked_type::operation::move_operator_as_that);

        REQUIRE(opt1.is_value());
        REQUIRE(opt1.get().last_op == tracked_type::operation::move_operator);

        option<tracked_type> opt2;
        opt1 = move(opt2);

        REQUIRE(not opt1.is_value());
    }

    SECTION("destructor")
    {
        STATIC_REQUIRE(type_info<option<trivially_destructible_mock>>::is_trivially_destructible());
        STATIC_REQUIRE(type_info<option<destructible_mock>>::is_destructible());

        tracked_type::operation* last_op;

        {
            option<tracked_type> opt = tracked_type();
            last_op = &opt.get().last_op;
        }

        REQUIRE(*last_op == tracked_type::operation::destructor);
    }

    SECTION("emplace")
    {
        class type
        {
        public:
            type(i32 a, char b, f32 c)
                : a(a)
                , b(b)
                , c(c)
            {}

        public:
            i32 a;
            char b;
            f32 c;
        };

        option<type> opt;
        opt.emplace(9, 'a', 0.99f);

        REQUIRE(opt.is_value());
        REQUIRE(opt.get().a == 9);
        REQUIRE(opt.get().b == 'a');
        REQUIRE(opt.get().c == 0.99f);
    }

    SECTION("value access")
    {
        option<i32> opt = i32{ 10 };

        REQUIRE(opt.get() == 10);
    }

    SECTION("comparision")
    {
        STATIC_REQUIRE(type_info<option<equality_comparable_mock>>::is_equality_comparable());
        STATIC_REQUIRE(type_info<option<comparable_mock>>::is_comparable());

        option<tracked_type> opt0;
        option<tracked_type> opt1;

        // both have null state, so they are compared equal.
        REQUIRE(opt0 == opt1);

        // both have null state, they will not be compared.
        REQUIRE(not (opt0 < opt1));
        REQUIRE(not (opt0 > opt1));
        REQUIRE(not (opt0 <= opt1));
        REQUIRE(not (opt0 >= opt1));

        // if either have them null state, still they will not be compared.
        opt0 = tracked_type();
        opt0->last_op = tracked_type::operation::none;

        REQUIRE(not (opt0 == opt1));
        REQUIRE(opt0.get().last_op == tracked_type::operation::none);

        REQUIRE(not (opt0 < opt1));
        REQUIRE(opt0.get().last_op == tracked_type::operation::none);

        REQUIRE(not (opt0 > opt1));
        REQUIRE(opt0.get().last_op == tracked_type::operation::none);

        REQUIRE(not (opt0 <= opt1));
        REQUIRE(opt0.get().last_op == tracked_type::operation::none);

        REQUIRE(not (opt0 >= opt1));
        REQUIRE(opt0.get().last_op == tracked_type::operation::none);

        // they will be compared if they both have value state.
        opt1 = tracked_type();
        opt1->last_op = tracked_type::operation::none;

        REQUIRE(opt0 == opt1);
        REQUIRE(opt0.get().last_op == tracked_type::operation::equal_operator);
        REQUIRE(opt1.get().last_op == tracked_type::operation::equal_operator);

        REQUIRE(opt0 < opt1);
        REQUIRE(opt0.get().last_op == tracked_type::operation::less_than_operator);
        REQUIRE(opt1.get().last_op == tracked_type::operation::less_than_operator);

        REQUIRE(opt0 > opt1);
        REQUIRE(opt0.get().last_op == tracked_type::operation::greater_than_operator);
        REQUIRE(opt1.get().last_op == tracked_type::operation::greater_than_operator);

        REQUIRE(opt0 <= opt1);
        REQUIRE(opt0.get().last_op == tracked_type::operation::less_than_or_equal_operator);
        REQUIRE(opt1.get().last_op == tracked_type::operation::less_than_or_equal_operator);

        REQUIRE(opt0 >= opt1);
        REQUIRE(opt0.get().last_op == tracked_type::operation::greater_than_or_equal_operator);
        REQUIRE(opt1.get().last_op == tracked_type::operation::greater_than_or_equal_operator);
    }

    SECTION("reset")
    {
        tracked_type::operation* last_op;
        option<tracked_type> opt = tracked_type();
        last_op = &opt.get().last_op;

        opt.reset();

        REQUIRE(not opt.is_value());
        REQUIRE(*last_op == tracked_type::operation::destructor);
    }
}
