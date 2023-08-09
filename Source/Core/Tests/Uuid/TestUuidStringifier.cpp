#include "catch2/catch_all.hpp"

#include "Atom/Uuid/UuidStringifier.h"

using namespace Atom;

TEST_CASE("Atom::Uuid::UuidStringifier")
{
    Uuid uuid = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };

    Str str = UuidStringifier()
        .toStr(uuid);

    cexpr StrView expected = "00010203-0405-0607-0809-101112131415";

    CHECK(str == expected);
}
