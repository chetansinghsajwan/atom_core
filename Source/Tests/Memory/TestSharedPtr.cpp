import atom.core;
#include "Helpers/TrackedType.h"
#include "catch2/catch_test_macros.hpp"

using namespace Atom;
using namespace Atom::Tests;

TEST_CASE("Atom.Core.SharedPtr")
{
    SECTION("Default Constructor")
    {
        SharedPtr<TrackedType> ptr;

        REQUIRE(ptr.eq(nullptr));
    }

    SECTION("Null Constructor")
    {
        SharedPtr<TrackedType> ptr;

        REQUIRE(ptr.eq(nullptr));
    }

    SECTION("Value Constructor")
    {
        TrackedType val;
        SharedPtr<TrackedType> ptr(&val);

        REQUIRE(ptr.eq(&val));
        REQUIRE(ptr.getCount() == 1);
    }

    SECTION("Copy Constructor")
    {
        TrackedType val;
        SharedPtr<TrackedType> ptr0(&val);
        SharedPtr<TrackedType> ptr1(ptr0);

        REQUIRE(ptr1.eq(&val));
        REQUIRE(ptr1.getCount() == 2);
    }

    SECTION("Move Constructor")
    {
        TrackedType val;
        SharedPtr<TrackedType> ptr0(&val);
        SharedPtr<TrackedType> ptr1(mov(ptr0));

        REQUIRE(ptr0.eq(nullptr));
        REQUIRE(ptr0.getCount() == 0);
        REQUIRE(ptr1.eq(&val));
        REQUIRE(ptr1.getCount() == 2);
    }

    SECTION("Destructor")
    {
        TrackedType val;

        {
            SharedPtr<TrackedType> ptr0(&val);

            {
                SharedPtr<TrackedType> ptr1(ptr0);
            }

            REQUIRE(val.lastOp == TrackedType::EOperation::DefaultConstructor);
        }

        REQUIRE(val.lastOp == TrackedType::EOperation::Destructor);
    }

    SECTION("Null Operator")
    {
        TrackedType val;
        SharedPtr<TrackedType> ptr0(&val);
        SharedPtr<TrackedType> ptr1(ptr0);

        ptr0 = nullptr;

        REQUIRE(ptr0.eq(nullptr));
        REQUIRE(ptr0.getCount() == 0);
        REQUIRE(ptr1.getCount() == 1);
        REQUIRE(val.lastOp == TrackedType::EOperation::DefaultConstructor);

        REQUIRE(ptr1.eq(nullptr));
        REQUIRE(ptr1.getCount() == 0);
        REQUIRE(val.lastOp == TrackedType::EOperation::Destructor);
    }

    SECTION("Copy Operator")
    {
        TrackedType val0;
        TrackedType val1;
        SharedPtr<TrackedType> ptr0(&val0);
        SharedPtr<TrackedType> ptr1(&val1);

        ptr1 = ptr0;

        REQUIRE(val1.lastOp == TrackedType::EOperation::Destructor);
        REQUIRE(ptr1.eq(&val0));
        REQUIRE(ptr1.getCount() == 2);
    }

    SECTION("Move Operator")
    {
        TrackedType val0;
        TrackedType val1;
        SharedPtr<TrackedType> ptr0(&val0);
        SharedPtr<TrackedType> ptr1(&val1);

        ptr1 = mov(ptr0);

        REQUIRE(val1.lastOp == TrackedType::EOperation::Destructor);
        REQUIRE(ptr0.eq(nullptr));
        REQUIRE(ptr0.getCount() == 0);
        REQUIRE(ptr1.eq(&val0));
        REQUIRE(ptr1.getCount() == 1);
    }

    SECTION("release()")
    {
        // Same as null operator.
    }
}
