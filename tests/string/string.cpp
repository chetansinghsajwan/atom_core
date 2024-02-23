import atom.core;
#include "catch2/catch_test_macros.hpp"

using namespace atom;

TEST_CASE("atom.core.string")
{
    string str;

    REQUIRE(str.count() == 0);
    REQUIRE(str.data() == nullptr);

    REQUIRE(str.iter() == str.iter_end());
    REQUIRE(str.mut_iter() == str.mut_iter_end());

    string str0(move(str));

    //     str.insert_at(str.iter(), 0);
    //     str.insert_at(str.iter(), { 1, 2 });
    //
    //     str.insert_front(0);
    //     str.insert_front({ 1, 2 });
    //
    //     str.insert_back(0);
    //     str.insert_back({ 1, 2 });
    //
    //     str.remove_at(str.iter());
    //     str.remove_range({ str.iter(), str.end() });
}
