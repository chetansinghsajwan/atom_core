import atom.core;
#include "catch2/catch_test_macros.hpp"

using namespace atom;

TEST_CASE("atom.core.mut_ptr")
{
    const i32 val0 = 10;
    const i32 val1 = 11;

    SECTION("default constructor")
    {
        const ptr<i32> ptr;

        REQUIRE(ptr.is_null());
    }

    SECTION("null constructor")
    {
        const ptr<i32> ptr = nullptr;

        REQUIRE(ptr.is_null());
    }

    SECTION("value constructor")
    {
        ptr<i32> ptr = &val0;

        REQUIRE(ptr.unwrap() == &val0);
        REQUIRE(ptr.get() == val0);
    }

    SECTION("copy constructor")
    {
        STATIC_REQUIRE(rtrivially_copy_constructible<ptr<i32>>);

        ptr<i32> ptr0 = &val0;
        ptr<i32> ptr1 = ptr0;

        REQUIRE(ptr1.unwrap() == ptr0.unwrap());
        REQUIRE(ptr1.get() == ptr0.get());
    }

    SECTION("destructor")
    {
        STATIC_REQUIRE(rtrivially_destructible<ptr<i32>>);
    }

    SECTION("null operator")
    {
        ptr<i32> ptr = &val0;

        ptr = nullptr;

        REQUIRE(ptr.is_null());
    }

    SECTION("copy operator")
    {
        STATIC_REQUIRE(rtrivially_copy_assignable<ptr<i32>>);

        ptr<i32> ptr0 = &val0;
        ptr<i32> ptr1 = nullptr;
        ptr1 = &val0;

        REQUIRE(ptr1.unwrap() == ptr0.unwrap());
        REQUIRE(ptr1.get() == ptr0.get());
    }

    SECTION("value operator")
    {
        ptr<i32> ptr = nullptr;
        ptr = &val0;

        REQUIRE(ptr.get() == val0);
        REQUIRE(ptr.unwrap() == &val0);
    }

    SECTION("value access")
    {
        ptr<i32> ptr = nullptr;

        REQUIRE_THROWS_AS(ptr.get_safe(), contract_violation_exception);

        ptr = &val0;
        REQUIRE(ptr.get_safe() == val0);
    }

    SECTION("casting")
    {
        ptr<i32> ptr0;
        ptr<i64> ptr1 = ptr0.as_unsafe<i64>();
    }

    SECTION("compairision")
    {
        ptr<i32> ptr0 = nullptr;
        ptr<i32> ptr1 = nullptr;

        REQUIRE(ptr0.is_eq(nullptr));
        REQUIRE(ptr1.is_eq(nullptr));
        REQUIRE(ptr0.is_eq(ptr1));

        ptr0 = &val0;

        REQUIRE(not ptr0.is_eq(nullptr));
        REQUIRE(not ptr0.is_eq(ptr1));

        ptr1 = &val1;

        REQUIRE(not ptr1.is_eq(nullptr));
        REQUIRE(not ptr0.is_eq(ptr1));

        ptr1 = &val0;

        REQUIRE(ptr0.is_eq(ptr1));
    }
}
