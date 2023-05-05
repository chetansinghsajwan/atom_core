#include "catch2/catch_all.hpp"

#include "Atom/Uuid/Uuid4Generator.h"
#include "Atom/Uuid/UuidUtils.h"

using namespace Atom;

TEST_CASE("Atom::Uuid::Uuid4Generator")
{
    Uuid4Generator generator;
    Uuid uuid1 = generator.Generate();
    Uuid uuid2 = generator.Generate();

    CHECK(uuid1 != Uuid::Null);
    CHECK(uuid1 != uuid2);

    CHECK(UuidUtils::GetVersion(uuid1) == EUuidVersion::V4);
    CHECK(UuidUtils::GetVersion(uuid2) == EUuidVersion::V4);
}