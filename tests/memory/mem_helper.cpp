#include "atom.core.h"
#include "catch2/catch_test_macros.hpp"

using namespace atom;

TEST_CASE("atom::memory::mem_helper")
{
    void* src = std::malloc(100);
    void* dest = std::malloc(100);

    mem_helper::fwd_copy_to(src, 5, dest, 10);

    std::free(src);
    std::free(dest);
}
