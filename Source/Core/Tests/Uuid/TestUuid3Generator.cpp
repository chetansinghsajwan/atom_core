#include "catch2/catch_all.hpp"

#include "Atom/Uuid/Uuid3Generator.h"
#include "Atom/Uuid/UuidHelper.h"

using namespace Atom;

TEST_CASE("Atom::Uuid::Uuid3Generator")
{
    Uuid3Generator generator(UuidNamespaces::Oid);
    Uuid uuid1 = generator.Generate("Hello");
    Uuid uuid2 = generator.Generate("Hello");

    CHECK(uuid1 != Uuid::Null);
    CHECK(uuid1 == uuid2);

    CHECK(UuidHelper().GetVersion(uuid1) == EUuidVersion::V3);
    CHECK(UuidHelper().GetVersion(uuid2) == EUuidVersion::V3);
}