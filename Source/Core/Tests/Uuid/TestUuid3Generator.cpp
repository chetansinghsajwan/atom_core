#include "catch2/catch_all.hpp"

#include "Atom/Uuid/Uuid3Generator.h"

using namespace Atom;

TEST_CASE("Atom/Uuid/Uuid3Generator")
{
    Uuid3Generator generator(UuidNamespaces::Oid);
    Uuid uuid1 = generator.Generate("Hello");
    Uuid uuid2 = generator.Generate("Hello");

    CHECK(uuid1 != Uuid{ });
    CHECK(uuid1 == uuid2);
}