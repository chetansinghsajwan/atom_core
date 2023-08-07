#include "catch2/catch_all.hpp"

#include "Atom/Str/StrConverter.h"

using namespace Atom;

TEST_CASE("Atom.Str.StrConverter")
{
    Str str = "Hello.";
    const Str& strRef = str;

    StrConverter<Str> converter;
    Str newStr = converter.Convert(strRef);
}