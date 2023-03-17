#include "catch2/catch_all.hpp"

#include "Atom/Uuid/Uuid5Generator.h"

using namespace Atom;

TEST_CASE("Atom/Uuid/Uuid5Generator")
{
    Uuid5Generator generator(UuidNamespaces::Oid);
    Uuid uuid1 = generator.Generate("Hello");
    Uuid uuid2 = generator.Generate("Hello");

    CHECK(uuid1 != Uuid{ });
    CHECK(uuid1 == uuid2);
}