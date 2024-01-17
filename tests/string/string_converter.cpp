import atom.core;
#include "catch2/catch_test_macros.hpp"

using namespace Atom;

TEST_CASE("Atom.String.StringConverter")
{
    String str = MakeRange("Hello.");
    const String& strRef = str;

    StringConverter<String> converter;
    String newString = converter.Convert(strRef);
}
