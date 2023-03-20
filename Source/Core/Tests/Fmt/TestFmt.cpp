#include "catch2/catch_all.hpp"

#include "Atom/Fmt/Fmt.h"

using namespace Atom;

TEST_CASE("Atom/Fmt")
{
    String out = Fmt::Format(TEXT("Hello {}"), TEXT("World."));

    CHECK(out == TEXT("Hello World."));
}