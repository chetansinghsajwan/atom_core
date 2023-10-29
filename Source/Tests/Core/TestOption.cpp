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
        STATIC_REQUIRE(RDefaultConstructible<Option<NonDefaultConstructibleMock>>);

        Option<TrackedType> opt;

        REQUIRE(not opt.isValue());
    }

    SECTION("Value Constructor")
    {
        Option<TrackedType> opt = TrackedType();

        REQUIRE(opt.isValue());
        REQUIRE(opt.value().lastOp == TrackedType::EOperation::MoveConstructor);
    }

    SECTION("Value Assignment")
    {
        Option<TrackedType> opt;
        opt = TrackedType();

        REQUIRE(opt.isValue());
        REQUIRE(opt.value().lastOp == TrackedType::EOperation::MoveOperator);
    }

    SECTION("Copy Constructor")
    {
        STATIC_REQUIRE(RTriviallyCopyConstructible<Option<TriviallyCopyConstructibleMock>>);

        STATIC_REQUIRE(RCopyConstructible<Option<CopyConstructibleMock>>);

        Option<TrackedType> opt0 = TrackedType();
        Option<TrackedType> opt1 = opt0;

        REQUIRE(opt0.isValue());
        REQUIRE(opt0.value().lastOp == TrackedType::EOperation::CopyConstructorAsThat);

        REQUIRE(opt1.isValue());
        REQUIRE(opt1.value().lastOp == TrackedType::EOperation::CopyConstructor);
    }

    SECTION("Copy Assignment")
    {
        STATIC_REQUIRE(RTriviallyCopyAssignable<Option<TriviallyCopyableMock>>);

        STATIC_REQUIRE(RCopyAssignable<Option<CopyableMock>>);

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

    SECTION("Move Constructor")
    {
        STATIC_REQUIRE(RTriviallyMoveConstructible<Option<TriviallyMoveConstructibleMock>>);

        STATIC_REQUIRE(RMoveConstructible<Option<MoveConstructibleMock>>);

        Option<TrackedType> opt0 = TrackedType();
        Option<TrackedType> opt1 = mov(opt0);

        REQUIRE(opt0.isValue());
        REQUIRE(opt0.value().lastOp == TrackedType::EOperation::MoveConstructorAsThat);

        REQUIRE(opt1.isValue());
        REQUIRE(opt1.value().lastOp == TrackedType::EOperation::MoveConstructor);
    }

    SECTION("Move Assignment")
    {
        STATIC_REQUIRE(RTriviallyMoveAssignable<Option<TriviallyMoveableMock>>);

        STATIC_REQUIRE(RMoveAssignable<Option<MoveableMock>>);

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

    SECTION("Destructor")
    {
        STATIC_REQUIRE(RTriviallyDestructible<Option<TriviallyDestructibleMock>>);

        STATIC_REQUIRE(RDestructible<Option<DestructibleMock>>);

        TrackedType::EOperation* lastOp;

        {
            Option<TrackedType> opt = TrackedType();
            lastOp = &opt.value().lastOp;
        }

        REQUIRE(*lastOp == TrackedType::EOperation::Destructor);
    }

    SECTION("Access Value using value(), operator*(), operator->()")
    {
        class Overloads
        {
        public:
            static constexpr usize Test(TrackedType&)
            {
                return 0;
            }

            static constexpr usize Test(const TrackedType&)
            {
                return 1;
            }

            static constexpr usize Test(TrackedType&&)
            {
                return 2;
            }
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
            Type(i32 a, char b, f32 c):
                a{ a }, b{ b }, c{ c }
            {}

        public:
            i32 a;
            char b;
            f32 c;
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

    SECTION("Comparision with `Option`")
    {
        STATIC_REQUIRE(REqualityComparable<Option<EqualityComparableMock>>);

        STATIC_REQUIRE(RComparable<Option<ComparableMock>>);

        Option<TrackedType> opt0;
        Option<TrackedType> opt1;

        // Both have null state, so they are compared equal.
        REQUIRE(opt0 == opt1);
        REQUIRE_FALSE(opt0 != opt1);

        // Both have null state, they will not be compared.
        REQUIRE_FALSE(opt0 < opt1);
        REQUIRE_FALSE(opt0 > opt1);
        REQUIRE_FALSE(opt0 <= opt1);
        REQUIRE_FALSE(opt0 >= opt1);

        // If either have them null state, still they will not be compared.
        opt0 = TrackedType();
        opt0->lastOp = TrackedType::EOperation::None;

        REQUIRE_FALSE(opt0 == opt1);
        REQUIRE(opt0.value().lastOp == TrackedType::EOperation::None);

        REQUIRE(opt0 != opt1);
        REQUIRE(opt0.value().lastOp == TrackedType::EOperation::None);

        REQUIRE_FALSE(opt0 < opt1);
        REQUIRE(opt0.value().lastOp == TrackedType::EOperation::None);

        REQUIRE_FALSE(opt0 > opt1);
        REQUIRE(opt0.value().lastOp == TrackedType::EOperation::None);

        REQUIRE_FALSE(opt0 <= opt1);
        REQUIRE(opt0.value().lastOp == TrackedType::EOperation::None);

        REQUIRE_FALSE(opt0 >= opt1);
        REQUIRE(opt0.value().lastOp == TrackedType::EOperation::None);

        // They will be compared if they both have value state.
        opt1 = TrackedType();
        opt1->lastOp = TrackedType::EOperation::None;

        REQUIRE(opt0 == opt1);
        REQUIRE(opt0.value().lastOp == TrackedType::EOperation::EqualOperator);
        REQUIRE(opt1.value().lastOp == TrackedType::EOperation::EqualOperator);

        REQUIRE_FALSE(opt0 != opt1);
        REQUIRE(opt0.value().lastOp == TrackedType::EOperation::EqualOperator);
        REQUIRE(opt1.value().lastOp == TrackedType::EOperation::EqualOperator);

        REQUIRE(opt0 < opt1);
        REQUIRE(opt0.value().lastOp == TrackedType::EOperation::LessThanOperator);
        REQUIRE(opt1.value().lastOp == TrackedType::EOperation::LessThanOperator);

        REQUIRE(opt0 > opt1);
        REQUIRE(opt0.value().lastOp == TrackedType::EOperation::GreaterThanOperator);
        REQUIRE(opt1.value().lastOp == TrackedType::EOperation::GreaterThanOperator);

        REQUIRE(opt0 <= opt1);
        REQUIRE(opt0.value().lastOp == TrackedType::EOperation::LessThanOrEqualOperator);
        REQUIRE(opt1.value().lastOp == TrackedType::EOperation::LessThanOrEqualOperator);

        REQUIRE(opt0 >= opt1);
        REQUIRE(opt0.value().lastOp == TrackedType::EOperation::GreaterThanOrEqualOperator);
        REQUIRE(opt1.value().lastOp == TrackedType::EOperation::GreaterThanOrEqualOperator);
    }

    SECTION("Comparision with `nullopt`")
    {
        Option<TrackedType> opt;

        // `opt` has null state.
        REQUIRE(opt == nullopt);

        opt = TrackedType();
        opt->lastOp = TrackedType::EOperation::None;

        // `opt` doesn't have null state anymore.
        REQUIRE(opt != nullopt);

        // The value isn't compared in this comparision.
        REQUIRE(opt->lastOp == TrackedType::EOperation::None);
    }

    SECTION("Reference Types")
    {
        TrackedType val;
        Option<TrackedType&> opt = val;

        REQUIRE(&val == &opt.value());

        opt.value().lastOp = TrackedType::EOperation::Destructor;
        REQUIRE(val.lastOp == TrackedType::EOperation::Destructor);

        using Opt = Option<i32&>;

        STATIC_REQUIRE(RDefaultConstructible<Opt>);
        STATIC_REQUIRE(RCopyConstructible<Opt>);
        STATIC_REQUIRE(RMoveConstructible<Opt>);
        STATIC_REQUIRE(RCopyAssignable<Opt>);
        STATIC_REQUIRE(RMoveAssignable<Opt>);
        STATIC_REQUIRE(RDestructible<Opt>);
    }
}
