#include "catch2/catch_all.hpp"

#include "Atom/String/StringConverter.h"

using namespace Atom;

TEST_CASE("Atom.String.StringConverter")
{
    String str = TEXT("Hello.");
    const String& strRef = str;

    StringConverter<String> converter;
    String newStr = converter.Convert(strRef);
}