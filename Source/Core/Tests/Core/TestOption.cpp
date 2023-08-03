#include "catch2/catch_all.hpp"
#include "Atom/Core/Option.h"
#include "Atom/Test/TrackedType.h"
#include "Atom/Test/CustomType.h"

using namespace Atom;
using namespace Atom::Test;

/// ------------------------------------------------------------------------------------------------
/// # To Do
/// 
/// - Check triviality with custom types for accurate checking.
/// ------------------------------------------------------------------------------------------------
TEST_CASE("Atom.Core.Option")
{
    SECTION("Default Constructor")
    {
        Option<TrackedType> opt;

        REQUIRE(not opt.isValue());
    }

    SECTION("Value Constructor")
    {
        Option<TrackedType> opt = TrackedType();

        REQUIRE(opt.isValue());
        REQUIRE(opt.value().lastOp == TrackedType::EOperation::MoveConstructor);

        opt = TrackedType();

        REQUIRE(opt.isValue());
        REQUIRE(opt.value().lastOp == TrackedType::EOperation::MoveOperator);
    }

    SECTION("Trivial Copy Constructor")
    {
        using Opt = Option<int>;
        STATIC_REQUIRE(RTriviallyCopyConstructible<Opt>);
    }

    SECTION("Copy Constructor")
    {
        Option<TrackedType> opt0 = TrackedType();
        Option<TrackedType> opt1 = opt0;

        REQUIRE(opt0.isValue());
        REQUIRE(opt0.value().lastOp == TrackedType::EOperation::CopyConstructorAsThat);

        REQUIRE(opt1.isValue());
        REQUIRE(opt1.value().lastOp == TrackedType::EOperation::CopyConstructor);
    }

    SECTION("Trivial Move Constructor")
    {
        using Opt = Option<int>;
        STATIC_REQUIRE(RTriviallyMoveConstructible<Opt>);
    }

    SECTION("Move Constructor")
    {
        Option<TrackedType> opt0 = TrackedType();
        Option<TrackedType> opt1 = mov(opt0);

        REQUIRE(opt0.isValue());
        REQUIRE(opt0.value().lastOp == TrackedType::EOperation::MoveConstructorAsThat);

        REQUIRE(opt1.isValue());
        REQUIRE(opt1.value().lastOp == TrackedType::EOperation::MoveConstructor);
    }

    SECTION("Trivial Copy Assignment")
    {
        using Opt = Option<int>;
        STATIC_REQUIRE(RTriviallyCopyAssignable<Opt>);
    }

    SECTION("Copy Assignment")
    {
        Option<TrackedType> opt0 = TrackedType();
        Option<TrackedType> opt1 = TrackedType();
        opt1 = opt0;

        REQUIRE(opt0.isValue());
        REQUIRE(opt0.value().lastOp == TrackedType::EOperation::CopyOperatorAsThat);

        REQUIRE(opt1.isValue());
        REQUIRE(opt1.value().lastOp == TrackedType::EOperation::CopyOperator);

        Option<TrackedType> opt2;
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
        Option<TrackedType> opt0 = TrackedType();
        Option<TrackedType> opt1 = TrackedType();
        opt1 = mov(opt0);

        REQUIRE(opt0.isValue());
        REQUIRE(opt0.value().lastOp == TrackedType::EOperation::MoveOperatorAsThat);

        REQUIRE(opt1.isValue());
        REQUIRE(opt1.value().lastOp == TrackedType::EOperation::MoveOperator);

        Option<TrackedType> opt2;
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
        TrackedType::EOperation* lastOp;

        {
            Option<TrackedType> opt = TrackedType();
            lastOp = &opt.value().lastOp;
        }

        REQUIRE(*lastOp == TrackedType::EOperation::Destructor);
    }

    SECTION("Access Value using value(), op*(), op->()")
    {
        class Overloads
        {
        public:
            static constexpr usize Test(TrackedType&) { return 0; }
            static constexpr usize Test(const TrackedType&) { return 1; }
            static constexpr usize Test(TrackedType&&) { return 2; }
        };

        Option<TrackedType> opt0 = TrackedType();
        const Option<TrackedType> opt1 = TrackedType();

        REQUIRE(Overloads::Test(opt0.value()) == 0);
        REQUIRE(Overloads::Test(opt1.value()) == 1);
        REQUIRE(Overloads::Test(mov(opt0).value()) == 2);

        REQUIRE(Overloads::Test(*opt0) == 0);
        REQUIRE(Overloads::Test(*opt1) == 1);
        REQUIRE(Overloads::Test(mov(*opt0)) == 2);

        REQUIRE(opt0->lastOp == TrackedType::EOperation::MoveConstructor);
        REQUIRE(opt1->lastOp == TrackedType::EOperation::MoveConstructor);
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
        Option<TrackedType> opt0;
        Option<TrackedType> opt1;

        // They have same state, that is null.
        REQUIRE(opt0 == opt1);

        opt0 = TrackedType();
        opt0.value().lastOp = TrackedType::EOperation::None;

        // They don't have same state anymore.
        REQUIRE(opt0 != opt1);

        // No comparision is performed as one of the operands has no value.
        REQUIRE(opt0.value().lastOp == TrackedType::EOperation::None);

        // Now they both have some values.
        opt1 = TrackedType();

        REQUIRE(opt0 == opt1);

        // Comparision is performed as both have some value.
        REQUIRE(opt0.value().lastOp == TrackedType::EOperation::EqualOperator);
    }

    SECTION("Other Compairision Operators, compairison with Option")
    {
        Option<TrackedType> opt0;
        Option<TrackedType> opt1;

        REQUIRE_FALSE(opt0 < opt1);
        REQUIRE_FALSE(opt0 > opt1);
        REQUIRE_FALSE(opt0 <= opt1);
        REQUIRE_FALSE(opt0 >= opt1);

        opt0 = TrackedType();
        REQUIRE_FALSE(opt0 < opt1);
        REQUIRE_FALSE(opt0 > opt1);
        REQUIRE_FALSE(opt0 <= opt1);
        REQUIRE_FALSE(opt0 >= opt1);

        opt1 = TrackedType();
        REQUIRE(opt0 < opt1);
        REQUIRE(opt0 > opt1);
        REQUIRE(opt0 <= opt1);
        REQUIRE(opt0 >= opt1);
    }

    SECTION("Reference Types")
    {
        TrackedType val;
        Option<TrackedType&> opt = val;

        REQUIRE(&val == &opt.value());

        opt.value().lastOp = TrackedType::EOperation::Destructor;
        REQUIRE(val.lastOp == TrackedType::EOperation::Destructor);

        using Opt = Option<int&>;

        STATIC_REQUIRE(RDefaultConstructible<Opt>);
        STATIC_REQUIRE(RCopyConstructible<Opt>);
        STATIC_REQUIRE(RMoveConstructible<Opt>);
        STATIC_REQUIRE(RCopyAssignable<Opt>);
        STATIC_REQUIRE(RMoveAssignable<Opt>);
        STATIC_REQUIRE(RDestructible<Opt>);
    }
}