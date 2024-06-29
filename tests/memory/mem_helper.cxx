module;
#include "catch2/catch_test_macros.hpp"

module atom.core.tests:mem_helper;

import atom.core;

using namespace atom;

TEST_CASE("atom::memory::mem_helper")
{
    void* src = std::malloc(100);
    void* dest = std::malloc(100);

    mem_helper::fwd_copy_to(src, 5, dest, 10);

    std::free(src);
    std::free(dest);
}
