#include "catch2/catch_all.hpp"
#include "Atom/Core/Option.h"

using namespace Atom;

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
    Destructor,

    EqualOperator,
    NotEqualOperator,
    LessThanOperator,
    GreaterThanOperator,
    LessThanOrEqualOperator,
    GreaterThanOrEqualOperator
};

/// ------------------------------------------------------------------------------------------------
/// Type used to test object handling inside variant.
/// 
/// # To Do
/// 
/// - Move these utils to separate file, also from TestVariant.cpp.
/// ------------------------------------------------------------------------------------------------
class TestType
{
public:
    ctor TestType()
    {
        lastOp = EOperation::DefaultConstructor;
    }

    ctor TestType(const TestType& that)
    {
        lastOp = EOperation::CopyConstructor;
        that.lastOp = EOperation::CopyConstructorAsThat;
    }

    fn op=(const TestType& that) -> TestType&
    {
        lastOp = EOperation::CopyOperator;
        that.lastOp = EOperation::CopyOperatorAsThat;
        return *this;
    }

    ctor TestType(TestType&& that)
    {
        lastOp = EOperation::MoveConstructor;
        that.lastOp = EOperation::MoveConstructorAsThat;
    }

    fn op=(TestType&& that) -> TestType&
    {
        lastOp = EOperation::MoveOperator;
        that.lastOp = EOperation::MoveOperatorAsThat;
        return *this;
    }

    dtor TestType()
    {
        lastOp = EOperation::Destructor;
    }

public:
    fn op==(const TestType& opt1) const -> bool
    {
        lastOp = EOperation::EqualOperator;
        return true;
    }

    fn op!=(const TestType& opt1) const -> bool
    {
        lastOp = EOperation::NotEqualOperator;
        return false;
    }

    fn op<(const TestType& opt1) const -> bool
    {
        lastOp = EOperation::LessThanOperator;
        return true;
    }

    fn op>(const TestType& opt1) const -> bool
    {
        lastOp = EOperation::GreaterThanOperator;
        return true;
    }

    fn op<=(const TestType& opt1) const -> bool
    {
        lastOp = EOperation::LessThanOrEqualOperator;
        return true;
    }

    fn op>=(const TestType& opt1) const -> bool
    {
        lastOp = EOperation::GreaterThanOrEqualOperator;
        return true;
    }

public:
    mutable EOperation lastOp;
};

