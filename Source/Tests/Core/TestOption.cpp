#include "Atom/Core/Option.h"
#include "Helpers/CustomType.h"
#include "Helpers/TrackedType.h"
#include "catch2/catch_all.hpp"

using namespace Atom;
using namespace Atom::Tests;

TEST_CASE("Atom.Core.Option")
{
    SECTION("Default Constructor")
    {
        // Default constructor doesn't constructs the object, so it's not affected by the object's
        // default constructor.
        STATIC_REQUIRE(RDefaultConstructible<Option<NonDefaultConstructibleMock>>);

        Option<TrackedType> opt;

        REQUIRE(opt.isNull());
    }

    SECTION("Null Constructor")
    {
        Option<i32> opt = nullopt;

        REQUIRE(opt.isNull());
    }

    SECTION("Null Operator")
    {
        Option<i32> opt = i32(0);

        opt = nullopt;
        REQUIRE(opt.isNull());
    }

    SECTION("Value Copy Constructor")
    {
        TrackedType obj = TrackedType();
        Option<TrackedType> opt = obj;

        REQUIRE(opt.isValue());
        REQUIRE(opt.get().lastOp == TrackedType::EOperation::CopyConstructor);
    }

    SECTION("Value Copy Operator")
    {
        TrackedType obj = TrackedType();
        Option<TrackedType> opt;
        opt = obj;

        REQUIRE(opt.isValue());
        REQUIRE(opt.get().lastOp == TrackedType::EOperation::CopyConstructor);

        opt = obj;

        REQUIRE(opt.isValue());
        REQUIRE(opt.get().lastOp == TrackedType::EOperation::CopyOperator);
    }

    SECTION("Value Move Constructor")
    {
        TrackedType obj = TrackedType();
        Option<TrackedType> opt = mov(obj);

        REQUIRE(opt.isValue());
        REQUIRE(opt.get().lastOp == TrackedType::EOperation::MoveConstructor);
    }

    SECTION("Value Move Operator")
    {
        TrackedType obj = TrackedType();
        Option<TrackedType> opt;
        opt = mov(obj);

        REQUIRE(opt.isValue());
        REQUIRE(opt.get().lastOp == TrackedType::EOperation::MoveConstructor);

        opt = mov(obj);

        REQUIRE(opt.isValue());
        REQUIRE(opt.get().lastOp == TrackedType::EOperation::MoveOperator);
    }

    SECTION("Copy Constructor")
    {
        STATIC_REQUIRE(RTriviallyCopyConstructible<Option<TriviallyCopyConstructibleMock>>);
        STATIC_REQUIRE(RCopyConstructible<Option<CopyConstructibleMock>>);

        const Option<TrackedType> opt0 = TrackedType();
        Option<TrackedType> opt1 = opt0;

        REQUIRE(opt0.isValue());
        REQUIRE(opt0.get().lastOp == TrackedType::EOperation::CopyConstructorAsThat);

        REQUIRE(opt1.isValue());
        REQUIRE(opt1.get().lastOp == TrackedType::EOperation::CopyConstructor);
    }

    SECTION("Copy Operator")
    {
        STATIC_REQUIRE(RTriviallyCopyAssignable<Option<TriviallyCopyableMock>>);
        STATIC_REQUIRE(RCopyAssignable<Option<CopyableMock>>);

        const Option<TrackedType> opt0 = TrackedType();
        Option<TrackedType> opt1 = TrackedType();
        opt1 = opt0;

        REQUIRE(opt0.isValue());
        REQUIRE(opt0.get().lastOp == TrackedType::EOperation::CopyOperatorAsThat);

        REQUIRE(opt1.isValue());
        REQUIRE(opt1.get().lastOp == TrackedType::EOperation::CopyOperator);

        const Option<TrackedType> opt2;
        opt1 = opt2;

        REQUIRE(opt1.isNull());
    }

    SECTION("Move Constructor")
    {
        STATIC_REQUIRE(RTriviallyMoveConstructible<Option<TriviallyMoveConstructibleMock>>);
        STATIC_REQUIRE(RMoveConstructible<Option<MoveConstructibleMock>>);

        Option<TrackedType> opt0 = TrackedType();
        Option<TrackedType> opt1 = mov(opt0);

        REQUIRE(opt0.isValue());
        REQUIRE(opt0.get().lastOp == TrackedType::EOperation::MoveConstructorAsThat);

        REQUIRE(opt1.isValue());
        REQUIRE(opt1.get().lastOp == TrackedType::EOperation::MoveConstructor);
    }

    SECTION("Move Operator")
    {
        STATIC_REQUIRE(RTriviallyMoveAssignable<Option<TriviallyMoveableMock>>);
        STATIC_REQUIRE(RMoveAssignable<Option<MoveableMock>>);

        Option<TrackedType> opt0 = TrackedType();
        Option<TrackedType> opt1 = TrackedType();
        opt1 = mov(opt0);

        REQUIRE(opt0.isValue());
        REQUIRE(opt0.get().lastOp == TrackedType::EOperation::MoveOperatorAsThat);

        REQUIRE(opt1.isValue());
        REQUIRE(opt1.get().lastOp == TrackedType::EOperation::MoveOperator);

        Option<TrackedType> opt2;
        opt1 = mov(opt2);

        REQUIRE(opt1.isNull());
    }

    SECTION("Destructor")
    {
        STATIC_REQUIRE(RTriviallyDestructible<Option<TriviallyDestructibleMock>>);
        STATIC_REQUIRE(RDestructible<Option<DestructibleMock>>);

        TrackedType::EOperation* lastOp;

        {
            Option<TrackedType> opt = TrackedType();
            lastOp = &opt.get().lastOp;
        }

        REQUIRE(*lastOp == TrackedType::EOperation::Destructor);
    }

    SECTION("Emplace")
    {
        class Type
        {
        public:
            Type(i32 a, Char b, f32 c):
                a{ a }, b{ b }, c{ c }
            {}

        public:
            i32 a;
            Char b;
            f32 c;
        };

        Option<Type> opt;
        opt.emplace(9, 'a', 0.99f);

        REQUIRE(opt.isValue());
        REQUIRE(opt.get().a == 9);
        REQUIRE(opt.get().b == 'a');
        REQUIRE(opt.get().c == 0.99f);
    }

    SECTION("Value access")
    {
        SECTION("value()")
        {
            Option<i32> opt = i32(10);

            REQUIRE(opt.get() == 10);
        }

        SECTION("value()")
        {
            Option<i32> opt;
            REQUIRE(opt.getOr(99) == 99);

            opt = i32(10);
            REQUIRE(opt.getOr(99) == 10);
        }

        SECTION("valueOrInvoke()")
        {
            auto invoke = [&]() -> i32 { return 99; };

            Option<i32> opt;
            REQUIRE(opt.getOrInvoke(invoke) == 99);

            opt = i32(10);
            REQUIRE(opt.getOrInvoke(invoke) == 10);
        }

        SECTION("valueOrDefault()")
        {
            Option<i32> opt;
            REQUIRE(opt.getOrDefault() == 0);

            opt = i32(99);
            REQUIRE(opt.getOrDefault() == 99);
        }
    }

    SECTION("Comparision")
    {
        STATIC_REQUIRE(REqualityComparable<Option<EqualityComparableMock>>);
        STATIC_REQUIRE(RComparable<Option<ComparableMock>>);

        Option<TrackedType> opt0;
        Option<TrackedType> opt1;

        // Both have null state, so they are compared equal.
        REQUIRE(opt0.eq(opt1));

        // Both have null state, they will not be compared.
        REQUIRE(not opt0.lt(opt1));
        REQUIRE(not opt0.gt(opt1));
        REQUIRE(not opt0.le(opt1));
        REQUIRE(not opt0.ge(opt1));

        // If either have them null state, still they will not be compared.
        opt0 = TrackedType();
        opt0->lastOp = TrackedType::EOperation::None;

        REQUIRE(not opt0.eq(opt1));
        REQUIRE(opt0.get().lastOp == TrackedType::EOperation::None);

        REQUIRE(opt0.ne(opt1));
        REQUIRE(opt0.get().lastOp == TrackedType::EOperation::None);

        REQUIRE(not opt0.lt(opt1));
        REQUIRE(opt0.get().lastOp == TrackedType::EOperation::None);

        REQUIRE(not opt0.gt(opt1));
        REQUIRE(opt0.get().lastOp == TrackedType::EOperation::None);

        REQUIRE(not opt0.le(opt1));
        REQUIRE(opt0.get().lastOp == TrackedType::EOperation::None);

        REQUIRE(not opt0.ge(opt1));
        REQUIRE(opt0.get().lastOp == TrackedType::EOperation::None);

        // They will be compared if they both have value state.
        opt1 = TrackedType();
        opt1->lastOp = TrackedType::EOperation::None;

        REQUIRE(opt0.eq(opt1));
        REQUIRE(opt0.get().lastOp == TrackedType::EOperation::EqualOperator);
        REQUIRE(opt1.get().lastOp == TrackedType::EOperation::EqualOperator);

        REQUIRE(not opt0.ne(opt1));
        REQUIRE(opt0.get().lastOp == TrackedType::EOperation::EqualOperator);
        REQUIRE(opt1.get().lastOp == TrackedType::EOperation::EqualOperator);

        REQUIRE(opt0.lt(opt1));
        REQUIRE(opt0.get().lastOp == TrackedType::EOperation::LessThanOperator);
        REQUIRE(opt1.get().lastOp == TrackedType::EOperation::LessThanOperator);

        REQUIRE(opt0.gt(opt1));
        REQUIRE(opt0.get().lastOp == TrackedType::EOperation::GreaterThanOperator);
        REQUIRE(opt1.get().lastOp == TrackedType::EOperation::GreaterThanOperator);

        REQUIRE(opt0.le(opt1));
        REQUIRE(opt0.get().lastOp == TrackedType::EOperation::LessThanOrEqualOperator);
        REQUIRE(opt1.get().lastOp == TrackedType::EOperation::LessThanOrEqualOperator);

        REQUIRE(opt0.ge(opt1));
        REQUIRE(opt0.get().lastOp == TrackedType::EOperation::GreaterThanOrEqualOperator);
        REQUIRE(opt1.get().lastOp == TrackedType::EOperation::GreaterThanOrEqualOperator);
    }

    SECTION("Reset")
    {
        TrackedType::EOperation* lastOp;
        Option<TrackedType> opt = TrackedType();
        lastOp = &opt.get().lastOp;

        opt.reset();

        REQUIRE(opt.isNull());
        REQUIRE(*lastOp == TrackedType::EOperation::Destructor);
    }
}
