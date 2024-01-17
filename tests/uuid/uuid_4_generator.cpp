import atom.core;
#include "catch2/catch_test_macros.hpp"

using namespace atom;

TEST_CASE("atom::uuid::uuid4generator")
{
    uuid4generator generator;
    uuid uuid1 = generator.generate();
    uuid uuid2 = generator.generate();

    REQUIRE(uuid1 != uuid::null);
    REQUIRE(uuid1 != uuid2);

    REQUIRE(uuid1.get_version() == euuid_version::v4);
    REQUIRE(uuid2.get_version() == euuid_version::v4);
}
