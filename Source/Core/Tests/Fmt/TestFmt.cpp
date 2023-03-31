#include "catch2/catch_all.hpp"

#include "Atom/Fmt.h"

using namespace Atom;

TEST_CASE("Atom/Fmt")
{
    String out = Fmt::Format(TEXT("Hello {}"), StringView{ TEXT("World.") });
    CHECK(out == TEXT("Hello World."));

    StringView fmt = TEXT("Hello {}");
    String out1 = Fmt::Format(Fmt::Runtime(fmt), TEXT("World."));
    CHECK(out1 == TEXT("Hello World."));
}