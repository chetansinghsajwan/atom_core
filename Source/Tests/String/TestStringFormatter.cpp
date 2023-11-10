#include "catch2/catch_all.hpp"

#include "Atom/String/StringFormatter.h"

using namespace Atom;

TEST_CASE("Atom::StringFormatter")
{
    // String out = StringFmter().Fmt("Hello {}", StringView(MakeRange("World.")));
    // CHECK(out == MakeRange("Hello World."));

    // StringView fmt = "Hello {}";
    // String out1 = StringFmter().Fmt(RuntimeFormatString(fmt), "World.");
    // CHECK(out1 == "Hello World.");
}
