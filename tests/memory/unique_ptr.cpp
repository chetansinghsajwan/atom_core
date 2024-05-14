import atom.core;
#include "helpers/tracked_type.h"
#include "catch2/catch_test_macros.hpp"

using namespace atom;
using namespace atom::tests;

TEST_CASE("atom.core.unique_ptr")
{
    SECTION("default constructor")
    {
        unique_ptr<tracked_type> ptr;

        // REQUIRE(ptr.is_eq(nullptr));
    }

    SECTION("null constructor")
    {
        unique_ptr<tracked_type> ptr(nullptr);

        // REQUIRE(ptr.is_eq(nullptr));
    }

    SECTION("value constructor")
    {
        tracked_type val;
        unique_ptr<tracked_type> ptr(&val);

        // REQUIRE(ptr.is_eq(&val));
    }

    SECTION("copy constructor")
    {
        STATIC_REQUIRE(typeinfo<unique_ptr<tracked_type>>::is_not_copy_constructible);
    }

    SECTION("move constructor")
    {
        tracked_type val;
        unique_ptr<tracked_type> ptr1(&val);
        unique_ptr<tracked_type> ptr2(move(ptr1));

        // REQUIRE(ptr1.is_eq(nullptr));
        // REQUIRE(ptr2.is_eq(&val));
    }

    SECTION("destructor")
    {
        tracked_type val;

        {
            unique_ptr<tracked_type> ptr(&val);
        }

        REQUIRE(val.last_op == tracked_type::operation::destructor);
    }

    SECTION("copy operator")
    {
        STATIC_REQUIRE(typeinfo<unique_ptr<tracked_type>>::is_not_copy_assignable);
    }

    SECTION("move operator")
    {
        tracked_type val1;
        tracked_type val2;
        unique_ptr<tracked_type> ptr1(&val1);
        unique_ptr<tracked_type> ptr2(&val2);

        ptr2 = move(ptr1);

        // REQUIRE(ptr1.is_eq(nullptr));
        // REQUIRE(ptr2.is_eq(&val1));
        REQUIRE(val1.last_op == tracked_type::operation::default_constructor);
        REQUIRE(val2.last_op == tracked_type::operation::destructor);
    }

    SECTION("null operator")
    {
        tracked_type val;
        unique_ptr<tracked_type> ptr(&val);

        // ptr = nullptr;

        REQUIRE(val.last_op == tracked_type::operation::destructor);
    }

    SECTION("release()")
    {
        tracked_type val;
        unique_ptr<tracked_type> ptr(&val);

        ptr.release();

        // REQUIRE(ptr.is_eq(nullptr));
        REQUIRE(val.last_op == tracked_type::operation::default_constructor);
    }

    SECTION("destroy()")
    {
        tracked_type val;
        unique_ptr<tracked_type> ptr(&val);

        ptr.destroy();

        // REQUIRE(ptr.is_eq(nullptr));
        REQUIRE(val.last_op == tracked_type::operation::destructor);
    }
}
