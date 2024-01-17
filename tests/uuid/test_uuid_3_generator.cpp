import atom.core;
#include "catch2/catch_test_macros.hpp"

using namespace atom;

TEST_CASE("atom::uuid::uuid3generator")
{
    uuid3generator generator(uuid_namespaces::oid);
    uuid uuid1 = generator.generate(make_range("hello"));
    uuid uuid2 = generator.generate(make_range("hello"));

    REQUIRE(uuid1 != uuid::null);
    REQUIRE(uuid1 == uuid2);

    REQUIRE(uuid1.get_version() == euuid_version::v3);
    REQUIRE(uuid2.get_version() == euuid_version::v3);
}
