#include "catch2/catch_all.hpp"

#include "Atom/Uuid/Uuid4Generator.h"
#include "Atom/Uuid/UuidHelper.h"

using namespace Atom;

TEST_CASE("Atom::Uuid::Uuid4Generator")
{
    Uuid4Generator generator;
    Uuid uuid1 = generator.generate();
    Uuid uuid2 = generator.generate();

    CHECK(uuid1 != Uuid::Null);
    CHECK(uuid1 != uuid2);

    CHECK(UuidHelper().getVersion(uuid1) == EUuidVersion::V4);
    CHECK(UuidHelper().getVersion(uuid2) == EUuidVersion::V4);
}