#include "catch2/catch_all.hpp"

#include "Atom/Str/StrFormatter.h"

using namespace Atom;

TEST_CASE("Atom::StrFormatter")
{
    Str out = StrFmter().Fmt("Hello {}", StrView{ "World." });
    CHECK(out == Range("Hello World."));

    // StrView fmt = "Hello {}";
    // Str out1 = StrFmter().Fmt(RuntimeFormatStr(fmt), "World.");
    // CHECK(out1 == "Hello World.");
}
