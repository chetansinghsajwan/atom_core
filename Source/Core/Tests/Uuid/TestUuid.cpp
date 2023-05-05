#include "catch2/catch_all.hpp"

#include "Atom/Uuid/Uuid.h"

using namespace Atom;

TEST_CASE("Atom::Uuid::Uuid")
{
    Uuid uuid1 = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
    Uuid uuid2 = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
    Uuid uuid3 = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,14 };

    CHECK(uuid1 != Uuid::Null);
    CHECK(uuid1 == uuid2);
    CHECK(uuid1 != uuid3);
}