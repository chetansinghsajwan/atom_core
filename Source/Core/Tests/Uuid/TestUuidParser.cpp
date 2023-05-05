#include "catch2/catch_all.hpp"

#include "Atom/Uuid/UuidParser.h"

using namespace Atom;

TEST_CASE("Atom::Uuid::UuidParser")
{
    constexpr Char str[] = TEXT("00010203-0405-0607-0809-101112131415");

    Uuid uuid = UuidParser()
        .Parse(str);

    constexpr Uuid expected = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };

    CHECK(uuid == expected);
}
