module;
#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

module atom_core.tests:variant;

import atom_core;
import :tracked_type;

using namespace atom;
using namespace atom::tests;
using namespace Catch;

using tracked_i32 = tracked_type_of<i32>;
using tracked_f32 = tracked_type_of<f32>;
using tracked_uchar = tracked_type_of<char>;

TEST_CASE("atom_core.variant")
{
    SECTION("unique types")
    {
        // compilation error.
        // variant only allows unique types.
        // using var = variant<tracked_type0, tracked_type0, tracked_type1>;
    }

    SECTION("type list")
    {
        using variant_type = variant<tracked_i32, tracked_f32, tracked_uchar>;

        REQUIRE(typename variant_type::value_types_list{}
                == type_list<tracked_i32, tracked_f32, tracked_uchar>{});
    }

    SECTION("trivial copy constructor")
    {
        STATIC_REQUIRE(type_info<variant<i32, char, f32>>::is_trivially_copy_constructible());
    }

    SECTION("copy constructor")
    {
        variant<tracked_type0, tracked_type1, tracked_type2> v0 = tracked_type1{};
        variant<tracked_type0, tracked_type1, tracked_type2> v1 = v0;

        REQUIRE(v0.get_index() == 1);
        REQUIRE(v0.is<tracked_f32>());
        REQUIRE(v0.get<tracked_f32>().last_op == tracked_type::operation::copy_constructor_as_that);

        REQUIRE(v1.get_index() == 1);
        REQUIRE(v1.is<tracked_f32>());
        REQUIRE(v1.get<tracked_f32>().last_op == tracked_type::operation::copy_constructor);
    }

    SECTION("copy constructor template")
    {
        variant<tracked_type1, tracked_type2> v0 = tracked_type1{};
        variant<tracked_type0, tracked_type1, tracked_type2> v1 = v0;

        REQUIRE(v0.get_index() == 0);
        REQUIRE(v0.is<tracked_f32>());
        REQUIRE(v0.get<tracked_f32>().last_op == tracked_type::operation::copy_constructor_as_that);

        REQUIRE(v1.get_index() == 1);
        REQUIRE(v1.is<tracked_f32>());
        REQUIRE(v1.get<tracked_f32>().last_op == tracked_type::operation::copy_constructor);
    }

    SECTION("trivial copy assignment operator")
    {
        STATIC_REQUIRE(type_info<variant<i32, char, f32>>::is_trivially_copy_assignable());
    }

    SECTION("copy assignment operator")
    {
        variant<tracked_type0, tracked_type1, tracked_type2> v0 = tracked_type1{};

        // v1 holds tracked_i32, so it will construct tracked_f32 when assigned
        variant<tracked_i32, tracked_f32, tracked_uchar> v1 = tracked_i32{ 0 };
        v1 = v0;

        REQUIRE(v0.get_index() == 1);
        REQUIRE(v0.is<tracked_f32>());
        REQUIRE(v0.get<tracked_f32>().last_op == tracked_type::operation::copy_constructor_as_that);

        REQUIRE(v1.get_index() == 1);
        REQUIRE(v1.is<tracked_f32>());
        REQUIRE(v1.get<tracked_f32>().last_op == tracked_type::operation::copy_constructor);

        // v1 holds tracked_type1, so it will assign tracked_type1 when now
        v1 = v0;

        REQUIRE(v0.get_index() == 1);
        REQUIRE(v0.is<tracked_f32>());
        REQUIRE(v0.get<tracked_f32>().last_op == tracked_type::operation::copy_operator_as_that);

        REQUIRE(v1.get_index() == 1);
        REQUIRE(v1.is<tracked_f32>());
        REQUIRE(v1.get<tracked_f32>().last_op == tracked_type::operation::copy_operator);
    }

    SECTION("copy assignment operator template")
    {
        variant<tracked_type1, tracked_type2> v0 = tracked_type1{};

        // v1 holds tracked_i32, so it will construct tracked_f32 when assigned
        variant<tracked_i32, tracked_f32, tracked_uchar> v1 = tracked_i32{ 0 };
        v1 = v0;

        REQUIRE(v0.get_index() == 0);
        REQUIRE(v0.is<tracked_f32>());
        REQUIRE(v0.get<tracked_f32>().last_op == tracked_type::operation::copy_constructor_as_that);

        REQUIRE(v1.get_index() == 1);
        REQUIRE(v1.is<tracked_f32>());
        REQUIRE(v1.get<tracked_f32>().last_op == tracked_type::operation::copy_constructor);

        // v1 holds tracked_type1, so it will assign tracked_type1 when now
        v1 = v0;

        REQUIRE(v0.get_index() == 0);
        REQUIRE(v0.is<tracked_f32>());
        REQUIRE(v0.get<tracked_f32>().last_op == tracked_type::operation::copy_operator_as_that);

        REQUIRE(v1.get_index() == 1);
        REQUIRE(v1.is<tracked_f32>());
        REQUIRE(v1.get<tracked_f32>().last_op == tracked_type::operation::copy_operator);
    }

    SECTION("trivial move constructor")
    {
        STATIC_REQUIRE(type_info<variant<i32, char, f32>>::is_trivially_move_constructible());
    }

    SECTION("move constructor")
    {
        variant<tracked_i32, tracked_f32, tracked_uchar> v0 = tracked_f32{};
        variant<tracked_i32, tracked_f32, tracked_uchar> v1 = move(v0);

        REQUIRE(v0.get_index() == 1);
        REQUIRE(v0.is<tracked_f32>());
        REQUIRE(v0.get<tracked_f32>().last_op == tracked_type::operation::move_constructor_as_that);

        REQUIRE(v1.get_index() == 1);
        REQUIRE(v1.is<tracked_f32>());
        REQUIRE(v1.get<tracked_f32>().last_op == tracked_type::operation::move_constructor);
    }

    SECTION("move constructor template")
    {
        variant<tracked_f32, tracked_uchar> v0 = tracked_f32{};
        variant<tracked_i32, tracked_f32, tracked_uchar> v1 = move(v0);

        REQUIRE(v0.get_index() == 0);
        REQUIRE(v0.is<tracked_f32>());
        REQUIRE(v0.get<tracked_f32>().last_op == tracked_type::operation::move_constructor_as_that);

        REQUIRE(v1.get_index() == 1);
        REQUIRE(v1.is<tracked_f32>());
        REQUIRE(v1.get<tracked_f32>().last_op == tracked_type::operation::move_constructor);
    }

    SECTION("trivial move assignment operator")
    {
        STATIC_REQUIRE(type_info<variant<i32, char, f32>>::is_trivially_move_assignable());
    }

    SECTION("move assignment operator")
    {
        variant<tracked_type0, tracked_type1, tracked_type2> v0 = tracked_type1{};

        // v1 holds tracked_i32, so it will construct tracked_f32 when assigned
        variant<tracked_i32, tracked_f32, tracked_uchar> v1 = tracked_i32{ 0 };
        v1 = move(v0);

        REQUIRE(v0.get_index() == 1);
        REQUIRE(v0.is<tracked_f32>());
        REQUIRE(v0.get<tracked_f32>().last_op == tracked_type::operation::move_constructor_as_that);

        REQUIRE(v1.get_index() == 1);
        REQUIRE(v1.is<tracked_f32>());
        REQUIRE(v1.get<tracked_f32>().last_op == tracked_type::operation::move_constructor);

        // v1 holds tracked_f32, so it will assign tracked_f32 when now
        v1 = move(v0);

        REQUIRE(v0.get_index() == 1);
        REQUIRE(v0.is<tracked_f32>());
        REQUIRE(v0.get<tracked_f32>().last_op == tracked_type::operation::move_operator_as_that);

        REQUIRE(v1.get_index() == 1);
        REQUIRE(v1.is<tracked_f32>());
        REQUIRE(v1.get<tracked_f32>().last_op == tracked_type::operation::move_operator);
    }

    SECTION("move assignment operator template")
    {
        variant<tracked_type1, tracked_type2> v0 = tracked_type1{};

        // v1 holds tracked_i32, so it will construct tracked_f32 when assigned
        variant<tracked_i32, tracked_f32, tracked_uchar> v1 = tracked_i32{ 0 };
        v1 = move(v0);

        REQUIRE(v0.get_index() == 0);
        REQUIRE(v0.is<tracked_f32>());
        REQUIRE(v0.get<tracked_f32>().last_op == tracked_type::operation::move_constructor_as_that);

        REQUIRE(v1.get_index() == 1);
        REQUIRE(v1.is<tracked_f32>());
        REQUIRE(v1.get<tracked_f32>().last_op == tracked_type::operation::move_constructor);

        // v1 holds tracked_f32, so it will assign tracked_f32 when now
        v1 = move(v0);

        REQUIRE(v0.get_index() == 0);
        REQUIRE(v0.is<tracked_f32>());
        REQUIRE(v0.get<tracked_f32>().last_op == tracked_type::operation::move_operator_as_that);

        REQUIRE(v1.get_index() == 1);
        REQUIRE(v1.is<tracked_f32>());
        REQUIRE(v1.get<tracked_f32>().last_op == tracked_type::operation::move_operator);
    }

    SECTION("param constructor")
    {
        variant<i32, f64, char> v = char('c');

        REQUIRE(v.get_index() == 2);
        REQUIRE(v.is<char>());
        REQUIRE(v.get<char>() == char('c'));
    }

    SECTION("param constructor: non deducible type.")
    {
        // compilation error.
        // 99 is an i32. use type explicitly.
        // variant<usize> v = 99;
        variant<usize, char> v = usize{ 99 };

        REQUIRE(v.get_index() == 0);
        REQUIRE(v.is<usize>());
        REQUIRE(v.get<usize>() == 99);
    }

    SECTION("param operator")
    {
        variant<i32, f64, char> v = i32{ 0 };
        v = char('c');

        REQUIRE(v.get_index() == 2);
        REQUIRE(v.is<char>());
        REQUIRE(v.get<char>() == char('c'));
    }

    SECTION("param operator: non deducible type.")
    {
        variant<usize, char> v = usize{ 0 };

        // compilation error.
        // 99 is an i32. use type explicitly.
        // v = 99;
        v = usize{ 99 };

        REQUIRE(v.get_index() == 0);
        REQUIRE(v.is<usize>());
        REQUIRE(v.get<usize>() == 99);
    }

    SECTION("destructor")
    {
        bool is_destroyed = false;

        struct scope_type
        {
            scope_type(bool* flag)
                : _flag{ flag }
            {}

            ~scope_type()
            {
                *_flag = true;
            }

            bool* _flag;
        };

        {
            variant<scope_type, i8> v0 = scope_type{ &is_destroyed };
        }

        REQUIRE(is_destroyed);
    }

    SECTION("value write")
    {
        variant<i32, f64, char> v = i32{ 0 };

        v.emplace<char>('h');

        REQUIRE(v.get_index() == 2);
        REQUIRE(v.is<char>());
        REQUIRE(v.get<char>() == char('h'));

        v.set(i32{ 99 });

        REQUIRE(v.get_index() == 0);
        REQUIRE(v.is<i32>());
        REQUIRE(v.get<i32>() == 99);
    }

    SECTION("value read")
    {
        variant<i32, f64, char> v = char('h');

        REQUIRE(v.is<char>());
        REQUIRE(v.get<char>() == char('h'));
    }
}
