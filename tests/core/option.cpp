import atom.core;
import atom.core.tests;
#include "catch2/catch_test_macros.hpp"

using namespace atom;
using namespace atom::tests;

TEST_CASE("atom.core.option")
{
    SECTION("default constructor")
    {
        // default constructor doesn't constructs the object, so it's not affected by the object's
        // default constructor.
        STATIC_REQUIRE(rdefault_constructible<option<non_default_constructible_mock>>);

        option<tracked_type> opt;

        REQUIRE(opt.is_null());
    }

    SECTION("null constructor")
    {
        option<i32> opt = nullopt;

        REQUIRE(opt.is_null());
    }

    SECTION("null operator")
    {
        option<i32> opt = i32(0);

        opt = nullopt;
        REQUIRE(opt.is_null());
    }

    SECTION("value copy constructor")
    {
        tracked_type obj = tracked_type();
        option<tracked_type> opt = obj;

        REQUIRE(opt.is_value());
        REQUIRE(opt.get().last_op == tracked_type::eoperation::copy_constructor);
    }

    SECTION("value copy operator")
    {
        tracked_type obj = tracked_type();
        option<tracked_type> opt;
        opt = obj;

        REQUIRE(opt.is_value());
        REQUIRE(opt.get().last_op == tracked_type::eoperation::copy_constructor);

        opt = obj;

        REQUIRE(opt.is_value());
        REQUIRE(opt.get().last_op == tracked_type::eoperation::copy_operator);
    }

    SECTION("value move constructor")
    {
        tracked_type obj = tracked_type();
        option<tracked_type> opt = mov(obj);

        REQUIRE(opt.is_value());
        REQUIRE(opt.get().last_op == tracked_type::eoperation::move_constructor);
    }

    SECTION("value move operator")
    {
        tracked_type obj = tracked_type();
        option<tracked_type> opt;
        opt = mov(obj);

        REQUIRE(opt.is_value());
        REQUIRE(opt.get().last_op == tracked_type::eoperation::move_constructor);

        opt = mov(obj);

        REQUIRE(opt.is_value());
        REQUIRE(opt.get().last_op == tracked_type::eoperation::move_operator);
    }

    SECTION("copy constructor")
    {
        STATIC_REQUIRE(rtrivially_copy_constructible<option<trivially_copy_constructible_mock>>);
        STATIC_REQUIRE(rcopy_constructible<option<copy_constructible_mock>>);

        const option<tracked_type> opt0 = tracked_type();
        option<tracked_type> opt1 = opt0;

        REQUIRE(opt0.is_value());
        REQUIRE(opt0.get().last_op == tracked_type::eoperation::copy_constructor_as_that);

        REQUIRE(opt1.is_value());
        REQUIRE(opt1.get().last_op == tracked_type::eoperation::copy_constructor);
    }

    SECTION("copy operator")
    {
        STATIC_REQUIRE(rtrivially_copy_assignable<option<trivially_copyable_mock>>);
        STATIC_REQUIRE(rcopy_assignable<option<copyable_mock>>);

        const option<tracked_type> opt0 = tracked_type();
        option<tracked_type> opt1 = tracked_type();
        opt1 = opt0;

        REQUIRE(opt0.is_value());
        REQUIRE(opt0.get().last_op == tracked_type::eoperation::copy_operator_as_that);

        REQUIRE(opt1.is_value());
        REQUIRE(opt1.get().last_op == tracked_type::eoperation::copy_operator);

        const option<tracked_type> opt2;
        opt1 = opt2;

        REQUIRE(opt1.is_null());
    }

    SECTION("move constructor")
    {
        STATIC_REQUIRE(rtrivially_move_constructible<option<trivially_move_constructible_mock>>);
        STATIC_REQUIRE(rmove_constructible<option<move_constructible_mock>>);

        option<tracked_type> opt0 = tracked_type();
        option<tracked_type> opt1 = mov(opt0);

        REQUIRE(opt0.is_value());
        REQUIRE(opt0.get().last_op == tracked_type::eoperation::move_constructor_as_that);

        REQUIRE(opt1.is_value());
        REQUIRE(opt1.get().last_op == tracked_type::eoperation::move_constructor);
    }

    SECTION("move operator")
    {
        STATIC_REQUIRE(rtrivially_move_assignable<option<trivially_moveable_mock>>);
        STATIC_REQUIRE(rmove_assignable<option<moveable_mock>>);

        option<tracked_type> opt0 = tracked_type();
        option<tracked_type> opt1 = tracked_type();
        opt1 = mov(opt0);

        REQUIRE(opt0.is_value());
        REQUIRE(opt0.get().last_op == tracked_type::eoperation::move_operator_as_that);

        REQUIRE(opt1.is_value());
        REQUIRE(opt1.get().last_op == tracked_type::eoperation::move_operator);

        option<tracked_type> opt2;
        opt1 = mov(opt2);

        REQUIRE(opt1.is_null());
    }

    SECTION("destructor")
    {
        STATIC_REQUIRE(rtrivially_destructible<option<trivially_destructible_mock>>);
        STATIC_REQUIRE(rdestructible<option<destructible_mock>>);

        tracked_type::eoperation* last_op;

        {
            option<tracked_type> opt = tracked_type();
            last_op = &opt.get().last_op;
        }

        REQUIRE(*last_op == tracked_type::eoperation::destructor);
    }

    SECTION("emplace")
    {
        class type
        {
        public:
            type(i32 a, uchar b, f32 c)
                : a{ a }
                , b{ b }
                , c{ c }
            {}

        public:
            i32 a;
            uchar b;
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
        SECTION("value()")
        {
            option<i32> opt = i32(10);

            REQUIRE(opt.get() == 10);
        }

        SECTION("value()")
        {
            option<i32> opt;
            REQUIRE(opt.get_or(99) == 99);

            opt = i32(10);
            REQUIRE(opt.get_or(99) == 10);
        }

        SECTION("value_or_invoke()")
        {
            auto invoke = [&]() -> i32 { return 99; };

            option<i32> opt;
            REQUIRE(opt.get_or_invoke(invoke) == 99);

            opt = i32(10);
            REQUIRE(opt.get_or_invoke(invoke) == 10);
        }

        SECTION("value_or_default()")
        {
            option<i32> opt;
            REQUIRE(opt.get_or_default() == 0);

            opt = i32(99);
            REQUIRE(opt.get_or_default() == 99);
        }
    }

    SECTION("comparision")
    {
        STATIC_REQUIRE(requality_comparable<option<equality_comparable_mock>>);
        STATIC_REQUIRE(rcomparable<option<comparable_mock>>);

        option<tracked_type> opt0;
        option<tracked_type> opt1;

        // both have null state, so they are compared equal.
        REQUIRE(opt0.is_eq(opt1));

        // both have null state, they will not be compared.
        REQUIRE(not opt0.is_lt(opt1));
        REQUIRE(not opt0.is_gt(opt1));
        REQUIRE(not opt0.is_le(opt1));
        REQUIRE(not opt0.is_ge(opt1));

        // if either have them null state, still they will not be compared.
        opt0 = tracked_type();
        opt0->last_op = tracked_type::eoperation::none;

        REQUIRE(not opt0.is_eq(opt1));
        REQUIRE(opt0.get().last_op == tracked_type::eoperation::none);

        REQUIRE(not opt0.is_lt(opt1));
        REQUIRE(opt0.get().last_op == tracked_type::eoperation::none);

        REQUIRE(not opt0.is_gt(opt1));
        REQUIRE(opt0.get().last_op == tracked_type::eoperation::none);

        REQUIRE(not opt0.is_le(opt1));
        REQUIRE(opt0.get().last_op == tracked_type::eoperation::none);

        REQUIRE(not opt0.is_ge(opt1));
        REQUIRE(opt0.get().last_op == tracked_type::eoperation::none);

        // they will be compared if they both have value state.
        opt1 = tracked_type();
        opt1->last_op = tracked_type::eoperation::none;

        REQUIRE(opt0.is_eq(opt1));
        REQUIRE(opt0.get().last_op == tracked_type::eoperation::equal_operator);
        REQUIRE(opt1.get().last_op == tracked_type::eoperation::equal_operator);

        REQUIRE(opt0.is_lt(opt1));
        REQUIRE(opt0.get().last_op == tracked_type::eoperation::less_than_operator);
        REQUIRE(opt1.get().last_op == tracked_type::eoperation::less_than_operator);

        REQUIRE(opt0.is_gt(opt1));
        REQUIRE(opt0.get().last_op == tracked_type::eoperation::greater_than_operator);
        REQUIRE(opt1.get().last_op == tracked_type::eoperation::greater_than_operator);

        REQUIRE(opt0.is_le(opt1));
        REQUIRE(opt0.get().last_op == tracked_type::eoperation::less_than_or_equal_operator);
        REQUIRE(opt1.get().last_op == tracked_type::eoperation::less_than_or_equal_operator);

        REQUIRE(opt0.is_ge(opt1));
        REQUIRE(opt0.get().last_op == tracked_type::eoperation::greater_than_or_equal_operator);
        REQUIRE(opt1.get().last_op == tracked_type::eoperation::greater_than_or_equal_operator);
    }

    SECTION("reset")
    {
        tracked_type::eoperation* last_op;
        option<tracked_type> opt = tracked_type();
        last_op = &opt.get().last_op;

        opt.reset();

        REQUIRE(opt.is_null());
        REQUIRE(*last_op == tracked_type::eoperation::destructor);
    }
}
