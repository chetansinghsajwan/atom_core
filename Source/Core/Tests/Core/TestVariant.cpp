#include "catch2/catch_all.hpp"
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "Atom/Core/Variant.h"
#include "Atom/Test/TrackedType.h"

using namespace Atom;
using namespace Atom::Test;
using namespace Catch;

using TrackedType0 = TrackedTypeOf<i32>;
using TrackedType1 = TrackedTypeOf<f32>;
using TrackedType2 = TrackedTypeOf<char>;

TEST_CASE("Atom.Core.Variant")
{
    SECTION("Unique types")
    {
        // Compilation error.
        // Variant only allows unique types.
        // using Var = Variant<TrackedType0, TrackedType0, TrackedType1>;
    }

    SECTION("Type Indexing")
    {
        using Var = Variant<TrackedType0, TrackedType1, TrackedType2>;

        REQUIRE(Var::IndexOf<TrackedType0>() == 0);
        REQUIRE(Var::IndexOf<TrackedType1>() == 1);
        REQUIRE(Var::IndexOf<TrackedType2>() == 2);

        REQUIRE(TTI::IsSame<Var::TAt<0>, TrackedType0>);
        REQUIRE(TTI::IsSame<Var::TAt<1>, TrackedType1>);
        REQUIRE(TTI::IsSame<Var::TAt<2>, TrackedType2>);
    }

    SECTION("Count")
    {
        REQUIRE(Variant<i32, f64, f32, bool>::count() == 4);
    }

    SECTION("Has")
    {
        using Var = Variant<TrackedType0, TrackedType1, TrackedType2>;

        REQUIRE(Var::Has<TrackedType0>());
        REQUIRE(Var::Has<TrackedType1>());
        REQUIRE(Var::Has<TrackedType2>());
        REQUIRE(!Var::Has<i32>());
        REQUIRE(!Var::Has<f32>());

        REQUIRE(Var::Has<0>());
        REQUIRE(Var::Has<1>());
        REQUIRE(Var::Has<2>());
        REQUIRE(!Var::Has<3>());
    }

    SECTION("Trivial Default Constructor")
    {
        // # To Do: Fix this. Check _VariantImpl default value for _index.
        // 
        // STATIC_REQUIRE(RTriviallyDefaultConstructible<
        //     Variant<i32, char, f32>>);
    }

    SECTION("Default Constructor")
    {
        Variant<TrackedType0, TrackedType1, TrackedType2> v;

        REQUIRE(v.index() == 0);
        REQUIRE(v.is<TrackedType0>());
        REQUIRE(v.as<TrackedType0>().lastOp == TrackedType::EOperation::DefaultConstructor);
    }

    SECTION("Trivial Copy Constructor")
    {
        STATIC_REQUIRE(RTriviallyCopyConstructible<
            Variant<i32, char, f32>>);
    }

    SECTION("Copy Constructor")
    {
        Variant<TrackedType0, TrackedType1, TrackedType2> v0 = TrackedType1{};
        Variant<TrackedType0, TrackedType1, TrackedType2> v1 = v0;

        REQUIRE(v0.index() == 1);
        REQUIRE(v0.is<TrackedType1>());
        REQUIRE(v0.as<TrackedType1>().lastOp == TrackedType::EOperation::CopyConstructorAsThat);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<TrackedType1>());
        REQUIRE(v1.as<TrackedType1>().lastOp == TrackedType::EOperation::CopyConstructor);
    }

    SECTION("Copy Constructor Template")
    {
        Variant<TrackedType1, TrackedType2> v0 = TrackedType1{};
        Variant<TrackedType0, TrackedType1, TrackedType2> v1 = v0;

        REQUIRE(v0.index() == 0);
        REQUIRE(v0.is<TrackedType1>());
        REQUIRE(v0.as<TrackedType1>().lastOp == TrackedType::EOperation::CopyConstructorAsThat);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<TrackedType1>());
        REQUIRE(v1.as<TrackedType1>().lastOp == TrackedType::EOperation::CopyConstructor);
    }

    SECTION("Trivial Copy Assignment Operator")
    {
        STATIC_REQUIRE(RTriviallyCopyAssignable<
            Variant<i32, char, f32>>);
    }

    SECTION("Copy Assignment Operator")
    {
        Variant<TrackedType0, TrackedType1, TrackedType2> v0 = TrackedType1{};

        // v1 holds TrackedType0, so it will construct TrackedType1 when assigned
        Variant<TrackedType0, TrackedType1, TrackedType2> v1;
        v1 = v0;

        REQUIRE(v0.index() == 1);
        REQUIRE(v0.is<TrackedType1>());
        REQUIRE(v0.as<TrackedType1>().lastOp == TrackedType::EOperation::CopyConstructorAsThat);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<TrackedType1>());
        REQUIRE(v1.as<TrackedType1>().lastOp == TrackedType::EOperation::CopyConstructor);

        // v1 holds TrackedType1, so it will assign TrackedType1 when now
        v1 = v0;

        REQUIRE(v0.index() == 1);
        REQUIRE(v0.is<TrackedType1>());
        REQUIRE(v0.as<TrackedType1>().lastOp == TrackedType::EOperation::CopyOperatorAsThat);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<TrackedType1>());
        REQUIRE(v1.as<TrackedType1>().lastOp == TrackedType::EOperation::CopyOperator);
    }

    SECTION("Copy Assignment Operator Template")
    {
        Variant<TrackedType1, TrackedType2> v0 = TrackedType1{};

        // v1 holds TrackedType0, so it will construct TrackedType1 when assigned
        Variant<TrackedType0, TrackedType1, TrackedType2> v1;
        v1 = v0;

        REQUIRE(v0.index() == 0);
        REQUIRE(v0.is<TrackedType1>());
        REQUIRE(v0.as<TrackedType1>().lastOp == TrackedType::EOperation::CopyConstructorAsThat);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<TrackedType1>());
        REQUIRE(v1.as<TrackedType1>().lastOp == TrackedType::EOperation::CopyConstructor);

        // v1 holds TrackedType1, so it will assign TrackedType1 when now
        v1 = v0;

        REQUIRE(v0.index() == 0);
        REQUIRE(v0.is<TrackedType1>());
        REQUIRE(v0.as<TrackedType1>().lastOp == TrackedType::EOperation::CopyOperatorAsThat);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<TrackedType1>());
        REQUIRE(v1.as<TrackedType1>().lastOp == TrackedType::EOperation::CopyOperator);
    }

    SECTION("Trivial Move Constructor")
    {
        STATIC_REQUIRE(RTriviallyMoveConstructible<
            Variant<i32, char, f32>>);
    }

    SECTION("Move Constructor")
    {
        Variant<TrackedType0, TrackedType1, TrackedType2> v0 = TrackedType1{};
        Variant<TrackedType0, TrackedType1, TrackedType2> v1 = mov(v0);

        REQUIRE(v0.index() == 1);
        REQUIRE(v0.is<TrackedType1>());
        REQUIRE(v0.as<TrackedType1>().lastOp == TrackedType::EOperation::MoveConstructorAsThat);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<TrackedType1>());
        REQUIRE(v1.as<TrackedType1>().lastOp == TrackedType::EOperation::MoveConstructor);
    }

    SECTION("Move Constructor Template")
    {
        Variant<TrackedType1, TrackedType2> v0 = TrackedType1{};
        Variant<TrackedType0, TrackedType1, TrackedType2> v1 = mov(v0);

        REQUIRE(v0.index() == 0);
        REQUIRE(v0.is<TrackedType1>());
        REQUIRE(v0.as<TrackedType1>().lastOp == TrackedType::EOperation::MoveConstructorAsThat);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<TrackedType1>());
        REQUIRE(v1.as<TrackedType1>().lastOp == TrackedType::EOperation::MoveConstructor);
    }

    SECTION("Trivial Move Assignment Operator")
    {
        STATIC_REQUIRE(RTriviallyMoveAssignable<
            Variant<i32, char, f32>>);
    }

    SECTION("Move Assignment Operator")
    {
        Variant<TrackedType0, TrackedType1, TrackedType2> v0 = TrackedType1{};

        // v1 holds TrackedType0, so it will construct TrackedType1 when assigned
        Variant<TrackedType0, TrackedType1, TrackedType2> v1;
        v1 = mov(v0);

        REQUIRE(v0.index() == 1);
        REQUIRE(v0.is<TrackedType1>());
        REQUIRE(v0.as<TrackedType1>().lastOp == TrackedType::EOperation::MoveConstructorAsThat);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<TrackedType1>());
        REQUIRE(v1.as<TrackedType1>().lastOp == TrackedType::EOperation::MoveConstructor);

        // v1 holds TrackedType1, so it will assign TrackedType1 when now
        v1 = mov(v0);

        REQUIRE(v0.index() == 1);
        REQUIRE(v0.is<TrackedType1>());
        REQUIRE(v0.as<TrackedType1>().lastOp == TrackedType::EOperation::MoveOperatorAsThat);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<TrackedType1>());
        REQUIRE(v1.as<TrackedType1>().lastOp == TrackedType::EOperation::MoveOperator);
    }

    SECTION("Move Assignment Operator Template")
    {
        Variant<TrackedType1, TrackedType2> v0 = TrackedType1{};

        // v1 holds TrackedType0, so it will construct TrackedType1 when assigned
        Variant<TrackedType0, TrackedType1, TrackedType2> v1;
        v1 = mov(v0);

        REQUIRE(v0.index() == 0);
        REQUIRE(v0.is<TrackedType1>());
        REQUIRE(v0.as<TrackedType1>().lastOp == TrackedType::EOperation::MoveConstructorAsThat);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<TrackedType1>());
        REQUIRE(v1.as<TrackedType1>().lastOp == TrackedType::EOperation::MoveConstructor);

        // v1 holds TrackedType1, so it will assign TrackedType1 when now
        v1 = mov(v0);

        REQUIRE(v0.index() == 0);
        REQUIRE(v0.is<TrackedType1>());
        REQUIRE(v0.as<TrackedType1>().lastOp == TrackedType::EOperation::MoveOperatorAsThat);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<TrackedType1>());
        REQUIRE(v1.as<TrackedType1>().lastOp == TrackedType::EOperation::MoveOperator);
    }

    SECTION("Param Constructor")
    {
        Variant<i32, f64, char> v = 'c';

        REQUIRE(v.index() == 2);
        REQUIRE(v.is<char>());
        REQUIRE(v.as<char>() == 'c');
    }

    SECTION("Param Constructor: Non deducible type.")
    {
        // Compilation error.
        // 99 is an i32. Use type explicitly.
        // Variant<usize> v = 99;
        Variant<usize> v = usize{ 99 };

        REQUIRE(v.index() == 0);
        REQUIRE(v.is<usize>());
        REQUIRE(v.as<usize>() == 99);
    }

    SECTION("Param Operator")
    {
        Variant<i32, f64, char> v;
        v = 'c';

        REQUIRE(v.index() == 2);
        REQUIRE(v.is<char>());
        REQUIRE(v.as<char>() == 'c');
    }

    SECTION("Param Operator: Non deducible type.")
    {
        Variant<usize> v;

        // Compilation error.
        // 99 is an i32. Use type explicitly.
        // v = 99;
        v = usize{ 99 };

        REQUIRE(v.index() == 0);
        REQUIRE(v.is<usize>());
        REQUIRE(v.as<usize>() == 99);
    }

    SECTION("Destructor")
    {
        TrackedType::EOperation* lastOpPtr;

        {
            Variant<TrackedType0, TrackedType1, TrackedType2> v0 = TrackedType1{};
            lastOpPtr = &v0.as<TrackedType1>().lastOp;
        }

        REQUIRE(*lastOpPtr == TrackedType::EOperation::Destructor);
    }

    SECTION("Value write")
    {
        Variant<i32, f64, char> v;

        v.emplace<char>('h');

        REQUIRE(v.index() == 2);
        REQUIRE(v.is<char>());
        REQUIRE(v.as<char>() == 'h');

        v.set(99);

        REQUIRE(v.index() == 0);
        REQUIRE(v.is<i32>());
        REQUIRE(v.as<i32>() == 99);
    }

    SECTION("Value Read")
    {
        Variant<i32, f64, char> v = 'h';

        REQUIRE(v.is<char>());
        REQUIRE(v.as<char>() == 'h');

        REQUIRE(v.is<2>());
        REQUIRE(v.at<2>() == 'h');
    }
}
