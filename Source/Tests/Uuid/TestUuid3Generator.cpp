#include "catch2/catch_all.hpp"

#include "Atom/Uuid/Uuid3Generator.h"

using namespace Atom;

TEST_CASE("Atom::Uuid::Uuid3Generator")
{
    Uuid3Generator generator(UuidNamespaces::Oid);
    Uuid uuid1 = generator.generate(MakeRange("Hello"));
    Uuid uuid2 = generator.generate(MakeRange("Hello"));

    CHECK(uuid1 != Uuid::Null);
    CHECK(uuid1 == uuid2);

    CHECK(uuid1.getVersion() == EUuidVersion::V3);
    CHECK(uuid2.getVersion() == EUuidVersion::V3);
}
