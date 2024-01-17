import atom.core;
#include "catch2/catch_test_macros.hpp"

using namespace atom;

TEST_CASE("atom.containers.dynamic_array")
{
    dynamic_array<i32> arr;

    REQUIRE(arr.count() == 0);
    REQUIRE(arr.data() == nullptr);

    //     REQUIRE(arr.begin()         == arr.end());
    //     REQUIRE(arr.const_begin()    == arr.const_end());
    //     REQUIRE(arr.rev_begin()      == arr.rev_end());
    //     REQUIRE(arr.const_rev_begin() == arr.const_rev_end());
    //
    //     arr.insert_at(arr.begin(), 0);
    //     arr.insert_at(arr.begin(), { 1, 2 });
    //
    //     arr.insert_front(0);
    //     arr.insert_front({ 1, 2 });
    //
    //     arr.insert_back(0);
    //     arr.insert_back({ 1, 2 });
    //
    //     arr.remove_at(arr.begin());
    //     arr.remove_range({ arr.begin(), arr.end() });
}
