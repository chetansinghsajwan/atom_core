#include "catch2/catch_all.hpp"

#include "Atom/Str/StrFormatter.h"

using namespace Atom;

TEST_CASE("Atom::StrFormatter")
{
    Str out = StrFmter().Fmt(TEXT("Hello {}"), StrView{ TEXT("World.") });
    CHECK(out == Range(TEXT("Hello World.")));

    // StrView fmt = TEXT("Hello {}");
    // Str out1 = StrFmter().Fmt(RuntimeFormatStr(fmt), TEXT("World."));
    // CHECK(out1 == TEXT("Hello World."));
}