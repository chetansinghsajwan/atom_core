#include "catch2/catch_all.hpp"
#include "Atom/Range/IterWrappers.h"
#include "Atom/Range/ArrIter.h"

using namespace Atom;

TEST_CASE("Atom.Range.IterWrappers", "[.compilation]")
{
    using IterWrapMock = IterWrap<MutArrIter<int>>;
    using MutIterWrapMock = MutIterWrap<MutArrIter<int>>;
    using FwdIterWrapMock = FwdIterWrap<MutArrIter<int>>;
    using MutFwdIterWrapMock = MutFwdIterWrap<MutArrIter<int>>;
    using BidiIterWrapMock = BidiIterWrap<MutArrIter<int>>;
    using MutBidiIterWrapMock = MutBidiIterWrap<MutArrIter<int>>;
    using JumpIterWrapMock = JumpIterWrap<MutArrIter<int>>;
    using MutJumpIterWrapMock = MutJumpIterWrap<MutArrIter<int>>;

    STATIC_CHECK(RIter<IterWrapMock>);
    STATIC_CHECK_FALSE(RMutIter<IterWrapMock>);
    STATIC_CHECK_FALSE(RFwdIter<IterWrapMock>);

    STATIC_CHECK(RMutIter<MutIterWrapMock>);
    STATIC_CHECK_FALSE(RMutFwdIter<IterWrapMock>);

    STATIC_CHECK(RFwdIter<FwdIterWrapMock>);
    STATIC_CHECK_FALSE(RMutFwdIter<FwdIterWrapMock>);
    STATIC_CHECK_FALSE(RBidiIter<IterWrapMock>);

    STATIC_CHECK(RMutFwdIter<MutFwdIterWrapMock>);
    STATIC_CHECK_FALSE(RMutBidiIter<IterWrapMock>);

    STATIC_CHECK(RBidiIter<BidiIterWrapMock>);
    STATIC_CHECK_FALSE(RMutBidiIter<BidiIterWrapMock>);
    STATIC_CHECK_FALSE(RJumpIter<IterWrapMock>);

    STATIC_CHECK(RMutBidiIter<MutBidiIterWrapMock>);
    STATIC_CHECK_FALSE(RMutJumpIter<IterWrapMock>);

    STATIC_CHECK(RJumpIter<JumpIterWrapMock>);
    STATIC_CHECK_FALSE(RMutJumpIter<JumpIterWrapMock>);
    STATIC_CHECK_FALSE(RMutArrIter<IterWrapMock>);

    STATIC_CHECK(RMutJumpIter<MutJumpIterWrapMock>);
    STATIC_CHECK_FALSE(RMutArrIter<IterWrapMock>);
}

TEST_CASE("Atom.Range.IterWrappers", "[.interface]")
{
    struct CustomIter: MutArrIter<int>
    {
        using Base = MutArrIter<int>;

        constexpr CustomIter() { }

        constexpr CustomIter(int, char, float, const char*) { }

        constexpr CustomIter(const Base& base) noexcept:
            Base{ base } { }
    };

    SECTION("")
    {
        MutArrIter<int> iter;

        IterWrap iterWrap{ iter };
        MutIterWrap mutIterWrap{ iter };
        FwdIterWrap fwdIterWrap{ iter };
        MutFwdIterWrap mutFwdIterWrap{ iter };
        BidiIterWrap bidiIterWrap{ iter };
        MutBidiIterWrap mutBidiIterWrap{ iter };
        JumpIterWrap jumpIterWrap{ iter };
        MutJumpIterWrap mutJumpIterWrap{ iter };
    }

    SECTION("")
    {
        using IterWrapMock = IterWrap<CustomIter>;
        using MutIterWrapMock = MutIterWrap<CustomIter>;
        using FwdIterWrapMock = FwdIterWrap<CustomIter>;
        using MutFwdIterWrapMock = MutFwdIterWrap<CustomIter>;
        using BidiIterWrapMock = BidiIterWrap<CustomIter>;
        using MutBidiIterWrapMock = MutBidiIterWrap<CustomIter>;
        using JumpIterWrapMock = JumpIterWrap<CustomIter>;
        using MutJumpIterWrapMock = MutJumpIterWrap<CustomIter>;

        IterWrapMock iterWrap{ 0, 'a', 0.0f, "hello" };
        MutIterWrapMock mutIterWrap{ 0, 'a', 0.0f, "hello" };
        FwdIterWrapMock fwdIterWrap{ 0, 'a', 0.0f, "hello" };
        MutFwdIterWrapMock mutFwdIterWrap{ 0, 'a', 0.0f, "hello" };
        BidiIterWrapMock bidiIterWrap{ 0, 'a', 0.0f, "hello" };
        MutBidiIterWrapMock mutBidiIterWrap{ 0, 'a', 0.0f, "hello" };
        JumpIterWrapMock jumpIterWrap{ 0, 'a', 0.0f, "hello" };
        MutJumpIterWrapMock mutJumpIterWrap{ 0, 'a', 0.0f, "hello" };
    }
}