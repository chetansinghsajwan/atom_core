#include "Atom/Memory/MemHelper.h"
#include "catch2/catch_all.hpp"
#include <memory>

using namespace Atom;

TEST_CASE("Atom::Memory::MemHelper")
{
    MemHelper helper;

    memptr src = memptr(std::malloc(100));
    memptr dest = memptr(std::malloc(100));

    // TODO: Write tests.
    helper.FwdCopyTo({ src, 5 }, { dest, 10 });

    std::free(src);
    std::free(dest);
}
