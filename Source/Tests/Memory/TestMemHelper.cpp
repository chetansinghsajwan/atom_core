#include "Atom/Memory/MemHelper.h"
#include "catch2/catch_all.hpp"
#include <memory>

using namespace Atom;

TEST_CASE("Atom::Memory::MemHelper")
{
    MemHelper helper;

    MemPtr<void> src = std::malloc(100);
    MemPtr<void> dest = std::malloc(100);

    // TODO: Write tests.
    helper.FwdCopyTo({ src, 5 }, { dest, 10 });

    std::free(src.unwrap());
    std::free(dest.unwrap());
}
