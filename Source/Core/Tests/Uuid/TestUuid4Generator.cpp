#include "catch2/catch_all.hpp"

#include "Atom/Uuid/Uuid4Generator.h"

using namespace Atom;

TEST_CASE("Atom/Uuid/Uuid4Generator")
{
    Uuid4Generator generator;

    Uuid uuid1 = generator.Generate();
    Uuid uuid2 = generator.Generate();

    CHECK(uuid1 != Uuid{ });
    CHECK(uuid1 != uuid2);
}