#include "catch2/catch_all.hpp"

#include "Atom/String/StringFormatter.h"

using namespace Atom;

TEST_CASE("Atom::StringFormatter")
{
    String out = StrFmter().Fmt(TEXT("Hello {}"), StringView{ TEXT("World.") });
    CHECK(out == TEXT("Hello World."));

    // StringView fmt = TEXT("Hello {}");
    // String out1 = StrFmter().Fmt(RuntimeFormatString(fmt), TEXT("World."));
    // CHECK(out1 == TEXT("Hello World."));
}