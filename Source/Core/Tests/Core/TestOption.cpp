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
    Destructor
};

/// ------------------------------------------------------------------------------------------------
/// Type used to test object handling inside variant.
/// 
/// # To Do
/// 
/// - Move these utils to separate file, also from TestVariant.cpp.s
/// ------------------------------------------------------------------------------------------------
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
        REQUIRE(RTriviallyCopyConstructible<Opt>);
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
        REQUIRE(RTriviallyMoveConstructible<Opt>);
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
        REQUIRE(RTriviallyCopyAssignable<Opt>);
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
        REQUIRE(RTriviallyMoveAssignable<Opt>);
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
        REQUIRE(RTriviallyDestructible<Opt>);
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

    SECTION("Access Value")
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

    SECTION("Reference Types")
    {
        TestType val;
        Option<TestType&> opt = val;

        REQUIRE(&val == &opt.value());

        opt.value().lastOp = EOperation::Destructor;
        REQUIRE(val.lastOp == EOperation::Destructor);

        using Opt = Option<int&>;

        static_assert(RDefaultConstructible<Opt>);
        static_assert(RCopyConstructible<Opt>);
        static_assert(RMoveConstructible<Opt>);
        static_assert(RCopyAssignable<Opt>);
        static_assert(RMoveAssignable<Opt>);
        static_assert(RDestructible<Opt>);
    }
}