/// ------------------------------------------------------------------------------------------------
/// # To Do
/// 
/// - Check triviality with custom types for accurate checking.
/// ------------------------------------------------------------------------------------------------
TEST_CASE("Atom.Core.Option")
{
    SECTION("Default Constructor")
    {
        Option<TestType> opt;

        REQUIRE(not opt.isValue());
    }

    SECTION("Value Constructor")
    {
        Option<TestType> opt = TestType();

        REQUIRE(opt.isValue());
        REQUIRE(opt.value().lastOp == EOperation::MoveConstructor);

        opt = TestType();

        REQUIRE(opt.isValue());
        REQUIRE(opt.value().lastOp == EOperation::MoveOperator);
    }

    SECTION("Trivial Copy Constructor")
    {
        using Opt = Option<int>;
        STATIC_REQUIRE(RTriviallyCopyConstructible<Opt>);
    }

    SECTION("Copy Constructor")
    {
        Option<TestType> opt0 = TestType();
        Option<TestType> opt1 = opt0;

        REQUIRE(opt0.isValue());
        REQUIRE(opt0.value().lastOp == EOperation::CopyConstructorAsThat);

        REQUIRE(opt1.isValue());
        REQUIRE(opt1.value().lastOp == EOperation::CopyConstructor);
    }

    SECTION("Trivial Move Constructor")
    {
        using Opt = Option<int>;
        STATIC_REQUIRE(RTriviallyMoveConstructible<Opt>);
    }

    SECTION("Move Constructor")
    {
        Option<TestType> opt0 = TestType();
        Option<TestType> opt1 = mov(opt0);

        REQUIRE(opt0.isValue());
        REQUIRE(opt0.value().lastOp == EOperation::MoveConstructorAsThat);

        REQUIRE(opt1.isValue());
        REQUIRE(opt1.value().lastOp == EOperation::MoveConstructor);
    }

    SECTION("Trivial Copy Assignment")
    {
        using Opt = Option<int>;
        STATIC_REQUIRE(RTriviallyCopyAssignable<Opt>);
    }

    SECTION("Copy Assignment")
    {
        Option<TestType> opt0 = TestType();
        Option<TestType> opt1 = TestType();
        opt1 = opt0;

        REQUIRE(opt0.isValue());
        REQUIRE(opt0.value().lastOp == EOperation::CopyOperatorAsThat);

        REQUIRE(opt1.isValue());
        REQUIRE(opt1.value().lastOp == EOperation::CopyOperator);

        Option<TestType> opt2;
        opt1 = opt2;

        REQUIRE(not opt1.isValue());
    }

    SECTION("Trivial Move Assignment")
    {
        using Opt = Option<int>;
        STATIC_REQUIRE(RTriviallyMoveAssignable<Opt>);
    }

    SECTION("Move Assignment")
    {
        Option<TestType> opt0 = TestType();
        Option<TestType> opt1 = TestType();
        opt1 = mov(opt0);

        REQUIRE(opt0.isValue());
        REQUIRE(opt0.value().lastOp == EOperation::MoveOperatorAsThat);

        REQUIRE(opt1.isValue());
        REQUIRE(opt1.value().lastOp == EOperation::MoveOperator);

        Option<TestType> opt2;
        opt1 = mov(opt2);

        REQUIRE(not opt1.isValue());
    }

    SECTION("Trivial Destructor")
    {
        using Opt = Option<int>;
        STATIC_REQUIRE(RTriviallyDestructible<Opt>);
    }

    SECTION("Destructor")
    {
        EOperation* lastOp;

        {
            Option<TestType> opt = TestType();
            lastOp = &opt.value().lastOp;
        }

        REQUIRE(*lastOp == EOperation::Destructor);
    }

    SECTION("Access Value using value(), op*(), op->()")
    {
        class Overloads
        {
        public:
            static constexpr usize Test(TestType&) { return 0; }
            static constexpr usize Test(const TestType&) { return 1; }
            static constexpr usize Test(TestType&&) { return 2; }
        };

        Option<TestType> opt0 = TestType();
        const Option<TestType> opt1 = TestType();

        REQUIRE(Overloads::Test(opt0.value()) == 0);
        REQUIRE(Overloads::Test(opt1.value()) == 1);
        REQUIRE(Overloads::Test(mov(opt0).value()) == 2);

        REQUIRE(Overloads::Test(*opt0) == 0);
        REQUIRE(Overloads::Test(*opt1) == 1);
        REQUIRE(Overloads::Test(mov(*opt0)) == 2);

        REQUIRE(opt0->lastOp == EOperation::MoveConstructor);
        REQUIRE(opt1->lastOp == EOperation::MoveConstructor);
    }

    SECTION("Emplace")
    {
        class Type
        {
        public:
            Type(int a, char b, float c):
                a{ a }, b{ b }, c{ c } { }

        public:
            int a;
            char b;
            float c;
        };

        Option<Type> opt;
        opt.emplace(9, 'a', 0.99f);

        REQUIRE(opt.isValue());
        REQUIRE(opt.value().a == 9);
        REQUIRE(opt.value().b == 'a');
        REQUIRE(opt.value().c == 0.99f);

        opt.reset();

        REQUIRE(not opt.isValue());
    }

    SECTION("Equal Operator")
    {
        Option<TestType> opt0;
        Option<TestType> opt1;

        // They have same state, that is null.
        REQUIRE(opt0 == opt1);

        opt0 = TestType();
        opt0.value().lastOp = EOperation::None;

        // They don't have same state anymore.
        REQUIRE(opt0 != opt1);

        // No comparision is performed as one of the operands has no value.
        REQUIRE(opt0.value().lastOp == EOperation::None);

        // Now they both have some values.
        opt1 = TestType();

        REQUIRE(opt0 == opt1);

        // Comparision is performed as both have some value.
        REQUIRE(opt0.value().lastOp == EOperation::EqualOperator);
    }

    SECTION("Other Compairision Operators, compairison with Option")
    {
        Option<TestType> opt0;
        Option<TestType> opt1;

        REQUIRE_FALSE(opt0 < opt1);
        REQUIRE_FALSE(opt0 > opt1);
        REQUIRE_FALSE(opt0 <= opt1);
        REQUIRE_FALSE(opt0 >= opt1);

        opt0 = TestType();
        REQUIRE_FALSE(opt0 < opt1);
        REQUIRE_FALSE(opt0 > opt1);
        REQUIRE_FALSE(opt0 <= opt1);
        REQUIRE_FALSE(opt0 >= opt1);

        opt1 = TestType();
        REQUIRE(opt0 < opt1);
        REQUIRE(opt0 > opt1);
        REQUIRE(opt0 <= opt1);
        REQUIRE(opt0 >= opt1);
    }

    SECTION("Reference Types")
    {
        TestType val;
        Option<TestType&> opt = val;

        REQUIRE(&val == &opt.value());

        opt.value().lastOp = EOperation::Destructor;
        REQUIRE(val.lastOp == EOperation::Destructor);

        using Opt = Option<int&>;

        STATIC_REQUIRE(RDefaultConstructible<Opt>);
        STATIC_REQUIRE(RCopyConstructible<Opt>);
        STATIC_REQUIRE(RMoveConstructible<Opt>);
        STATIC_REQUIRE(RCopyAssignable<Opt>);
        STATIC_REQUIRE(RMoveAssignable<Opt>);
        STATIC_REQUIRE(RDestructible<Opt>);
    }
}