import atom.core;
#include "catch2/catch_test_macros.hpp"
// #include <memory>

using namespace Atom;

TEST_CASE("Atom::Memory::MemHelper")
{
    MemHelper helper;

    MutMemPtr<void> src = std::malloc(100);
    MutMemPtr<void> dest = std::malloc(100);

    // TODO: Write tests.
    helper.FwdCopyTo({ src, 5 }, { dest, 10 });

    std::free(src.unwrap());
    std::free(dest.unwrap());
}
