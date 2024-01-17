import atom.core;
#include "catch2/catch_test_macros.hpp"

using namespace atom;

TEST_CASE("atom.string.string_converter")
{
    string str = make_range("hello.");
    const string& str_ref = str;

    string_converter<string> converter;
    string new_string = converter.convert(str_ref);
}
