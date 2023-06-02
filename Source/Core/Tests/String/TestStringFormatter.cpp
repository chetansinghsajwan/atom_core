#include "catch2/catch_all.hpp"

#include "Atom/String/StringFormatter.h"

using namespace Atom;

TEST_CASE("Atom::StringFormatter")
{
    String out = StringFormatter().Format(TEXT("Hello {}"), StringView{ TEXT("World.") });
    CHECK(out == TEXT("Hello World."));

    // StringView fmt = TEXT("Hello {}");
    // String out1 = StringFormatter().Format(RuntimeFormatString(fmt), TEXT("World."));
    // CHECK(out1 == TEXT("Hello World."));
}