#include "catch2/catch_all.hpp"

#include "Atom/Uuid/UuidParser.h"

using namespace Atom;

TEST_CASE("Atom::Uuid::UuidParser")
{
    constexpr Char str[] = "00010203-0405-0607-0809-0A0B0C0D0E0F";

    Uuid uuid = UuidParser()
        .parse(Range(str));

    constexpr Uuid expected = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };

    CHECK(uuid == expected);
}
