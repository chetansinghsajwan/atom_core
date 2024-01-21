import atom.core;
#include "catch2/catch_test_macros.hpp"

using namespace atom;

TEST_CASE("atom::uuid::uuid")
{
    SECTION("basics")
    {
        uuid uuid1 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
        uuid uuid2 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
        uuid uuid3 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 14 };

        REQUIRE(not uuid1.eq(uuid::null));
        REQUIRE(uuid1.eq(uuid2));
        REQUIRE(not uuid1.eq(uuid3));
    }

    SECTION("stringification")
    {
        uuid uuid = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
        string_view expected = make_range("00010203-0405-0607-0809-101112131415");

        REQUIRE(uuid.to_string() == expected);
    }
}
