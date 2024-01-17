import atom.core;
#include "catch2/catch_test_macros.hpp"

using namespace atom;

TEST_CASE("atom::uuid::uuid_parser")
{
    constexpr char str[] = "00010203-0405-0607-0809-0a0b0c0d0e0f";

    uuid uuid = uuid_parser().parse(make_range(str));

    constexpr class uuid expected = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

    REQUIRE(uuid.eq(expected));
}
