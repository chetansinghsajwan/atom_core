#include "Atom/Memory/MemPtr.h"
#include "catch2/catch_all.hpp"

using namespace Atom;

TEST_CASE("Atom.Core.Ptr")
{
    STATIC_REQUIRE(RConstPtr<ConstPtr<i32>>);

    const i32 val0 = 10;
    const i32 val1 = 11;

    SECTION("Default Constructor")
    {
        const ConstPtr<i32> ptr;

        REQUIRE(ptr.isNull());
    }

    SECTION("Null Constructor")
    {
        const ConstPtr<i32> ptr = nullptr;

        REQUIRE(ptr.isNull());
    }

    SECTION("Value Constructor")
    {
        ConstPtr<i32> ptr = &val0;

        REQUIRE(ptr.unwrap() == &val0);
        REQUIRE(ptr.val() == val0);
    }

    SECTION("Copy Constructor")
    {
        STATIC_REQUIRE(RTriviallyCopyConstructible<ConstPtr<i32>>);

        ConstPtr<i32> ptr0 = &val0;
        ConstPtr<i32> ptr1 = ptr0;

        REQUIRE(ptr1.unwrap() == ptr0.unwrap());
        REQUIRE(ptr1.val() == ptr0.val());
    }

    SECTION("Construct from `RConstPtr`") {}

    SECTION("Destructor")
    {
        STATIC_REQUIRE(RTriviallyDestructible<ConstPtr<i32>>);
    }

    SECTION("Null Operator")
    {
        ConstPtr<i32> ptr = &val0;

        ptr = nullptr;

        REQUIRE(ptr.isNull());
    }

    SECTION("Copy Operator")
    {
        STATIC_REQUIRE(RTriviallyCopyAssignable<ConstPtr<i32>>);

        ConstPtr<i32> ptr0 = &val0;
        ConstPtr<i32> ptr1 = nullptr;
        ptr1 = &val0;

        REQUIRE(ptr1.unwrap() == ptr0.unwrap());
        REQUIRE(ptr1.val() == ptr0.val());
    }

    SECTION("Value Operator")
    {
        ConstPtr<i32> ptr = nullptr;
        ptr = &val0;

        REQUIRE(ptr.val() == val0);
        REQUIRE(ptr.unwrap() == &val0);
    }

    SECTION("Assign from `RConstPtr`") {}

    SECTION("Value access")
    {
        ConstPtr<i32> ptr = nullptr;

        REQUIRE_THROWS_AS(ptr.checkVal(), ContractViolationException);

        ptr = &val0;
        REQUIRE(ptr.checkVal() == val0);
    }

    SECTION("Casting")
    {
        ConstPtr<i32> ptr;
        ConstPtr<i64> ptr1 = ptr.unsafeAs<i64>();
    }

    SECTION("Compairision")
    {
        ConstPtr<i32> ptr0 = nullptr;
        ConstPtr<i32> ptr1 = nullptr;

        REQUIRE(ptr0.eq(nullptr));
        REQUIRE(ptr1.eq(nullptr));
        REQUIRE(ptr0.eq(ptr1));

        ptr0 = &val0;

        REQUIRE(ptr0.ne(nullptr));
        REQUIRE(ptr0.ne(ptr1));

        ptr1 = &val1;

        REQUIRE(ptr1.ne(nullptr));
        REQUIRE(ptr0.ne(ptr1));

        ptr1 = &val0;

        REQUIRE(ptr0.eq(ptr1));
    }
}
