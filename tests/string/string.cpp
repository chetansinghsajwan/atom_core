#include "atom.core.h"
#include "catch2/catch_test_macros.hpp"

using namespace atom;

TEST_CASE("atom.core.string")
{
    string str;

    REQUIRE(str.get_count() == 0);
    REQUIRE(str.get_data() == nullptr);

    REQUIRE(str.get_iter() == str.get_iter_end());
    REQUIRE(str.get_mut_iter() == str.get_mut_iter_end());

    string str0(move(str));

    //     str.insert_at(str.get_iter(), 0);
    //     str.insert_at(str.get_iter(), { 1, 2 });
    //
    //     str.insert_front(0);
    //     str.insert_front({ 1, 2 });
    //
    //     str.insert_back(0);
    //     str.insert_back({ 1, 2 });
    //
    //     str.remove_at(str.get_iter());
    //     str.remove_range({ str.get_iter(), str.end() });
}
