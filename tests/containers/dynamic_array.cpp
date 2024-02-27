import atom.core;
#include "catch2/catch_test_macros.hpp"

using namespace atom;

TEST_CASE("atom.core.dynamic_array")
{
    dynamic_array<i32> arr;

    REQUIRE(arr.get_count() == 0);
    REQUIRE(arr.get_data() == nullptr);
}
