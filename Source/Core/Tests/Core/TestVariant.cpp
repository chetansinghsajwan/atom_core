#include "catch2/catch_all.hpp"
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "Atom/Core/Variant.h"

using namespace Atom;
using namespace Catch;

enum class EOperation
{
    None,
    DefaultConstructor,
    CopyConstructor,
    CopyConstructorAsThat,
    CopyOperator,
    CopyOperatorAsThat,
    MoveConstructor,
    MoveConstructorAsThat,
    MoveOperator,
    MoveOperatorAsThat,
    Destructor
};

/// ------------------------------------------------------------------------------------------------
/// Type used to test object handling inside variant.
/// 
/// # Template Parameters
/// 
/// - `T`: Used to make different types for testing with Variant.
/// ------------------------------------------------------------------------------------------------
template <tname T>
class TestType
{
    pub ctor TestType()
    {
        lastOp = EOperation::DefaultConstructor;
    }

    pub ctor TestType(const TestType& that)
    {
        lastOp = EOperation::CopyConstructor;
        that.lastOp = EOperation::CopyConstructorAsThat;
    }

    pub fn op=(const TestType& that) -> TestType&
    {
        lastOp = EOperation::CopyOperator;
        that.lastOp = EOperation::CopyOperatorAsThat;
        return *this;
    }

    pub ctor TestType(TestType&& that)
    {
        lastOp = EOperation::MoveConstructor;
        that.lastOp = EOperation::MoveConstructorAsThat;
    }

    pub fn op=(TestType&& that) -> TestType&
    {
        lastOp = EOperation::MoveOperator;
        that.lastOp = EOperation::MoveOperatorAsThat;
        return *this;
    }

    pub dtor TestType()
    {
        lastOp = EOperation::Destructor;
    }

    pub mutable EOperation lastOp;
};

using TestType0 = TestType<int>;
using TestType1 = TestType<float>;
using TestType2 = TestType<char>;

