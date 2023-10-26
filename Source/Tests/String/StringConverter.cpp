#include "catch2/catch_all.hpp"

#include "Atom/String/StringConverter.h"
#include "Atom/Contracts.h"

using namespace Atom;

TEST_CASE("Atom.String.StringConverter")
{
    String str = "Hello.";
    const String& strRef = str;

    StringConverter<String> converter;
    String newString = converter.Convert(strRef);
}
