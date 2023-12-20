#include "Atom/Memory/UniquePtr.h"
#include "Helpers/TrackedType.h"
#include "catch2/catch_all.hpp"

using namespace Atom;
using namespace Atom::Tests;

TEST_CASE("Atom.Core.UniquePtr")
{
    SECTION("Default Constructor")
    {
        UniquePtr<TrackedType> ptr;

        REQUIRE(ptr.eq(nullptr));
    }

    SECTION("Null Constructor")
    {
        UniquePtr<TrackedType> ptr(nullptr);

        REQUIRE(ptr.eq(nullptr));
    }

    SECTION("Value Constructor")
    {
        TrackedType val;
        UniquePtr<TrackedType> ptr(&val);

        REQUIRE(ptr.eq(&val));
    }

    SECTION("Copy Constructor")
    {
        STATIC_REQUIRE(not RCopyConstructible<UniquePtr<TrackedType>>);
    }

    SECTION("Move Constructor")
    {
        TrackedType val;
        UniquePtr<TrackedType> ptr1(&val);
        UniquePtr<TrackedType> ptr2(mov(ptr1));

        REQUIRE(ptr1.eq(nullptr));
        REQUIRE(ptr2.eq(&val));
    }

    SECTION("Destructor")
    {
        TrackedType val;

        {
            UniquePtr<TrackedType> ptr(&val);
        }

        REQUIRE(val.lastOp == TrackedType::EOperation::Destructor);
    }

    SECTION("Copy Operator")
    {
        STATIC_REQUIRE(not RCopyAssignable<UniquePtr<TrackedType>>);
    }

    SECTION("Move Operator")
    {
        TrackedType val1;
        TrackedType val2;
        UniquePtr<TrackedType> ptr1(&val1);
        UniquePtr<TrackedType> ptr2(&val2);

        ptr2 = mov(ptr1);

        REQUIRE(ptr1.eq(nullptr));
        REQUIRE(ptr2.eq(&val1));
        REQUIRE(val1.lastOp == TrackedType::EOperation::DefaultConstructor);
        REQUIRE(val2.lastOp == TrackedType::EOperation::Destructor);
    }

    SECTION("Null Operator")
    {
        TrackedType val;
        UniquePtr<TrackedType> ptr(&val);

        ptr = nullptr;

        REQUIRE(val.lastOp == TrackedType::EOperation::Destructor);
    }

    SECTION("release()")
    {
        TrackedType val;
        UniquePtr<TrackedType> ptr(&val);

        ptr.release();

        REQUIRE(ptr.eq(nullptr));
        REQUIRE(val.lastOp == TrackedType::EOperation::DefaultConstructor);
    }

    SECTION("destroy()")
    {
        TrackedType val;
        UniquePtr<TrackedType> ptr(&val);

        ptr.destroy();

        REQUIRE(ptr.eq(nullptr));
        REQUIRE(val.lastOp == TrackedType::EOperation::Destructor);
    }
}