TEST_CASE("Atom.Core.Variant")
{
    SECTION("Unique types")
    {
        // Compilation error.
        // Variant only allows unique types.
        // using Var = Variant<TestType0, TestType0, TestType1>;
    }

    SECTION("Type Indexing")
    {
        using Var = Variant<TestType0, TestType1, TestType2>;

        REQUIRE(Var::IndexOf<TestType0>() == 0);
        REQUIRE(Var::IndexOf<TestType1>() == 1);
        REQUIRE(Var::IndexOf<TestType2>() == 2);

        REQUIRE(TTI::IsSame<Var::TAt<0>, TestType0>);
        REQUIRE(TTI::IsSame<Var::TAt<1>, TestType1>);
        REQUIRE(TTI::IsSame<Var::TAt<2>, TestType2>);
    }

    SECTION("Count")
    {
        REQUIRE(Variant<int, double, float, bool>::Count() == 4);
    }

    SECTION("Has")
    {
        using Var = Variant<TestType0, TestType1, TestType2>;

        REQUIRE(Var::Has<TestType0>());
        REQUIRE(Var::Has<TestType1>());
        REQUIRE(Var::Has<TestType2>());
        REQUIRE(!Var::Has<int>());
        REQUIRE(!Var::Has<float>());

        REQUIRE(Var::Has<0>());
        REQUIRE(Var::Has<1>());
        REQUIRE(Var::Has<2>());
        REQUIRE(!Var::Has<3>());
    }

    SECTION("Trivial Default Constructor")
    {
        REQUIRE(RTriviallyDefaultConstructible<
            Variant<int, char, float>>);
    }

    SECTION("Default Constructor")
    {
        Variant<TestType0, TestType1, TestType2> v;

        REQUIRE(v.index() == 0);
        REQUIRE(v.is<TestType0>());
        REQUIRE(v.as<TestType0>().lastOp == EOperation::DefaultConstructor);
    }

    SECTION("Trivial Copy Constructor")
    {
        REQUIRE(RTriviallyCopyConstructible<
            Variant<int, char, float>>);
    }

    SECTION("Copy Constructor")
    {
        Variant<TestType0, TestType1, TestType2> v0 = TestType1{};
        Variant<TestType0, TestType1, TestType2> v1 = v0;

        REQUIRE(v0.index() == 1);
        REQUIRE(v0.is<TestType1>());
        REQUIRE(v0.as<TestType1>().lastOp == EOperation::CopyConstructorAsThat);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<TestType1>());
        REQUIRE(v1.as<TestType1>().lastOp == EOperation::CopyConstructor);
    }

    SECTION("Copy Constructor Template")
    {
        Variant<TestType1, TestType2> v0 = TestType1{};
        Variant<TestType0, TestType1, TestType2> v1 = v0;

        REQUIRE(v0.index() == 0);
        REQUIRE(v0.is<TestType1>());
        REQUIRE(v0.as<TestType1>().lastOp == EOperation::CopyConstructorAsThat);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<TestType1>());
        REQUIRE(v1.as<TestType1>().lastOp == EOperation::CopyConstructor);
    }

    SECTION("Trivial Copy Assignment Operator")
    {
        REQUIRE(RTriviallyCopyAssignable<
            Variant<int, char, float>>);
    }

    SECTION("Copy Assignment Operator")
    {
        Variant<TestType0, TestType1, TestType2> v0 = TestType1{};

        // v1 holds TestType0, so it will construct TestType1 when assigned
        Variant<TestType0, TestType1, TestType2> v1;
        v1 = v0;

        REQUIRE(v0.index() == 1);
        REQUIRE(v0.is<TestType1>());
        REQUIRE(v0.as<TestType1>().lastOp == EOperation::CopyConstructorAsThat);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<TestType1>());
        REQUIRE(v1.as<TestType1>().lastOp == EOperation::CopyConstructor);

        // v1 holds TestType1, so it will assign TestType1 when now
        v1 = v0;

        REQUIRE(v0.index() == 1);
        REQUIRE(v0.is<TestType1>());
        REQUIRE(v0.as<TestType1>().lastOp == EOperation::CopyOperatorAsThat);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<TestType1>());
        REQUIRE(v1.as<TestType1>().lastOp == EOperation::CopyOperator);
    }

    SECTION("Copy Assignment Operator Template")
    {
        Variant<TestType1, TestType2> v0 = TestType1{};

        // v1 holds TestType0, so it will construct TestType1 when assigned
        Variant<TestType0, TestType1, TestType2> v1;
        v1 = v0;

        REQUIRE(v0.index() == 0);
        REQUIRE(v0.is<TestType1>());
        REQUIRE(v0.as<TestType1>().lastOp == EOperation::CopyConstructorAsThat);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<TestType1>());
        REQUIRE(v1.as<TestType1>().lastOp == EOperation::CopyConstructor);

        // v1 holds TestType1, so it will assign TestType1 when now
        v1 = v0;

        REQUIRE(v0.index() == 0);
        REQUIRE(v0.is<TestType1>());
        REQUIRE(v0.as<TestType1>().lastOp == EOperation::CopyOperatorAsThat);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<TestType1>());
        REQUIRE(v1.as<TestType1>().lastOp == EOperation::CopyOperator);
    }

    SECTION("Trivial Move Constructor")
    {
        REQUIRE(RTriviallyMoveConstructible<
            Variant<int, char, float>>);
    }

    SECTION("Move Constructor")
    {
        Variant<TestType0, TestType1, TestType2> v0 = TestType1{};
        Variant<TestType0, TestType1, TestType2> v1 = mov(v0);

        REQUIRE(v0.index() == 1);
        REQUIRE(v0.is<TestType1>());
        REQUIRE(v0.as<TestType1>().lastOp == EOperation::MoveConstructorAsThat);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<TestType1>());
        REQUIRE(v1.as<TestType1>().lastOp == EOperation::MoveConstructor);
    }

    SECTION("Move Constructor Template")
    {
        Variant<TestType1, TestType2> v0 = TestType1{};
        Variant<TestType0, TestType1, TestType2> v1 = mov(v0);

        REQUIRE(v0.index() == 0);
        REQUIRE(v0.is<TestType1>());
        REQUIRE(v0.as<TestType1>().lastOp == EOperation::MoveConstructorAsThat);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<TestType1>());
        REQUIRE(v1.as<TestType1>().lastOp == EOperation::MoveConstructor);
    }

    SECTION("Trivial Move Assignment Operator")
    {
        REQUIRE(RTriviallyMoveAssignable<
            Variant<int, char, float>>);
    }

    SECTION("Move Assignment Operator")
    {
        Variant<TestType0, TestType1, TestType2> v0 = TestType1{};

        // v1 holds TestType0, so it will construct TestType1 when assigned
        Variant<TestType0, TestType1, TestType2> v1;
        v1 = mov(v0);

        REQUIRE(v0.index() == 1);
        REQUIRE(v0.is<TestType1>());
        REQUIRE(v0.as<TestType1>().lastOp == EOperation::MoveConstructorAsThat);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<TestType1>());
        REQUIRE(v1.as<TestType1>().lastOp == EOperation::MoveConstructor);

        // v1 holds TestType1, so it will assign TestType1 when now
        v1 = mov(v0);

        REQUIRE(v0.index() == 1);
        REQUIRE(v0.is<TestType1>());
        REQUIRE(v0.as<TestType1>().lastOp == EOperation::MoveOperatorAsThat);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<TestType1>());
        REQUIRE(v1.as<TestType1>().lastOp == EOperation::MoveOperator);
    }

    SECTION("Move Assignment Operator Template")
    {
        Variant<TestType1, TestType2> v0 = TestType1{};

        // v1 holds TestType0, so it will construct TestType1 when assigned
        Variant<TestType0, TestType1, TestType2> v1;
        v1 = mov(v0);

        REQUIRE(v0.index() == 0);
        REQUIRE(v0.is<TestType1>());
        REQUIRE(v0.as<TestType1>().lastOp == EOperation::MoveConstructorAsThat);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<TestType1>());
        REQUIRE(v1.as<TestType1>().lastOp == EOperation::MoveConstructor);

        // v1 holds TestType1, so it will assign TestType1 when now
        v1 = mov(v0);

        REQUIRE(v0.index() == 0);
        REQUIRE(v0.is<TestType1>());
        REQUIRE(v0.as<TestType1>().lastOp == EOperation::MoveOperatorAsThat);

        REQUIRE(v1.index() == 1);
        REQUIRE(v1.is<TestType1>());
        REQUIRE(v1.as<TestType1>().lastOp == EOperation::MoveOperator);
    }

    SECTION("Param Constructor")
    {
        Variant<int, double, char> v = 'c';

        REQUIRE(v.index() == 2);
        REQUIRE(v.is<char>());
        REQUIRE(v.as<char>() == 'c');
    }

    SECTION("Param Constructor: Non deducible type.")
    {
        // Compilation error.
        // 99 is an int. Use type explicitly.
        // Variant<usize> v = 99;
        Variant<usize> v = usize{ 99 };

        REQUIRE(v.index() == 0);
        REQUIRE(v.is<usize>());
        REQUIRE(v.as<usize>() == 99);
    }

    SECTION("Param Operator")
    {
        Variant<int, double, char> v;
        v = 'c';

        REQUIRE(v.index() == 2);
        REQUIRE(v.is<char>());
        REQUIRE(v.as<char>() == 'c');
    }

    SECTION("Param Operator: Non deducible type.")
    {
        Variant<usize> v;

        // Compilation error.
        // 99 is an int. Use type explicitly.
        // v = 99;
        v = usize{ 99 };

        REQUIRE(v.index() == 0);
        REQUIRE(v.is<usize>());
        REQUIRE(v.as<usize>() == 99);
    }

    SECTION("Destructor")
    {
        EOperation* lastOpPtr;

        {
            Variant<TestType0, TestType1, TestType2> v0 = TestType1{};
            lastOpPtr = &v0.as<TestType1>().lastOp;
        }

        REQUIRE(*lastOpPtr == EOperation::Destructor);
    }

    SECTION("Value write")
    {
        Variant<int, double, char> v;

        v.emplace<char>('h');

        REQUIRE(v.index() == 2);
        REQUIRE(v.is<char>());
        REQUIRE(v.as<char>() == 'h');

        v.set(99);

        REQUIRE(v.index() == 0);
        REQUIRE(v.is<int>());
        REQUIRE(v.as<int>() == 99);
    }

    SECTION("Value Read")
    {
        Variant<int, double, char> v = 'h';

        REQUIRE(v.is<char>());
        REQUIRE(v.as<char>() == 'h');

        REQUIRE(v.is<2>());
        REQUIRE(v.at<2>() == 'h');
    }
}
