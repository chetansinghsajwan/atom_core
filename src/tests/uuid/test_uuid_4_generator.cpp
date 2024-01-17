import atom.core;
#include "catch2/catch_test_macros.hpp"

using namespace Atom;

TEST_CASE("Atom::Uuid::Uuid4Generator")
{
    Uuid4Generator generator;
    Uuid uuid1 = generator.generate();
    Uuid uuid2 = generator.generate();

    CHECK(uuid1 != Uuid::Null);
    CHECK(uuid1 != uuid2);

    CHECK(uuid1.getVersion() == EUuidVersion::V4);
    CHECK(uuid2.getVersion() == EUuidVersion::V4);
}
