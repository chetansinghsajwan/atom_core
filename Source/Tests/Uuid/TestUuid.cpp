#include "catch2/catch_all.hpp"

#include "Atom/Uuid/Uuid.h"
#include "Atom/String/StringView.h"

using namespace Atom;

TEST_CASE("Atom::Uuid::Uuid")
{
    SECTION("Basics")
    {
        Uuid uuid1 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
        Uuid uuid2 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
        Uuid uuid3 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 14 };

        CHECK(uuid1.ne(Uuid::Null));
        CHECK(uuid1.eq(uuid2));
        CHECK(uuid1.ne(uuid3));
    }

    SECTION("Stringification")
    {
        Uuid uuid = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
        StringView expected = MakeRange("00010203-0405-0607-0809-101112131415");

        CHECK(uuid.toString() == expected);
    }
}
