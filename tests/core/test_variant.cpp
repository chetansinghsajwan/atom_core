import atom.core;
#include "helpers/tracked_type.h"
#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

using namespace atom;
using namespace atom::tests;
using namespace Catch;

using tracked_type0 = tracked_type_of<i32>;
using tracked_type1 = tracked_type_of<f32>;
using tracked_type2 = tracked_type_of<uchar>;

TEST_CASE("atom.core.variant")
{
    SECTION("unique types")
    {
        // compilation error.
        // variant only allows unique types.
        // using var = variant<tracked_type0, tracked_type0, tracked_type1>;
    }

    SECTION("type indexing")
    {
        using var = variant<tracked_type0, tracked_type1, tracked_type2>;

        REQUIRE(var::index_of<tracked_type0>() == 0);
        REQUIRE(var::index_of<tracked_type1>() == 1);
        REQUIRE(var::index_of<tracked_type2>() == 2);

        REQUIRE(tti::is_same<var::tat<0>, tracked_type0>);
        REQUIRE(tti::is_same<var::tat<1>, tracked_type1>);
        REQUIRE(tti::is_same<var::tat<2>, tracked_type2>);
    }

    SECTION("count")
    {
        REQUIRE(variant<i32, f64, f32, bool>::count() == 4);
    }

    SECTION("has")
    {
        using var = variant<tracked_type0, tracked_type1, tracked_type2>;

        REQUIRE(var::has<tracked_type0>());
        REQUIRE(var::has<tracked_type1>());
        REQUIRE(var::has<tracked_type2>());
        REQUIRE(!var::has<i32>());
        REQUIRE(!var::has<f32>());

        REQUIRE(var::has<0>());
        REQUIRE(var::has<1>());
        REQUIRE(var::has<2>());
        REQUIRE(!var::has<3>());
    }

    SECTION("trivial default constructor")
    {
        // # to do: fix this. check _variant_impl default value for _index.
        //
        // STATIC_REQUIRE(rtrivially_default_constructible<
        //     variant<i32, uchar, f32>>);
    }

    SECTION("default constructor")
    {
        variant<tracked_type0, tracked_type1, tracked_type2> v;

        REQUIRE(v.index() == 0);
        REQUIRE(v.is<tracked_type0>());
        REQUIRE(v.as<tracked_type0>().last_op == tracked_type::eoperation::default_constructor);
    }

    SECTION("trivial copy constructor")
    {
        STATIC_REQUIRE(rtrivially_copy_constructible<variant<i32, uchar, f32>>);
    }

    SECTION("copy constructor")
    {
        variant<tracked_type0, tracked_type1, tracked_type2> v0 = tracked_type1{};
        variant<tracked_type0, tracked_type1, tracked_type2> v1 = v0;

        REQUIRE(v0.index() == 1);
        REQUIRE(v0.is<tracked_type1>());
        REQUIRE(v0.as<tracked_type1>().last_op == tracked_type::eoperation::copy_constructor_as_that);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<tracked_type1>());
        REQUIRE(v1.as<tracked_type1>().last_op == tracked_type::eoperation::copy_constructor);
    }

    SECTION("copy constructor template")
    {
        variant<tracked_type1, tracked_type2> v0 = tracked_type1{};
        variant<tracked_type0, tracked_type1, tracked_type2> v1 = v0;

        REQUIRE(v0.index() == 0);
        REQUIRE(v0.is<tracked_type1>());
        REQUIRE(v0.as<tracked_type1>().last_op == tracked_type::eoperation::copy_constructor_as_that);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<tracked_type1>());
        REQUIRE(v1.as<tracked_type1>().last_op == tracked_type::eoperation::copy_constructor);
    }

    SECTION("trivial copy assignment operator")
    {
        STATIC_REQUIRE(rtrivially_copy_assignable<variant<i32, uchar, f32>>);
    }

    SECTION("copy assignment operator")
    {
        variant<tracked_type0, tracked_type1, tracked_type2> v0 = tracked_type1{};

        // v1 holds tracked_type0, so it will construct tracked_type1 when assigned
        variant<tracked_type0, tracked_type1, tracked_type2> v1;
        v1 = v0;

        REQUIRE(v0.index() == 1);
        REQUIRE(v0.is<tracked_type1>());
        REQUIRE(v0.as<tracked_type1>().last_op == tracked_type::eoperation::copy_constructor_as_that);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<tracked_type1>());
        REQUIRE(v1.as<tracked_type1>().last_op == tracked_type::eoperation::copy_constructor);

        // v1 holds tracked_type1, so it will assign tracked_type1 when now
        v1 = v0;

        REQUIRE(v0.index() == 1);
        REQUIRE(v0.is<tracked_type1>());
        REQUIRE(v0.as<tracked_type1>().last_op == tracked_type::eoperation::copy_operator_as_that);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<tracked_type1>());
        REQUIRE(v1.as<tracked_type1>().last_op == tracked_type::eoperation::copy_operator);
    }

    SECTION("copy assignment operator template")
    {
        variant<tracked_type1, tracked_type2> v0 = tracked_type1{};

        // v1 holds tracked_type0, so it will construct tracked_type1 when assigned
        variant<tracked_type0, tracked_type1, tracked_type2> v1;
        v1 = v0;

        REQUIRE(v0.index() == 0);
        REQUIRE(v0.is<tracked_type1>());
        REQUIRE(v0.as<tracked_type1>().last_op == tracked_type::eoperation::copy_constructor_as_that);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<tracked_type1>());
        REQUIRE(v1.as<tracked_type1>().last_op == tracked_type::eoperation::copy_constructor);

        // v1 holds tracked_type1, so it will assign tracked_type1 when now
        v1 = v0;

        REQUIRE(v0.index() == 0);
        REQUIRE(v0.is<tracked_type1>());
        REQUIRE(v0.as<tracked_type1>().last_op == tracked_type::eoperation::copy_operator_as_that);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<tracked_type1>());
        REQUIRE(v1.as<tracked_type1>().last_op == tracked_type::eoperation::copy_operator);
    }

    SECTION("trivial move constructor")
    {
        STATIC_REQUIRE(rtrivially_move_constructible<variant<i32, uchar, f32>>);
    }

    SECTION("move constructor")
    {
        variant<tracked_type0, tracked_type1, tracked_type2> v0 = tracked_type1{};
        variant<tracked_type0, tracked_type1, tracked_type2> v1 = mov(v0);

        REQUIRE(v0.index() == 1);
        REQUIRE(v0.is<tracked_type1>());
        REQUIRE(v0.as<tracked_type1>().last_op == tracked_type::eoperation::move_constructor_as_that);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<tracked_type1>());
        REQUIRE(v1.as<tracked_type1>().last_op == tracked_type::eoperation::move_constructor);
    }

    SECTION("move constructor template")
    {
        variant<tracked_type1, tracked_type2> v0 = tracked_type1{};
        variant<tracked_type0, tracked_type1, tracked_type2> v1 = mov(v0);

        REQUIRE(v0.index() == 0);
        REQUIRE(v0.is<tracked_type1>());
        REQUIRE(v0.as<tracked_type1>().last_op == tracked_type::eoperation::move_constructor_as_that);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<tracked_type1>());
        REQUIRE(v1.as<tracked_type1>().last_op == tracked_type::eoperation::move_constructor);
    }

    SECTION("trivial move assignment operator")
    {
        STATIC_REQUIRE(rtrivially_move_assignable<variant<i32, uchar, f32>>);
    }

    SECTION("move assignment operator")
    {
        variant<tracked_type0, tracked_type1, tracked_type2> v0 = tracked_type1{};

        // v1 holds tracked_type0, so it will construct tracked_type1 when assigned
        variant<tracked_type0, tracked_type1, tracked_type2> v1;
        v1 = mov(v0);

        REQUIRE(v0.index() == 1);
        REQUIRE(v0.is<tracked_type1>());
        REQUIRE(v0.as<tracked_type1>().last_op == tracked_type::eoperation::move_constructor_as_that);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<tracked_type1>());
        REQUIRE(v1.as<tracked_type1>().last_op == tracked_type::eoperation::move_constructor);

        // v1 holds tracked_type1, so it will assign tracked_type1 when now
        v1 = mov(v0);

        REQUIRE(v0.index() == 1);
        REQUIRE(v0.is<tracked_type1>());
        REQUIRE(v0.as<tracked_type1>().last_op == tracked_type::eoperation::move_operator_as_that);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<tracked_type1>());
        REQUIRE(v1.as<tracked_type1>().last_op == tracked_type::eoperation::move_operator);
    }

    SECTION("move assignment operator template")
    {
        variant<tracked_type1, tracked_type2> v0 = tracked_type1{};

        // v1 holds tracked_type0, so it will construct tracked_type1 when assigned
        variant<tracked_type0, tracked_type1, tracked_type2> v1;
        v1 = mov(v0);

        REQUIRE(v0.index() == 0);
        REQUIRE(v0.is<tracked_type1>());
        REQUIRE(v0.as<tracked_type1>().last_op == tracked_type::eoperation::move_constructor_as_that);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<tracked_type1>());
        REQUIRE(v1.as<tracked_type1>().last_op == tracked_type::eoperation::move_constructor);

        // v1 holds tracked_type1, so it will assign tracked_type1 when now
        v1 = mov(v0);

        REQUIRE(v0.index() == 0);
        REQUIRE(v0.is<tracked_type1>());
        REQUIRE(v0.as<tracked_type1>().last_op == tracked_type::eoperation::move_operator_as_that);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<tracked_type1>());
        REQUIRE(v1.as<tracked_type1>().last_op == tracked_type::eoperation::move_operator);
    }

    SECTION("param constructor")
    {
        variant<i32, f64, uchar> v = uchar('c');

        REQUIRE(v.index() == 2);
        REQUIRE(v.is<uchar>());
        REQUIRE(v.as<uchar>() == uchar('c'));
    }

    SECTION("param constructor: non deducible type.")
    {
        // compilation error.
        // 99 is an i32. use type explicitly.
        // variant<usize> v = 99;
        variant<usize> v = usize{ 99 };

        REQUIRE(v.index() == 0);
        REQUIRE(v.is<usize>());
        REQUIRE(v.as<usize>() == 99);
    }

    SECTION("param operator")
    {
        variant<i32, f64, uchar> v;
        v = uchar('c');

        REQUIRE(v.index() == 2);
        REQUIRE(v.is<uchar>());
        REQUIRE(v.as<uchar>() == uchar('c'));
    }

    SECTION("param operator: non deducible type.")
    {
        variant<usize> v;

        // compilation error.
        // 99 is an i32. use type explicitly.
        // v = 99;
        v = usize{ 99 };

        REQUIRE(v.index() == 0);
        REQUIRE(v.is<usize>());
        REQUIRE(v.as<usize>() == 99);
    }

    SECTION("destructor")
    {
        tracked_type::eoperation* last_op_ptr;

        {
            variant<tracked_type0, tracked_type1, tracked_type2> v0 = tracked_type1{};
            last_op_ptr = &v0.as<tracked_type1>().last_op;
        }

        REQUIRE(*last_op_ptr == tracked_type::eoperation::destructor);
    }

    SECTION("value write")
    {
        variant<i32, f64, uchar> v;

        v.emplace<uchar>('h');

        REQUIRE(v.index() == 2);
        REQUIRE(v.is<uchar>());
        REQUIRE(v.as<uchar>() == uchar('h'));

        v.set(i32{ 99 });

        REQUIRE(v.index() == 0);
        REQUIRE(v.is<i32>());
        REQUIRE(v.as<i32>() == 99);
    }

    SECTION("value read")
    {
        variant<i32, f64, uchar> v = uchar('h');

        REQUIRE(v.is<uchar>());
        REQUIRE(v.as<uchar>() == uchar('h'));

        REQUIRE(v.is<2>());
        REQUIRE(v.at<2>() == uchar('h'));
    }
}
