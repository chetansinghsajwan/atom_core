import atom.core;
#include "catch2/catch_test_macros.hpp"

using namespace Atom;

TEST_CASE("Atom::Uuid::UuidParser")
{
    constexpr char str[] = "00010203-0405-0607-0809-0A0B0C0D0E0F";

    Uuid uuid = UuidParser().parse(MakeRange(str));

    constexpr Uuid expected = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

    CHECK(uuid.eq(expected));
}
