#include "catch2/catch_all.hpp"

#include "Atom/Uuid/Uuid3Generator.h"
#include "Atom/Uuid/UuidUtils.h"

using namespace Atom;

TEST_CASE("Atom::Uuid::Uuid3Generator")
{
    Uuid3Generator generator(UuidNamespaces::Oid);
    Uuid uuid1 = generator.Generate(TEXT("Hello"));
    Uuid uuid2 = generator.Generate(TEXT("Hello"));

    CHECK(uuid1 != Uuid::Null);
    CHECK(uuid1 == uuid2);

    CHECK(UuidUtils::GetVersion(uuid1) == EUuidVersion::V3);
    CHECK(UuidUtils::GetVersion(uuid2) == EUuidVersion::V3);
}